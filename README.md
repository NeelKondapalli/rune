# rune

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

### Convert image → ASCII frame

```bash
rune_cli input.jpg --width 160 --out frame.json
```

### View ASCII frame in browser

Open `view_art.html` and load the json you want to view in `output/`.

---

## ✦ Frame format

```json
{
  "cols": 160,
  "rows": 90,
  "cells": [
    { "g": "#", "h": 30, "s": 180, "l": 200 },
    ...
  ]
}
```

Each cell represents **one character**:

* `g` → glyph
* `h/s/l` → color (quantized HSL)

This format is renderer-agnostic and streamable.

---

## ✦ Design philosophy

* ASCII is typography, not pixels
* Prefer crisp text rendering over rasterization
* Separate **data generation** from **rendering**
* Favor simple primitives that compose well over time

---

## ✦ Etymology

**rune**

> *noun* — a letter or mark used to convey hidden or ancient meaning

The name is inspired by *Bloodborne* — where runes act as symbolic, low-level modifiers that reshape perception and reality itself.

In the same spirit, `rune` treats characters as fundamental units of transformation:
small symbols, arranged precisely, producing something greater than their parts.

---

## ✦ Status

This project is actively evolving.

Upcoming directions:

* ASCII video playback
* Frame-to-frame transitions
* Compression (RLE / diff frames)
* Procedural effects in character space

---

## ✦ License

MIT

