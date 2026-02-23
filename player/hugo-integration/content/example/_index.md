---
title: "Example ASCII Video Page"
---

# ASCII Video Example

This is an example of how to use the ascii-video shortcode in your Hugo content.

{{< ascii-video path="/videos/example/lh" maxFps="16">}}

## Usage

The ascii-video shortcode supports the following parameters:

- `path`: (required) Path to the folder containing `frames.jsonl.gz` and `manifest.json`
- `gzip`: (optional, default: true) Whether to prefer gzip compressed frames
- `invert`: (optional, default: "0") Invert mode - "0" (no invert), "1" (invert on light), "2" (invert on dark)
- `fontSize`: (optional) Custom font size for the video
- `align`: (optional) Text alignment (e.g., "center")
- `height`: (optional) Custom height for the video container
- `maxFps`: (optional) Maximum frames per second (capped at 18)

## Example Usage

```markdown
{{</* ascii-video path="/videos/my-video" maxFps="16" */>}}

{{</* ascii-video path="/videos/my-video" gzip="false" invert="1" */>}}

{{</* ascii-video path="/videos/my-video" fontSize="0.75rem" align="center" height="40em" */>}}
```
