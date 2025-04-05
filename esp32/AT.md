## Modules
esp8266

Reffers:

1. https://docs.ai-thinker.com/esp8266
2. https://xuhong.blog.csdn.net/article/details/104736261
3. https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-monitor.html
4. https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html


### Station Mode

```

$ stty -F /dev/ttyUSB0 115200  cs8 -cstopb -parenb -echo -icanon -onlcr
$ cat /dev/ttyUSB0
$ echo "AT+GMR" > /dev/ttyUSB0


// As station mode
$ echo AT+CWMODE_CUR=1 > /dev/ttyUSB0
// List all wifi ap
$ echo AT+CWLAP > /dev/ttyUSB0
// 
$ echo AT+CWJAP_CUR=\"abc\",\"0123456789\" > /dev/ttyUSB0
// show local IP
$ echo AT+CIFSR > /dev/ttyUSB0
```

### Server

```
$ echo AT+CIPSTATUS > /dev/ttyUSB0
$ echo AT+CIPMODE? > /dev/ttyUSB0
//  $ echo AT+CIPSTART=\"UDP\" > /dev/ttyUSB0
$ echo AT+CIPMUX=1 > /dev/ttyUSB0
$ echo AT+CIPSERVER=1,50001 > /dev/ttyUSB0

```

```
// UDP
$ echo AT+CIPMUX=1 > /dev/ttyUSB0
$ echo AT+CIPSTART=\"UDP\", \"192.168.0.102\", 8080, 50002, 2 > /dev/ttyUSB0


```



### Info 
```
+STA_CONNECTED:"12:d1:86:de:76:40"

+DIST_STA_IP:"12:d1:86:de:76:40","192.168.4.2"

+DIST_STA_IP:"12:d1:86:de:76:40","192.168.4.2"

+STA_DISCONNECTED:"12:d1:86:de:76:40"

AT+GMR

AT version:1.7.4.0(Jul  8 2020 15:53:04)

SDK version:3.0.5-dev(52383f9)

compile time:Aug 28 2020 14:37:33
```