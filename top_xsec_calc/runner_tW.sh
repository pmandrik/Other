
export OMP_NUM_THREADS=1


#for process in 161 166 171 176; do
  for energy in 7 8 13 14 27 100; do
    echo "PROCESS ENERGY $energy"
    sed     's/%ENERGY%/'$energy'/g' input_tW.DAT > input_tW_$energy.DAT
    
    cd ../
    ./mcfm_omp my_cards/input_tW_$energy.DAT | tee my_cards/log_tW_$energy.txt
    cd my_cards
  done

