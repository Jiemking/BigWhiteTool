adb shell su -c killall BigWhiteTool
adb push outputs\arm64-v8a\BigWhiteTool /data/local/tmp
adb shell su -c chmod 777 /data/local/tmp/BigWhiteTool
adb shell su -c /data/local/tmp/BigWhiteTool
