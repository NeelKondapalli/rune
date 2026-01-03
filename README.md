# ᛚ rune

A minimal ASCII rendering engine for images and video frames.

`rune` converts pixels into character-space frames and renders them crisply in the browser or terminal.  
It treats ASCII as a **first-class rendering medium**.

---

## ✦ What it does

- Converts images → ASCII frames (glyph + color)
- Preserves aspect ratio correctly for text rendering
- Outputs a clean, portable JSON frame format
- Renders frames as **pure ASCII** (DOM text, no canvas blur)
- Supports color via HSL (optional)
- Designed to extend naturally to **ASCII video**

At its core, `rune` is a **character-space framebuffer**.

---

## ✦ Example pipeline

```text
image / video
   ↓
pixel sampling
   ↓
glyph + luminance mapping
   ↓
ASCII frame (JSON)
   ↓
renderer (browser / terminal)
````

An ASCII video is just this pipeline applied **across time**.

---

## ✦ Usage (current)

### Build

```bash
mkdir build 
cd build
cmake --build .
```

```

### Convert video → ASCII frames

```bash
rune_cli --video input.mp4 --width 200 --target-fps 11 --out output/
```

This generates:
- `output/frames/manifest.json` - Video metadata (resolution, FPS, frame count)
- `output/frames/frames.jsonl` - All frames as JSONL (one JSON object per line)
- `output/frames/frames.jsonl.gz` - Gzip-compressed JSONL (~96.5% size reduction)
- `output/frames/frames.txt` - HTML span format for direct rendering

### View ASCII frame in browser

Open `view_art.html` and load the json you want to view in `output/`.

---

## ✦ Browser Integration

### Using JSONL.gz output in web applications

The JSONL.gz format is optimized for streaming ASCII video playback in browsers. Here's how to integrate it into your web app:

#### 1. Output format structure

**manifest.json:**
```json
{
  "cols": 200,
  "rows": 74,
  "channels": 3,
  "fps": 11,
  "frame_count": 161
}
```

**frames.jsonl (one JSON object per line):**
```json
{"cells":[{"g":";","h":0,"s":0,"l":154},{"g":";","h":0,"s":0,"l":224},...]}
{"cells":[{"g":"#","h":30,"s":120,"l":180},{"g":" ","h":0,"s":0,"l":0},...]}
```

Each cell contains:
- `g` - glyph (single ASCII character, 1 byte)
- `h` - hue (0-255, maps to 0-360 degrees)
- `s` - saturation (0-255, maps to 0-100%)
- `l` - luminance (0-255, maps to 0-100%)

#### 2. File size comparison

For a typical video (161 frames at 200x74 resolution):
- **Uncompressed JSONL:** ~69 MB (429 KB/frame)
- **Gzip compressed:** ~2.4 MB (15 KB/frame) - **96.5% reduction**
- **HTML spans:** ~38 MB (236 KB/frame)

The gzip format provides optimal network transfer performance while maintaining full data fidelity.

#### 3. Browser decompression strategies

**Automatic browser decompression** (recommended):
If your web server is configured to serve `.gz` files with `Content-Encoding: gzip`, modern browsers automatically decompress the content.

```javascript
const res = await fetch('/path/to/frames.jsonl.gz');
const text = await res.text();  // Already decompressed by browser
```

**Manual decompression** (fallback):
If the server doesn't set the encoding header, use the `pako` library:

```bash
npm install pako
```

```javascript
import pako from 'pako';

const res = await fetch('/path/to/frames.jsonl.gz');
const buffer = await res.arrayBuffer();
const decompressed = pako.ungzip(new Uint8Array(buffer));
const text = new TextDecoder().decode(decompressed);
```

#### 4. DOM rendering with zero-allocation frame updates and selective diffs

For a production-ready implementation, see the player files:
- `AsciiVideoJsonl.jsx` - React wrapper component
- `asciiPlayerJsonl.js` - Core player logic with all optimizations

Usage:
```jsx
import AsciiVideoJsonl from './components/rune/AsciiVideoJsonl';

function App() {
  return <AsciiVideoJsonl framesPath="/path/to/frames" preferGzip={true} />;
}
```

The component handles:
- Automatic gzip/uncompressed fallback
- Lazy loading with fade-in
- Lifecycle cleanup
- Performance logging

---

## 5. Design philosophy

* ASCII is typography, not pixels
* Prefer crisp text rendering over rasterization
* Separate **data generation** from **rendering**
* Favor simple primitives that compose well over time

---

## 6. Etymology

**rune**

> *noun* — a letter or mark used to convey hidden or ancient meaning

The name is inspired by *Bloodborne* — where runes act as symbolic, low-level modifiers that reshape perception and reality itself.

In the same spirit, `rune` treats characters as fundamental units of transformation:
small symbols, arranged precisely, producing something greater than their parts.

---

## 7. Status

This project is actively evolving.

Upcoming directions:

* ASCII video playback
* Frame-to-frame transitions
* Compression (RLE / diff frames)
* Procedural effects in character space

---

## 8. License

MIT

