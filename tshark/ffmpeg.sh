# this is test for reading m3u8 url
# but youtube and bilibili video cannot find u3m8 url directly
ffmpeg -i "https://s5.sanheyiliao.xyz//videos/202604/11/69d934d4ce6d9f98982d4a14/7f3464/index.m3u8" -c copy test.mkv
ffmpeg -i "outputs/test.mkv" -codec copy "outputs/test.mp4"