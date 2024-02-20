### disk kak primontirovatb kotoriu ostalcia so genevy
mkdir -vp ~/mount/cern_disk
sudo apt install lvm2  
sudo lvdisplay  
``` --- Logical volume ---
  LV Path                /dev/VolGroup00/LogVol00
  LV Name                LogVol00
  VG Name                VolGroup00
  LV UUID                L25jZe-dk03-qmNS-dKGv-SYQ1-kmK4-JHPfzw
  LV Write Access        read/write
  LV Creation host, time , 
  LV Status              available
  # open                 0
  LV Size                <2,72 TiB
  Current LE             89112
  Segments               1
  Allocation             inherit
  Read ahead sectors     auto
  - currently set to     256
  Block device           252:0
```

sudo mount /dev/VolGroup00/LogVol00 /home/i/mount/cern_disk

### kak zapustitb vpn dlya dostupa k ru zone iz kitaya
1. ищем хостинг, доступный из целевой сети
у меня открывался https://sprintbox.ru/ 
Это был KVM VDS, дистрибутив 	Ubuntu 20.04 with Docker and Portainer [x64] 
Тариф S - 349 ₽/мес 
Процессор 	1 ядро
RAM 	1 ГБ
NVMe 	16 ГБ 
2. оплачиваем и запускаем VM
3. поднимаем VPN ванлайном на докере.

Wireguard - проработал неделю, потом перестал работать:
docker run -d --name=wg-easy -e WG_HOST=XXX -e PASSWORD=XXX -v ~/.wg-easy:/etc/wireguard -p 51820:51820/udp -p 51821:51821/tcp --cap-add=NET_ADMIN --cap-add=SYS_MODULE --sysctl="net.ipv4.conf.all.src_valid_mark=1" --sysctl="net.ipv4.ip_forward=1" --restart unless-stopped \weejewel/wg-easy

Shadowsocket - работал:
docker pull shadowsocks/shadowsocks-libev
docker run -e PASSWORD=XXX -p 443:8388 -p 443:8388/udp -d --restart always shadowsocks/shadowsocks-libev

4. скачиваем клиент или просим кого-нибудь, чтобы нам скинули
