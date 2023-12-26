adb wait-for-device shell dumpsys battery reset
adb wait-for-device pull /data/local/tmp/PerformanceCollect
adb wait-for-device shell rm -rf /data/local/tmp/PerformanceCollect