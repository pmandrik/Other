
counter=0
for item in ../*.gif; do

  padd=$(printf %03d $counter) 
  ffmpeg -r 15 -f gif -i $item -b:v 50000K -r 60 "part"$padd".avi"

  counter=$((counter+1)) 
done
