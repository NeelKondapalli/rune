// @ts-nocheck
export function createAsciiPlayer({
    container,
    framesPath,
  }) {
    let frames = [];
    let frameIndex = 0;
  
    let fps = 12;
    let frameTime = 0;
  
    let running = false;
    let rafId = null;
  
    let lastTime = 0;
    let accumulator = 0;
  
    async function loadManifest() {
      const res = await fetch(`${framesPath}/manifest.json`);
      return res.json();
    }
  
    async function loadFrames() {
      const res = await fetch(`${framesPath}/frames.txt`);
      const text = await res.text();
  
      // Each line = one frame (HTML string)
      frames = text
        .split("\n")
        .filter(Boolean)
        .map(line => line.replace(/\\n/g, "\n"));
    }
  
    function renderFrame() {
      container.innerHTML = frames[frameIndex];
      frameIndex = (frameIndex + 1) % frames.length;
    }
  
    function tick(now) {
      if (!running) return;
  
      if (!lastTime) lastTime = now;
      const delta = now - lastTime;
      lastTime = now;
  
      accumulator += delta;
  
      while (accumulator >= frameTime) {
        renderFrame();
        accumulator -= frameTime;
      }
  
      rafId = requestAnimationFrame(tick);
    }
  
    async function start() {
      if (running) return;
  
      const manifest = await loadManifest();
      fps = manifest.fps || 12;
      frameTime = 1000 / fps;
  
      await loadFrames();
  
      frameIndex = 0;
      accumulator = 0;
      lastTime = performance.now();
  
      running = true;
      rafId = requestAnimationFrame(tick);
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
  