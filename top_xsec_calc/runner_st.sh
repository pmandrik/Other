
export OMP_NUM_THREADS=1


#for process in 161 166 171 176; do
for process in 171 176; do
  for energy in 7 8 13 14 27 100; do
    echo "PROCESS ENERGY $energy"
    sed     's/%ENERGY%/'$energy'/g' input_st_alt.DAT > input_st_alt_$process'_'$energy.DAT
    sed -i 's/%PROCESS%/'$process'/g' input_st_alt_$process'_'$energy.DAT

    cd ../
    ./mcfm_omp my_cards/input_st_alt_$process'_'$energy.DAT | tee my_cards/log_st_alt_$process'_'$energy.txt
    cd my_cards
  done
done
