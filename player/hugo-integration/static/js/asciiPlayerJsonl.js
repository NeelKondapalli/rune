// @ts-nocheck
import pako from 'https://cdn.jsdelivr.net/npm/pako@2.1.0/+esm';
import { logger } from './logger.js';

export function createAsciiPlayerJsonl({
  container,
  framesPath,
  preferGzip = true,
  onReady = null,
  maxFps = null,
}) {
  let cellSpans = [];

  // Typed arrays for efficient memory storage
  let glyphs = null;      // Uint8Array - character codes
  let hues = null;        // Uint16Array - hue values (0-360)
  let saturations = null; // Uint8Array - saturation (0-100)
  let lightness = null;   // Uint8Array - lightness (0-255)

  // Pre-allocated caches (ZERO allocation in render loop)
  let glyphCache = null;     // Array of 256 pre-allocated character strings
  let colorStrings = null;   // Pre-computed hsl() strings for all cells

  let frameCount = 0;
  let cellCount = 0;
  let frameIndex = 0;

  let fps = 12;
  let frameTime = 0;
  let cols = 0;
  let rows = 0;

  let running = false;
  let rafId = null;

  let lastTime = 0;
  let accumulator = 0;

  let isFullyLoaded = false;


  let fpsFrameCount = 0;
  let fpsLastTime = 0;
  let fpsUpdateInterval = 10000;

  // Build glyph cache once (all 256 possible ASCII chars)
  function initGlyphCache() {
    glyphCache = new Array(65536);
    for (let i = 0; i < 65536; i++) {
      glyphCache[i] = String.fromCodePoint(i);
    }
  }

  function hslToString(h, s, l) {
    return `hsl(${h}, ${s}%, ${l}%)`;
  }

  async function loadManifest() {
    const res = await fetch(`${framesPath}/manifest.json`);
    const manifest = await res.json();


    cols = manifest.cols;
    rows = manifest.rows;

    // Use maxFps if provided, otherwise use manifest fps, default to 12
    let targetFps = maxFps !== null ? maxFps : (manifest.fps || 12);

    // Cap at 18 fps if target exceeds it
    fps = targetFps > 18 ? 18 : targetFps;

    if (targetFps > 18) {
      logger.log(`[AsciiPlayer] Requested FPS ${targetFps} exceeds cap, using ${fps}`);
    }

    return manifest;
  }

  async function loadFrames() {
    const startTime = performance.now();
    let text;
    let res;


    if (preferGzip) {
      try {
        res = await fetch(`${framesPath}/frames.jsonl.gz`);

        if (res.ok) {
          const encoding = res.headers.get("content-encoding");

          if (encoding === "gzip") {

            logger.log("[AsciiPlayer] Browser auto-decompressed gzip");
            text = await res.text();
          } else {

            logger.log("[AsciiPlayer] Manual gzip decompress");
            const buf = await res.arrayBuffer();
            const decompressed = pako.ungzip(new Uint8Array(buf));
            text = new TextDecoder().decode(decompressed);
          }
        }
      } catch (fetchError) {
        logger.warn('[AsciiPlayer] Gzip fetch failed:', fetchError.message);
        logger.warn(fetchError);
        res = null;
      }
    }

    if (!text) {
      logger.log('[AsciiPlayer] Loading uncompressed...');
      res = await fetch(`${framesPath}/frames.jsonl`);
      if (!res.ok) {
        throw new Error(`Failed to load frames from ${framesPath}`);
      }
      text = await res.text();
    }

    const fetchTime = performance.now() - startTime;
    logger.log(`[AsciiPlayer] Fetch took ${fetchTime.toFixed(0)}ms`);


    const lines = text.trim().split('\n');
    frameCount = lines.length;


    cellCount = rows * cols;


    const totalCells = frameCount * cellCount;
    glyphs = new Uint16Array(totalCells);
    hues = new Uint16Array(totalCells);
    saturations = new Uint8Array(totalCells);
    lightness = new Uint8Array(totalCells);

    logger.log('[AsciiPlayer] Parsing all frames...');
    const parseStartTime = performance.now();


    lines.forEach((line, frameIdx) => {
      const frame = JSON.parse(line);
      frame.cells.forEach((cell, cellIdx) => {
        const idx = frameIdx * cellCount + cellIdx;
        const glyphCode = cell.g.codePointAt(0) || 0;
        glyphs[idx] = glyphCode;

        hues[idx] = cell.h || 0;
        saturations[idx] = cell.s || 0;
        lightness[idx] = cell.l || 0;
      });
    });
    const parseTime = performance.now() - parseStartTime;
    logger.log(`[AsciiPlayer] All frames parsed in ${parseTime.toFixed(0)}ms`);

    // Pre-compute all color strings
    logger.log('[AsciiPlayer] Pre-computing color strings...');
    const colorStartTime = performance.now();
    colorStrings = new Array(totalCells);
    for (let i = 0; i < totalCells; i++) {
      colorStrings[i] = hslToString(hues[i], saturations[i], lightness[i]);
    }
    const colorTime = performance.now() - colorStartTime;
    logger.log(`[AsciiPlayer] Color strings computed in ${colorTime.toFixed(0)}ms`);

    // Initialize glyph cache
    initGlyphCache();

    const totalBytes = glyphs.byteLength + hues.byteLength +
                       saturations.byteLength + lightness.byteLength;
    logger.log(`[AsciiPlayer] Loaded ${frameCount} frames, ${cellCount} cells/frame`);
    logger.log(`[AsciiPlayer] Memory: ${(totalBytes / 1024 / 1024).toFixed(1)} MB (frame data)`);

    return cellCount;
  }

  function initializeDOMStructure(cellCount) {
    cellSpans = [];

    container.innerHTML = '';

    for (let i = 0; i < cellCount; i++) {
      const span = document.createElement('span');
      container.appendChild(span);
      cellSpans.push(span);

      if ((i + 1) % cols === 0 && i < cellCount - 1) {
        container.appendChild(document.createTextNode('\n'));
      }
    }
  }

  function renderFrame() {
    const currOffset = frameIndex * cellCount;

    // First frame: render everything (no diff check possible)
    if (frameIndex === 0) {
      for (let i = 0; i < cellCount; i++) {
        const currIdx = currOffset + i;
        const span = cellSpans[i];

        const glyphCode = glyphs[currIdx];
        const glyphChar = glyphCache[glyphCode];
        span.textContent = glyphChar;
        span.style.color = colorStrings[currIdx];

      }

      // Call onReady after first frame is rendered
      if (onReady) {
        onReady();
        onReady = null;
      }

      frameIndex = 1;
      return;
    }

    // All other frames: diff against previous frame
    const prevFrameIndex = frameIndex - 1;
    const prevOffset = prevFrameIndex * cellCount;

    for (let i = 0; i < cellCount; i++) {
      const currIdx = currOffset + i;
      const prevIdx = prevOffset + i;
      const span = cellSpans[i];

      // Check glyph change (byte comparison against previous frame)
      const currentGlyph = glyphs[currIdx];
      const previousGlyph = glyphs[prevIdx];
      if (previousGlyph !== currentGlyph) {
        span.textContent = glyphCache[currentGlyph];
      }

      // Check color change (byte comparisons against previous frame)
      const currentH = hues[currIdx];
      const currentS = saturations[currIdx];
      const currentL = lightness[currIdx];
      const previousH = hues[prevIdx];
      const previousS = saturations[prevIdx];
      const previousL = lightness[prevIdx];

      if (previousH !== currentH ||
          previousS !== currentS ||
          previousL !== currentL) {
        span.style.color = colorStrings[currIdx]; // Use pre-computed string
      }
    }

    frameIndex = (frameIndex + 1) % frameCount;
  }

  function tick(now) {
    if (!running) return;

    if (!lastTime) lastTime = now;
    const delta = now - lastTime;
    lastTime = now;

    accumulator += delta;

    if (accumulator >= frameTime) {
      renderFrame();
      accumulator = 0;

      // Track FPS
      fpsFrameCount++;
      if (!fpsLastTime) fpsLastTime = now;

      const fpsDelta = now - fpsLastTime;
      if (fpsDelta >= fpsUpdateInterval) {
        const actualFps = (fpsFrameCount / fpsDelta * 1000).toFixed(2);
        logger.log(`[AsciiPlayer] Actual FPS: ${actualFps} | Target: ${fps}`);
        fpsFrameCount = 0;
        fpsLastTime = now;
      }
    }

    rafId = requestAnimationFrame(tick);
  }

  async function start() {
    if (running) return;

    try {
      await loadManifest();
      frameTime = 1000 / fps;

      const cellCount = await loadFrames();

      initializeDOMStructure(cellCount);

      frameIndex = 0;
      accumulator = 0;
      lastTime = performance.now();

      // Reset FPS tracking
      fpsFrameCount = 0;
      fpsLastTime = 0;

      running = true;
      rafId = requestAnimationFrame(tick);
    } catch (error) {
      logger.error('[AsciiPlayer] Failed to start:', error);
      throw error;
    }
  }

  function stop() {
    running = false;

    if (rafId) {
      cancelAnimationFrame(rafId);
      rafId = null;
    }
  }

  return { start, stop };
}
