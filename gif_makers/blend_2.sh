prev="" 
counter=0
for f in ../*.png; do 
  if [ ! -z $prev ]; then 
    padd=$(printf %03d $counter) 
    echo $prev "->" "mix"$padd".png"
    cp $prev "mix"$padd".png"
    counter=$((counter+1)) 
    #padd=$(printf %03d $counter) 
    #echo $prev $f "->" "mix"$padd".png" 
    #cp $prev "mix"$padd".png"
    #counter=$((counter+1)) 
  
    padd=$(printf %03d $counter) 
    echo $prev $f "->" "mix"$padd".png" 
    composite -blend 50 $prev $f "mix"$padd".png"
    counter=$((counter+1)) 
  fi 
  prev=$f 
done

padd=$(printf %03d $counter) 
echo $prev "->" "mix"$padd".png"
cp $prev "mix"$padd".png"
