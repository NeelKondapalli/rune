# React ASCII Video Player

This folder contains React components for playing ASCII video animations in the browser.

## Components

- `AsciiVideoJsonl.jsx` - React component for playing JSONL-based ASCII videos
- `AsciiVideoHtml.jsx` - React component for playing HTML-based ASCII videos
- `asciiPlayerJsonl.js` - Core player implementation for JSONL format
- `asciiPlayerHtml.js` - Core player implementation for HTML format
- `ascii-video.css` - Styles for the ASCII video player

## Usage

### AsciiVideoJsonl Component

```jsx
import AsciiVideoJsonl from './AsciiVideoJsonl';

function App() {
  return (
    <AsciiVideoJsonl
      framesPath="/path/to/video/folder"
      preferGzip={true}
    />
  );
}
```

### Props

- `framesPath` (required): Path to the folder containing `frames.jsonl.gz` (or `frames.jsonl`) and `manifest.json`
- `preferGzip` (optional, default: `true`): Whether to prefer gzip compressed frames

## Video Data Format

The player expects a folder containing:

1. `manifest.json` - Video metadata
   ```json
   {
     "cols": 160,
     "rows": 90,
     "fps": 12
   }
   ```

2. `frames.jsonl.gz` (or `frames.jsonl`) - Frame data in JSONL format
   - Each line is a JSON object representing one frame
   - Each frame contains a `cells` array with glyph and color information

## Features

- Efficient frame-by-frame diffing to minimize DOM updates
- Pre-computed color strings for zero-allocation rendering
- Support for gzip compression with automatic decompression
- FPS tracking and performance monitoring
- Smooth looping playback

## Performance

The player uses several optimization techniques:
- Typed arrays (Uint8Array, Uint16Array) for efficient memory storage
- Pre-allocated glyph and color caches
- Frame diffing to update only changed cells
- RequestAnimationFrame for smooth rendering
