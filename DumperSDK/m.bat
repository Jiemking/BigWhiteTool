adb shell su -c killall UE5Dumper
adb push outputs\arm64-v8a\UE5Dumper /data/local/tmp
adb shell su -c chmod 777 /data/local/tmp/UE5Dumper
adb shell su -c /data/local/tmp/UE5Dumper
