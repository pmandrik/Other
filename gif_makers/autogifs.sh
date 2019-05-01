
srcdir=`pwd`

make_gifs(){
  ffmpeg -r 60 -pattern_type glob -i '*.png' -r 60 out_60.gif

  for gif in "out_*.gif"; do
    gifsicle -i $gif -O3 --scale 1.00 --use-colormap gray  --colors 10 -o fnal_mono_100.gif
    gifsicle -i $gif -O3 --scale 0.75 --use-colormap gray  --colors 10 -o fnal_mono_75.gif
    gifsicle -i $gif -O3 --scale 0.50 --use-colormap gray  --colors 10 -o fnal_mono_50.gif
    gifsicle -i $gif -O3 --scale 0.75 --use-colormap gray  --colors 5  -o fnal_mono_75_2.gif

    gifsicle -i $gif -O3 --scale 1.00 --colors 50 -o fnal_scale_100_50.gif
    gifsicle -i $gif -O3 --scale 0.75 --colors 50 -o fnal_scale_75_50.gif
    gifsicle -i $gif -O3 --scale 0.50 --colors 50 -o fnal_scale_50_50.gif

    gifsicle -i $gif -O3 --scale 0.75 --colors 25 -o fnal_scale_75_25.gif
    gifsicle -i $gif -O3 --scale 0.50 --colors 25 -o fnal_scale_50_25.gif
    gifsicle -i $gif -O3 --scale 0.50 --colors 15 -o fnal_scale_75_15.gif
  done;
}

for item in $@; do
  echo "process ... "$item
  
  cd $srcdir; cd $item;
  mkdir blend_1;
  cd blend_1;

  $srcdir/blend_1.sh
  make_gifs

  mkdir blend_1;
  cd blend_1;
  $srcdir/blend_1.sh
  make_gifs
done;






