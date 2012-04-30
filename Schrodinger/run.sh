#!/bin/bash

./test $1 | ffmpeg -f rawvideo -pix_fmt rgb24 -s 600x600 -r 30 -i - -an -r 30 -vcodec libx264 -vpre libx264-max -threads 0 -b:v 2100k -f mp4 foo.mp4
