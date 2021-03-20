**LINUX**  
chrpath -r "\$ORIGIN" nfraid  
objdump -p nfraid  
readelf -a nfraid | grep "RPATH"   **<- is it [$ORIGIN]???**  
strings notfraid | grep "/usr" **<- is it empty???**  
./linuxdeploy-x86_64.AppImage --appdir build --output appimage **<- can collect libs**  

**LINUX** future reading:  
https://www.reddit.com/r/gamedev/comments/291rth/how_to_deploy_a_game_on_linux/  
http://jorgen.tjer.no/post/2014/05/26/self-contained-game-distribution-on-linux/  
