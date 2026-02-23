# ASCII Video Player

This directory contains player implementations for ASCII video playback in different environments.

## Directory Structure

```
player/
├── react-player/           # React components for ASCII video playback
│   ├── AsciiVideoJsonl.jsx
│   ├── AsciiVideoHtml.jsx
│   ├── asciiPlayerJsonl.js
│   ├── asciiPlayerHtml.js
│   ├── ascii-video.css
│   └── README.md
│
└── hugo-integration/       # Hugo static site integration
    ├── layouts/
    │   └── shortcodes/
    │       └── ascii-video.html
    ├── static/
    │   ├── css/
    │   │   └── style.css
    │   ├── js/
    │   │   ├── asciiPlayerJsonl.js
    │   │   └── logger.js
    │   └── videos/
    │       └── example/
    │           └── lh/
    ├── content/
    │   └── example/
    │       └── _index.md
    └── README.md
```

## Player Types

### React Player
Modern React components for embedding ASCII videos in React applications.

**Key Features:**
- React hooks-based implementation
- Efficient frame diffing
- Gzip compression support
- TypeScript-friendly

[See react-player/README.md for detailed documentation](react-player/README.md)

### Hugo Integration
Complete Hugo static site integration with shortcodes and example content.

**Key Features:**
- Hugo shortcode for easy embedding
- Pre-configured styles
- Example video and content
- Mobile-responsive

[See hugo-integration/README.md for detailed documentation](hugo-integration/README.md)

## Video Data Format

Both players use the same video data format:

### Folder Structure
```
video-name/
├── manifest.json       # Video metadata (dimensions, fps)
└── frames.jsonl.gz     # Compressed frame data (or frames.jsonl)
```

### manifest.json
```json
{
  "cols": 160,
  "rows": 90,
  "fps": 12
}
```

### frames.jsonl
JSONL format with one frame per line:
```json
{"cells":[{"g":"@","h":180,"s":50,"l":50},{"g":" ","h":0,"s":0,"l":0},...]}
{"cells":[{"g":"#","h":180,"s":50,"l":50},{"g":" ","h":0,"s":0,"l":0},...]}
```

Each cell contains:
- `g`: Glyph (character)
- `h`: Hue (0-360)
- `s`: Saturation (0-100)
- `l`: Lightness (0-255)

## Quick Start

### For React Projects
```jsx
import AsciiVideoJsonl from './player/react-player/AsciiVideoJsonl';

<AsciiVideoJsonl framesPath="/videos/my-video" preferGzip={true} />
```

### For Hugo Sites
1. Copy `hugo-integration/layouts/` to your Hugo site
2. Copy `hugo-integration/static/` to your Hugo site
3. In your markdown:
```markdown
{{< ascii-video path="/videos/my-video" maxFps="16" >}}
```

## Performance Characteristics

- **Memory**: ~1-2MB per minute of video (compressed)
- **FPS**: Up to 18 fps (capped for performance)
- **Load time**: ~100-500ms for typical videos
- **Render**: 60fps smooth playback via RAF
- **Optimization**: Frame diffing, pre-computed colors, typed arrays

## Browser Compatibility

Requires modern browsers with:
- ES6 module support
- RequestAnimationFrame
- Typed arrays
- Fetch API

Tested on:
- Chrome/Edge 61+
- Firefox 60+
- Safari 11+

## Creating Video Files

See the main project documentation for information on generating ASCII video files from source videos.

## License

Part of the Rune ASCII video project.
