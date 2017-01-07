#!/bin/bash

./test $1 | ffmpeg -f rawvideo -pix_fmt rgb24 -s 600x600 -r 60 -i - -an -c:v libx264 -preset ultrafast foo.mp4
