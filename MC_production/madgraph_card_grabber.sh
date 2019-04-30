path=$(pwd)
input_tar=$1
process_name="${input_tar##*/}"
process_name="${process_name%*_tarball.tar.xz}"
output_dir=$path/$process_name
card_path="process/madevent/Cards"

mkdir -p $output_dir"_card_temp"

cd $output_dir"_card_temp"
tar -xvf $input_tar gridpack_generation.log
tar -xvf $input_tar $card_path

mkdir -p $process_name
cp $card_path/run_card.dat $process_name/$process_name"_run_card.dat"
cp $card_path/proc_card_mg5.dat $process_name/$process_name"_proc_card.dat"
