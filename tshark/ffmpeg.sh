# this is test for reading m3u8 url
# but youtube and bilibili video cannot find u3m8 url directly
ffmpeg -i "https://tx-safety-video.acfun.cn/mediacloud/acfun/acfun_video/hls/134373999005147136_HLS_720P_1.m3u8?pkey=ABB1578urHIwRjG-k6SJoRVHimeBcTtXA8CBpy8dGkzaJXtyaX8wKmnUqglob6daCoYOrFxidT2tktdwULk2w_co9bRST07T9druu4m1g4UtjJSm5RoXyZnXM0nInvJsbk3QDx-BMV28yalKL_TAa5oN20KKedvsB50e-txwN6Mo7EQnBgPl6nJVWAj8nKr1Jwm2beXPh-AuCZtZxD-F1IexT6GjTo7epaeEp0gfr7--kd1ZdRwR94KWZZTxt52Ms9c&safety_id=AAKm9HyTLoJNUcWCXAqslGU8" -c copy test.mkv
ffmpeg -i "outputs/test.mkv" -codec copy "outputs/test.mp4"