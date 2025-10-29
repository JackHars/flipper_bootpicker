BootPicker Icon Requirements
============================

The application requires a 10x10 pixel icon in PNG format named:
  bootpicker_10px.png

Icon Design Suggestion:
- A simple boot symbol (e.g., boot with arrow, or F12 key representation)
- Black and white (1-bit) or grayscale
- 10x10 pixels

You can create this icon using:
1. GIMP, Photoshop, or any image editor
2. Online pixel art tools
3. Convert from XBM format

Temporary Workaround:
---------------------
If you don't have an icon ready, you can:
1. Use a blank 10x10 PNG
2. Copy an existing Flipper app icon
3. The app will build without the icon but may not display properly in the menu

To create a simple blank icon using ImageMagick:
  convert -size 10x10 xc:white bootpicker_10px.png

Or use this simple representation (can be converted to PNG):

  ##########
  #........#
  #..####..#
  #..#..#..#
  #..####..#
  #..#.....#
  #..#.....#
  #........#
  ##########

This represents an "F" character (for F12).

