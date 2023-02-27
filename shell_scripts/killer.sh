#!/bin/bash

# this guy I made to to kill process with too much RAM or CPU usage on my old bad poor IHEP PC machine :D

Check_Repeating_Time=3; # in seconds
Max_CPU_Usage='25.0'; #%
Warning_RAM_Usage='30.0'; #%
Max_RAM_Usage='40.0'; #%
Log_Path='./auto_killer_log'; # path to file when killing logs will be writed

while [ 1 ]; do

    ps -aux | 
    awk '{
        Username = $1;
        Proc_Name = $11;
        CPU_Usage = $3;
        RAM_Usage = $4;
        PID = $2;
        TTY = $7;

        if((RAM_Usage >= '$Max_RAM_Usage' ) &&  !($1 == "root" || $1 == "daemon"))
        {
          print Proc_Name RAM_Usage;

                system ("echo \"\" >> /dev/" TTY);
                system ("echo \"Process "Proc_Name" was killed because it used to much of system resources!\" >> /dev/" TTY );
                system ("echo \"\" >> /dev/" TTY);
                system ("kill -9 " PID);
                Data = "date";
                Data |getline Str_Data;
                system ("echo \""Str_Data"  "Username"  "Proc_Name" "TTY"\" >> '$Log_Path'");

        }

        if((RAM_Usage >= '$Warning_RAM_Usage' ) &&  !($1 == "root" || $1 == "daemon"))
        {
          print Proc_Name RAM_Usage "<WARNING";
        }
    }';

    if [ -e ./auto_killer_data.new ]; then
        mv ./auto_killer_data.new ./auto_killer_data
    else    
        echo '' > ./auto_killer_data
    fi

    #We wait fo a while and repeate process
    sleep $Check_Repeating_Time\s;
done;
