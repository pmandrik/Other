
expect -c ' spawn -noecho root -l
            send "gSystem->Load(\"/home/pmandrik/soft/hepmc2.07-beta00/lib/libHepMC.so.4.0.0\");\r"
            send ".x hepmc_test.cpp\r"
            interact '



















