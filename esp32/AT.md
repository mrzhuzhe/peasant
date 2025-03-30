### CMD

```

$ stty -F /dev/ttyUSB0 115200  cs8 -cstopb -parenb -echo 
$ cat /dev/ttyUSB0
$ echo "AT+GMR" > /dev/ttyUSB0

AT+GMR


$ AT+CWJAP_CUR ="abc","0123456789" 

```

### Info 

+STA_CONNECTED:"12:d1:86:de:76:40"

+DIST_STA_IP:"12:d1:86:de:76:40","192.168.4.2"

+DIST_STA_IP:"12:d1:86:de:76:40","192.168.4.2"

+STA_DISCONNECTED:"12:d1:86:de:76:40"

AT+GMR

AT version:1.7.4.0(Jul  8 2020 15:53:04)

SDK version:3.0.5-dev(52383f9)

compile time:Aug 28 2020 14:37:33
