# Hugo ASCII Video Integration

This folder contains everything needed to integrate ASCII video playback into a Hugo static site.

## Structure

```
hugo-integration/
├── layouts/
│   └── shortcodes/
│       └── ascii-video.html          # Hugo shortcode template
├── static/
│   ├── css/
│   │   └── ascii-video.css           # ASCII video styles only
│   ├── js/
│   │   ├── asciiPlayerJsonl.js       # ASCII video player implementation
│   │   └── logger.js                 # Logging utility
│   └── videos/
│       └── example/
│           └── lh/                   # Example video folder
│               ├── frames.jsonl.gz   # Compressed video frames
│               └── manifest.json     # Video metadata
└── content/
    └── example/
        └── _index.md                 # Example page with usage

```

## Installation

1. Copy the `layouts/` folder contents to your Hugo site's `layouts/` directory
2. Copy the `static/` folder contents to your Hugo site's `static/` directory
3. Include `ascii-video.css` in your site (add to your main CSS or link in head)
4. Place your ASCII video folders (containing `frames.jsonl.gz` and `manifest.json`) in `static/videos/`

## Usage

In your Hugo markdown content files, use the `ascii-video` shortcode:

```markdown
{{< ascii-video path="/videos/example/lh" maxFps="16">}}
```

### Shortcode Parameters

- `path` (required): Path to the folder containing video files (relative to `/static`)
- `gzip` (optional, default: `true`): Whether to prefer gzip compressed frames
- `invert` (optional, default: `"0"`): Invert mode
  - `"0"` - No invert
  - `"1"` - Invert on light background
  - `"2"` - Invert on dark background
- `fontSize` (optional): Custom font size (e.g., `"0.75rem"`)
- `align` (optional): Text alignment (e.g., `"center"`)
- `height` (optional): Custom height (e.g., `"40em"`)
- `maxFps` (optional): Maximum frames per second (capped at 18)

### Examples

Basic usage:
```markdown
{{< ascii-video path="/videos/my-video" >}}
```

With custom settings:
```markdown
{{< ascii-video path="/videos/my-video" maxFps="16" invert="1" fontSize="0.6rem" align="center" >}}
```

## Video Data Format

Each video folder in `static/videos/` should contain:

1. **manifest.json** - Video metadata
   ```json
   {
     "cols": 160,
     "rows": 90,
     "fps": 12
   }
   ```

2. **frames.jsonl.gz** (or **frames.jsonl**) - Frame data
   - Compressed JSONL file with one frame per line
   - Each frame contains cell data with glyphs and colors

## Features

- Automatic gzip decompression (browser or manual with pako.js)
- Frame-by-frame diffing for efficient rendering
- Pre-computed color strings for performance
- Responsive design with mobile support
- Dark mode support via CSS
- Smooth fade-in on load

## Dependencies

The shortcode uses:
- [pako](https://cdn.jsdelivr.net/npm/pako@2.1.0/+esm) - For gzip decompression (loaded from CDN)

## Customization

### Styling

The ASCII video player styles are in `static/css/ascii-video.css`. You can customize:

- Font family (default: monospace)
- Font size (default: 0.5rem, 0.25rem on mobile)
- Colors (default: white text on transparent background)
- Container dimensions
- Mobile responsiveness
- Invert filters for light/dark mode

### JavaScript

The player implementation is in `static/js/asciiPlayerJsonl.js`. It's written as an ES module and exported via the `createAsciiPlayerJsonl` function.

## Browser Compatibility

Modern browsers with ES6 module support required:
- Chrome/Edge 61+
- Firefox 60+
- Safari 11+

## Performance

The player is optimized for:
- Minimal DOM updates via frame diffing
- Efficient memory usage with typed arrays
- Smooth 60fps rendering (video FPS capped at 18)
- Fast load times with gzip compression
