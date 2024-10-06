# MH Image Tool

This program serves as the first tool for what could become a full game engine
pipeline. An engine is only as good as what you can see, so the graphics viewing
and editing tool comes first.

MHIT has the following command line structure: `./mhit mode [arg0 [arg1 ...]]`

**Note:** A mode is always supplied and every mode (besides `version`) requires
at least one argument, usually the file to read from. If MHIT *is* run without a
mode, the standard help text is printed as if the user had run `./mhit help`.

## Accepted Program Modes

Below are the currently accepted modes for use with MHIT, followed by a small
description:

|  Mode  |         Short Description of Fnuctionality            |  Arguments  |
|:------:|:-----------------------------------------------------:|:-----------:|
| `help` | Offers help for individual modes and MHIT as a whole. |  Mode Name  |
| `version` | Displays version and compile information for MHIT. |    NONE     |
| `info` | Dumps information about the given file to the terminal. | Image File|
| `cons` | Draws a sprite to the terminal with ANSI control codes. |Sprite File|
| `convert` |  Creates an MHS file, using two source BMP files.  | Source BMPs |
| `vis-test` | Runs one of five SDL tests, supplied as a letter. | Test Letter |
| `display` | Draws the given sprite in an interactive SDL window.| Sprite File|

## `help` Mode

As mentioned above, this is the "default" mode of MHIT. When run without the
name of another mode, `help` will, as expected, output general help for MHIT as
a whole, listing both usage information and the accepted modes. When provided
with the name of another mode, `help` will output information specific to the
supplied mode.

## `version` Mode

This mode is quite simple to understand from its name alone. However, not only
does this mode make MHIT output its version number, but also the version of GCC
it was compiled with and the time that `main.c` was last compiled.

## `info` Mode

This mode dumps all information about supported files to the terminal. Currently
supported file types are `BMP` (Windows bitmap) and `MHS` (MH Paletted Sprite).

Running `info` on an `MHS` file will output its version, the sprite's color
indicies, the amount of palettes, the colors per palette, the sprite's palette
data, and the palette itself. While the palette is always printed, it does not
appear correctly within terminals and consoles that do not support truecolor
text through ANSI control codes.

Running `info` on a `BMP` file outputs a parsing of both the standard bitmap
file header and the Device Independant Bitmap header (DIB), showing all possible
information the user could need about a given bitmap image.

## `cons` Mode

Originally used to visualize incredibly small and *handwritten* MHS files
before the SDL-based visualizer had been written, this is now a legacy mode
kept for the novelty of a colored, terminal-/text-based image viewer. **Do
note**, however, that the drawing space is limited to the text area of the
terminal. This means that, if the supplied sprite is taller than the terminal or
the space remaining to its bottom, the sprite will be cut off with the final
line being replaced by the post-program shell input.

## `convert` Mode

Being an image tool that supports a custom format, this mode is a necessity,
though it currently can only convert two Windows bitmaps *to* a single MHS file.
The two supplied bitmap files are, in order, a sixteen-gray "skeleton" and an,
at maximum, sixteen-by-sixteen pixel palette reference. These two images are
read together to create an MHS sprite. **This mode is the *first* of MHIT's two
"primary modes."**

## `vis-test` Mode

This mode exists solely to test SDL2. There are five tests ,all of which are
described below:

| Test Letter |                            Test Action                         |
|:---------:|:----------------------------------------------------------------:|
|   `A`   |      Displays a salmon-like color throughout the whole window.     |
|   `B`   | Draws ramdomly-sized, ramdomly-colored boxes to the display window.|
|   `C`   |Draws randomly-colored lines with random starting and ending points.|
|   `D`   | Draws randomly-colored, randomly-positioned pixels to the window.  |
|   `E`   |                   Tests the built-in font Minif.                   |

## `display` Mode

This is the second of MHIT's "primary modes," and the greater of the two. This
mode allows for the user to view the contents of MHS files in a more accessable,
visual format that allows for interaction. The "interaction keys" are given
within the mode's help text, and are reproduced below:

|   Key   |                Action Upon Keypress               |
|:-------:|:-------------------------------------------------:|
|  `ESC`  | Stops the sprite visualizer and exits the program |
|  `LEFT` |         Shifts the sprite left by one unit        |
| `RIGHT` |        Shifts the sprite right by one unit        |
|   `UP`  |          Shifts the sprite up by one unit         |
|  `DOWN` |         Shifts the sprite down by one unit        |
|   `]`   |      Increment the displayed palette number       |
|   `[`   |      Decrement the displayed palette number       |
|   `.`   |           Increase sprite display scale           |
|   `,`   |           Decrease sprite display scale           |
|  `TAB`  |          Reset display to default values          |

## Roadmap

### Pressing:

- Bugfixes, especially around memory allocations.
- Optimizations where applicable
- Conversion from MHS to Windows Bitmap

### Will Be Added Soon™:

- Information flags for MHS files
- RLE compression

### Will Be Added Eventually:

- Animated MHS (tentatively MH Animated/MHA)

### May Be Added:

- Other format support for `convert`:
  - PNG, JPG, PCX
  - TAR, ZIP, RAR

## Credits

MHIT was first released and published to GitHub on October 2nd, 2024 by Mike
Hensley. It is licensed under the MIT License.
