Arduino code for Clockwork Butterfly's big performance in July 2016.

## Building with PlatformIO

Sample usage for an Arduino Uno after
[installing platform.io](http://docs.platformio.org/en/latest/installation.html):
```
platformio run -e arduinouno --target upload
```

Or for a Teensy (3.0 or LC, respectively):
```
platformio run -e teensy30 --target upload
platformio run -e teensylc --target upload
```

Or to build for all platforms specified in the [`platformio.ini`](platformio.ini):
```
platformio run
```

## Installing on Teensy boards

Note that if you are using the Arduino IDE with a Teensy microcontroller you will need to install
[Teensyduino](https://www.pjrc.com/teensy/td_download.html).
