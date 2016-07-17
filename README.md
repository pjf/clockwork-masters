Arduino code for [Clockwork Butterfly](http://www.clockworkbutterfly.net/)'s [Celestial Bodies](https://www.facebook.com/events/247737515598696/) performance in July 2016.

## What is this?

This repository contains designs and software for an arudino-compatible board used to run lighting effects in live performances. A typical board consists of:

- 1 x Teensy-LC microcontroller, which runs the show.
- 3 x Strings of LED lights, to delight the audience.
- 3 x 2N7000 MOSFETs, for low-side driving the LEDs strands.
- 3 x 10Ω resistors, for current limiting the LED connections
- 3 x 33Ω resistors, to stop the MOSFETs from ringing.
- 3 x Hall Effect Sensors (4 in the original build) for user configurable events.
- 1 x 3-axis accelerometer, for movement-reactive effects.
- 1 x 4-dip switch bank, for configuring the board without needing to reprogram.
- 1 x S7V7F5 5V step-up/step-down voltage regulator, giving us lots of power flexibility.

For simplicity, the boards used in the performance run off 5V USB batteries, but we've also had success withe 3.3V CR2032 coin batteries, and 9V cells. The main difference is higher voltages allow for more juice to be pumped to the lights, which affects their brightness.

## Things to find here

Circuit diagrams are in the `kicad` directory, with various reference materials (including the main board design) in the `reference` directory. This main directory contains the source code for the boards.

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

## What does it look like?

- [Promotional Video](https://www.youtube.com/watch?v=G8GqFaP5fc0)
- [Video: V1 Board being tested, with commentary](https://www.youtube.com/watch?v=aZwal5Rc-4o)
- [Video: V3 Board being tested, no commentary](https://www.youtube.com/watch?v=_FixIgUwdGU)

### Pictures:

![Finished board, front](http://i.imgur.com/OjaWQ3j.jpg)
![Finished board, back](http://i.imgur.com/Cn9hbLk.jpg)
![Display piece audience members can interact with](http://i.imgur.com/IH1JgYL.jpg)
