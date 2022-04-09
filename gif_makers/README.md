ffmpeg -r 60 -pattern_type glob -i '*.png' -r 60 out.gif

convert out.gif -coalesce   -duplicate 1,-2-1 -quiet -layers OptimizePlus  -loop 0 patrol_cycle.gif

convert out_scale_3.gif -monochrome out_scale_4.gif

gifsicle -i out.gif -O3 --scale 0.75 -o out_scale.gif
gifsicle -i out.gif -O3 --colors 10 -o out_scale.gif

gifsicle -i out.gif -O3 --scale 1.00 --use-colormap gray  --colors 10 -o out_mono.gif
gifsicle -i out.gif -O3 --scale 0.5 --use-colormap gray --color-method median-cut --colors 10 -o out_mono.gif
gifsicle -i out.gif -O3 --scale 0.75 --colors 50 -o out_scale.gif

convert out.gif -monochrome out_mono.gif

ffmpeg -f concat -safe 0 -i <(find . -name '*.avi' -printf "file '$PWD/%p'\n") -c:v libvpx-vp9 -c:a libopus  -c copy output.avi

**RGBA -> RGB**  
convert Devochka_4.png -flatten +matte Devochka_4.png  

**VIDEO GOOD QUALITY**  
ffmpeg -r 60 -pattern_type glob -i '*.png' -r 60 -b 4M out.mp4  

**SPEED X2**  
ffmpeg -i out.mp4 -map 0:v -c:v copy -bsf:v h264_mp4toannexb raw.h264  
ffmpeg -fflags +genpts -r 15 -i raw.h264 -c:v copy outx.mp4  

**Width not divisible by 2**  
ffmpeg -i 11222.avi -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" 1111.mp4

**ping pong loop**  
 convert out.gif -coalesce -duplicate 1,-2-1 -quiet -layers OptimizePlus -loop 0 OutputPatrolCycleFile.gif

**change gif speed**  
 convert -delay 10x100 your.gif your_slow.gif

