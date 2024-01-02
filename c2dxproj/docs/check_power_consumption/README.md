# version

## v0.1
- 2024.01.02
- Collect FPS and Power consumtion every second.
- Disable battery charge during check via 'adb wait-for-device shell dumpsys battery unplug', it is called in 'PushAndRun.bat'.
- Collect FPS via 'dumpsys SurfaceFlinger --latency' every second.
- Collect Power consumtion by reading '/sys/class/power_supply/battery/current_now' and '/sys/class/power_supply/battery/voltage_now' and multifly these, and multiply '-1' to convert.
- Collect Power consumtion 2 times a second, and use average of these.
