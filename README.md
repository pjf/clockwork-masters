Arduino code for Clockwork Butterfly's big performance in July 2016.

Sample usage for an Arduino Uno after
[installing platform.io](http://docs.platformio.org/en/latest/installation.html):
```
platformio run -e arduinouno --target upload
```

Or for a Teensy:
```
platformio run -e teensy --target upload
```

Or to build for all platforms specified in the [`platformio.ini`](platformio.ini):
```
platformio run
```
