
export OMP_NUM_THREADS=1

for energy in 7; do
  echo "PROCESS ENERGY $energy"
  sed 's/%ENERGY%/'$energy'/g' input.DAT > input_$energy.DAT

  cd ../
  ./mcfm_omp my_cards/input_$energy.DAT | tee my_cards/log_$energy.txt
  cd my_cards
done
