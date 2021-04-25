#Fonts
* [Jetbrains Mono](https://www.jetbrains.com/fr-fr/lp/mono/)
* [Awesome font from LVGL](https://lvgl.io/assets/others/FontAwesome5-Solid+Brands+Regular.woff)

## Generate the fonts:

 * Open the [LVGL font converter](https://lvgl.io/tools/fontconverter)
 * Name : jetbrains_mono_bold_20
 * Size : 20
 * Bpp : 1 bit-per-pixel
 * Do not enable font compression and horizontal subpixel hinting
 * Load the file `JetBrainsMono-Bold.woff` and specify the following range : `0x20-0x7f, 0x410-0x44f`
 * Add a 2nd font, load the file `FontAwesome5-Solid+Brands+Regular.woff` and specify the following range : `0xf293, 0xf294, 0xf244, 0xf240, 0xf242, 0xf243, 0xf241, 0xf54b, 0xf21e, 0xf1e6, 0xf54b, 0xf017, 0xf129, 0xf03a, 0xf185, 0xf560, 0xf001, 0xf3fd, 0xf069, 0xf1fc, 0xf45d`
 * Click on Convert, and download the file `jetbrains_mono_bold_20.c` and copy it in `src/DisplayApp/Fonts`
  
Add new symbols:
 * Browse the [cheatsheet](https://fontawesome.com/cheatsheet/free/solid) and find your new symbols
 * For each symbol, add its hex code (0xf641 for the 'Ad' icon, for example) to the *Range* list
 * Convert this hex value into a UTF-8 code using [this site](http://www.ltg.ed.ac.uk/~richard/utf-8.cgi?input=f185&mode=hex)
 * Define the new symbols in `src/DisplayApp/Screens/Symbols.h`: 
```
static constex char* newSymbol = "\xEF\x86\x85";
```
---
## Pinetile Lite Fonts

- General font : OpenSans-Regular.ttf
  * Size     : 20
  * Bpp      : 2 bit-per-pixel
  * Range    : 0x20-0x7f,0x2020
  * Symbols  : ãÃâÂçÇôÔõÕéèÉÈíìÍÌúùÙÚáÁàÀ

- Emoji : NotoEmoji-Regular.ttf
  * Size     : 20
  * Bpp      : 2 bit-per-pixel
  * Range    : 0x1F600-0x1F64F

- Font for Watch faces numbers - 071MKSDBoldB.TTF
  * Size     : 42,76,90
  * Bpp      : 2 bit-per-pixel
  * Symbols  : %-/0123456789:

#### Fonts created in https://icomoon.io/app ( "Pinetime Sys 20.json" and "Pinetime Sys 48.json" )
#### More info in Symbols.h

- Systems symbols 20 - icons_sys_20.ttf
  * Size     : 20
  * Bpp      : 2 bit-per-pixel
  * Range    : 0xe90d, 0xe902, 0xe909, 0xe906, 0xe907, 0xe903, 0xe90a, 0xe900, 0xe90b, 0xe90c, 0xe904, 0xe910, 0xe911, 0xe908, 0xe905, 0xe901, 0xe90d, 0xe920, 0xe921, 0xe922, 0xe923, 0xe924, 0xe90f

- Systems symbols 48 - icons_sys_48.ttf
  * Size     : 48
  * Bpp      : 2 bit-per-pixel
  * Range    : 0xe908, 0xe901, 0xe900, 0xe903, 0xe909, 0xe904, 0xe905, 0xe906, 0xe907, 0xe90b, 0xe90c, 0xe902, 0xe910, 0xe914


---
### Convert to .bin, only clock fonts are converted to .bin

```
lv_font_conv --font icons_sys_20.ttf -r 0xe920 -r 0xe924 -r 0xe921 -r 0xe922 -r 0xe923 -r 0xe906 -r 0xe907 -r 0xe901 -r 0xe903 -r 0xe905 -r 0xe904 -r 0xe902 -r 0xe900 -r 0xe908 -r 0xe909 -r 0xe90a -r 0xe90b -r 0xe90c --size 20 --format bin --bpp 2 --no-compress -o lv_font_sys_20.bin
```
```
lv_font_conv --font icons_sys_48.ttf -r 0xe908 -r 0xe901 -r 0xe900 -r 0xe903 -r 0xe909 -r 0xe904 -r 0xe905 -r 0xe906 -r 0xe907 -r 0xe90b -r 0xe90c -r 0xe902 -r 0xe910 -r 0xe914 --size 48 --format bin --bpp 2 --no-compress -o lv_font_sys_48.bin
```
```
lv_font_conv --font OpenSans-Regular.ttf --symbols ãÃâÂçÇôÔõÕéèÉÈíìÍÌúùÙÚáÁàÀ -r '0x20-0x7f,0x2020' --font NotoEmoji-Regular.ttf -r '0x1F600-0x1F64F' --size 20 --format bin --bpp 2 --no-compress -o lv_font_roboto_20.bin
```
---

lv_font_conv --font icons_sys_20.ttf -r 0xe920 -r 0xe924 -r 0xe921 -r 0xe922 -r 0xe923 -r 0xe906 -r 0xe907 -r 0xe901 -r 0xe903 -r 0xe905 -r 0xe904 -r 0xe902 -r 0xe900 -r 0xe908 -r 0xe909 -r 0xe90a -r 0xe90b -r 0xe90c --size 20 --format lvgl --bpp 2 --no-compress -o lv_font_sys_20.c

```
lv_font_conv --font OpenSans-Regular.ttf --symbols ãÃâÂçÇôÔõÕéèÉÈíìÍÌúùÙÚáÁàÀ° -r '0x20-0x7f,0x2020,0xB0' --font NotoEmoji-Regular.ttf -r '0x1F600-0x1F64F' --size 20 --format lvgl --bpp 2 --no-compress -o lv_font_roboto_20.c
```

#### This fonts are in external Flash

```
lv_font_conv --font 071MKSDBoldB.TTF --symbols %-/0123456789: -r 0x20 --size 42 --format bin --bpp 2 --no-compress -o lv_font_clock_42.bin
```
```
lv_font_conv --font 071MKSDBoldB.TTF --symbols %-/0123456789: -r 0x20 --size 76 --format bin --bpp 2 --no-compress -o lv_font_clock_76.bin
```
```
lv_font_conv --font 071MKSDBoldB.TTF --symbols %-/0123456789: -r 0x20 --size 90 --format bin --bpp 2 --no-compress -o lv_font_clock_90.bin
```