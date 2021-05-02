#!/bin/sh
echo "\nConvert resources Images to LVGL .BIN files\n"

echo "\n#### True Color files"

rm ./resbinfiles/*.bin

## true_color
FILESTC=./resourcesfiles/*.png
for f in $FILESTC
do
    filename=$(basename -- "$f")
    filename="${filename%.*}"
    echo "Converting $f \t $filename"
    php ./utils/img_conv_core.php "name=./resbinfiles/$filename&img=$f&format=bin_565_swap&cf=true_color"
done

echo "\n#### True Color Alpha files"
## true_color_alpha
FILESTCA=./resourcesfilesa/*.png
for f in $FILESTCA
do
    filename=$(basename -- "$f")
    filename="${filename%.*}"
    echo "Converting $f \t $filename"
    php ./utils/img_conv_core.php "name=./resbinfiles/$filename&img=$f&format=bin_565_swap&cf=true_color_alpha"
done

