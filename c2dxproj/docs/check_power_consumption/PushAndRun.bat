adb wait-for-device shell dumpsys battery unplug
adb wait-for-device push performance_collector /data/local/tmp/performance_collector
adb wait-for-device shell chmod +x /data/local/tmp/performance_collector

adb wait-for-device shell /data/local/tmp/performance_collector com.cocos.c2dxproj