EESchema Schematic File Version 2
LIBS:freetronics_schematic
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:teensy_3.1
LIBS:S7V7F5
LIBS:AM3X
LIBS:clockwork-bfly-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Teensy_3.1 U1
U 1 1 571F694B
P 7100 3550
F 0 "U1" H 7150 4600 60  0000 C CNN
F 1 "Teensy LC" H 7200 5050 60  0000 C CNN
F 2 "Mechanical_Sockets:Socket_DIN41612-CaseC2-AC-Male-32Pin-2rows" H 7200 3550 60  0001 C CNN
F 3 "" H 7200 3550 60  0000 C CNN
	1    7100 3550
	1    0    0    -1  
$EndComp
$Comp
L S7V7F5-RESCUE-test PS1
U 1 1 571F6E8C
P 8950 1050
F 0 "PS1" H 8950 950 50  0000 C CNN
F 1 "S7V7F5" H 8950 1150 50  0000 C CNN
F 2 "Power_Integrations:TO-220" H 8950 1050 50  0001 C CNN
F 3 "DOCUMENTATION" H 8950 1050 50  0001 C CNN
	1    8950 1050
	-1   0    0    -1  
$EndComp
$Comp
L AM3X U2
U 1 1 571F74B7
P 10300 4050
F 0 "U2" H 10300 3950 50  0000 C CNN
F 1 "AM3X" H 10300 4150 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x05_Pitch2.00mm" H 10300 4050 50  0001 C CNN
F 3 "DOCUMENTATION" H 10300 4050 50  0001 C CNN
	1    10300 4050
	1    0    0    -1  
$EndComp
Text Label 8100 1800 0    60   ~ 0
+3.3V
NoConn ~ 11050 3850
NoConn ~ 11050 3950
NoConn ~ 11050 4050
NoConn ~ 11050 4150
NoConn ~ 11050 4250
NoConn ~ 8100 3750
NoConn ~ 8100 3900
NoConn ~ 8100 4050
NoConn ~ 8100 4200
NoConn ~ 6850 4750
NoConn ~ 7000 4750
NoConn ~ 7150 4750
NoConn ~ 7300 4750
NoConn ~ 7450 4750
NoConn ~ 6100 2700
NoConn ~ 6100 3000
NoConn ~ 6100 3150
NoConn ~ 6100 3600
Text Label 6100 1750 0    60   ~ 0
GND
$Comp
L GND #PWR01
U 1 1 571F8A17
P 10100 1050
F 0 "#PWR01" H 10100 800 50  0001 C CNN
F 1 "GND" H 10100 900 50  0000 C CNN
F 2 "" H 10100 1050 50  0000 C CNN
F 3 "" H 10100 1050 50  0000 C CNN
	1    10100 1050
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG02
U 1 1 571F8ED4
P 10050 1150
F 0 "#FLG02" H 10050 1245 50  0001 C CNN
F 1 "PWR_FLAG" H 10050 1330 50  0000 C CNN
F 2 "" H 10050 1150 50  0000 C CNN
F 3 "" H 10050 1150 50  0000 C CNN
	1    10050 1150
	-1   0    0    1   
$EndComp
$Comp
L +BATT #PWR03
U 1 1 571F8F1D
P 10100 800
F 0 "#PWR03" H 10100 650 50  0001 C CNN
F 1 "+BATT" H 10100 940 50  0000 C CNN
F 2 "" H 10100 800 50  0000 C CNN
F 3 "" H 10100 800 50  0000 C CNN
	1    10100 800 
	0    1    1    0   
$EndComp
$Comp
L PWR_FLAG #FLG04
U 1 1 571F8F5E
P 9950 850
F 0 "#FLG04" H 9950 945 50  0001 C CNN
F 1 "PWR_FLAG" H 9950 1030 50  0000 C CNN
F 2 "" H 9950 850 50  0000 C CNN
F 3 "" H 9950 850 50  0000 C CNN
	1    9950 850 
	1    0    0    -1  
$EndComp
Text Notes 8150 2000 0    60   ~ 0
3.3V - 100mA max
Text Label 9350 1650 0    60   ~ 0
+5V
$Comp
L MOSFET_N Q?
U 1 1 5722B9B5
P 4200 2850
F 0 "Q?" H 4110 3020 60  0000 R CNN
F 1 "MOSFET_N" H 4260 2600 60  0000 R CNN
F 2 "" H 4200 2850 60  0000 C CNN
F 3 "" H 4200 2850 60  0000 C CNN
	1    4200 2850
	-1   0    0    -1  
$EndComp
$Comp
L LED LED?
U 1 1 5722BA38
P 4100 1750
F 0 "LED?" H 4100 1850 50  0000 C CNN
F 1 "LED" H 4100 1650 50  0000 C CNN
F 2 "" H 4100 1750 60  0000 C CNN
F 3 "" H 4100 1750 60  0000 C CNN
	1    4100 1750
	0    -1   1    0   
$EndComp
$Comp
L R R?
U 1 1 5722BABB
P 4100 2200
F 0 "R?" V 4180 2200 50  0000 C CNN
F 1 "10R" V 4100 2200 50  0000 C CNN
F 2 "" H 4100 2200 60  0000 C CNN
F 3 "" H 4100 2200 60  0000 C CNN
	1    4100 2200
	-1   0    0    -1  
$EndComp
Text Label 4100 1200 1    60   ~ 0
+5V
$Comp
L GND #PWR?
U 1 1 5722BEEF
P 4100 3400
F 0 "#PWR?" H 4100 3150 50  0001 C CNN
F 1 "GND" H 4100 3250 50  0000 C CNN
F 2 "" H 4100 3400 50  0000 C CNN
F 3 "" H 4100 3400 50  0000 C CNN
	1    4100 3400
	-1   0    0    -1  
$EndComp
Text Notes 3400 1800 0    60   ~ 0
LED strand\n
Text Notes 4200 3250 0    60   ~ 0
2N7000\n
Text Notes 5050 3350 0    60   ~ 0
PIN 6- LED Strand 2
Text Notes 5000 3750 0    60   ~ 0
PIN 9 - LED Strand 3\n
Text Notes 4900 2700 0    60   ~ 0
PIN 3 - LED Strand 1\n
Text Notes 7450 7500 0    60   ~ 0
Clockwork Butterfly Masters Board - V3
Text Notes 10050 4600 0    60   ~ 0
Accelerometer
Text Notes 750  7450 0    60   ~ 0
3.3V power budget: 100mA from Teensy LC.\nHall effect sensors  require 10mA each, and *must* run on 3.3V\nAccel requires 0.6mA, and can run on either 5 or 3.3V.\nMicrophone can run on either, unknown power consumption.
Text Notes 4200 1300 0    60   ~ 0
Pin 2\n
Text Notes 4250 2000 0    60   ~ 0
Pin 4\n
Text Notes 4750 4100 0    60   ~ 0
PINS 10-12 - Hall Effect
Text Notes 1000 1800 0    60   ~ 0
LED strands are three pins:\n- +5V (to LEDs)\n- GND (unused)\n- Return (to MOSFET_N)
Text Notes 1050 2450 0    60   ~ 0
Hall effect strands are three pins:\n\n- +3.3V\n- GND\n- Sensor output
Text Notes 1100 3100 0    60   ~ 0
3 LED strands\n4 Hall effect strands\n= 7 strands = 21 pin connector required
Text Notes 1050 3850 0    60   ~ 0
Max LED stand mA with 33ohm resistor = 50mA\nWith 16ohm resistor = 100mA\nNative LED strand runs at 220mA\nOur mosfets can handle up to 400mA continuous.
Text Notes 750  6600 0    60   ~ 0
Total power budget:\n800mA @ 5V when running off 3.6V LiPos\n1250mA @ 5V running off a 5V source.\n1600mA @ 5V running off a 6V source.
Text Notes 5100 3500 0    60   ~ 0
PIN 7 - Hall Effect\n
Text Notes 950  5750 0    60   ~ 0
All odd-numbered pins are ground.\nStart counting from PIN 1.\n\n2 - VIN\n4 - Lights 1 control\n6 - +5V\n8 - Lights 2 control\n10 - +5V\n12 - Lights 3 control\n14 - +5V\n16 - Sensor 1\n18 - +3.3V\n20 - Sensor 2\n22 - +3.3V\n24 - Sensor 3\n26 - +3.3V\n\nNB: LED strands go from +5V to control. They do *not* use GND pins.
$Comp
L R R?
U 1 1 5733DB26
P 5200 2850
F 0 "R?" V 5280 2850 50  0000 C CNN
F 1 "33R" V 5200 2850 50  0000 C CNN
F 2 "" H 5200 2850 60  0000 C CNN
F 3 "" H 5200 2850 60  0000 C CNN
	1    5200 2850
	0    1    1    0   
$EndComp
Text Notes 4350 1550 0    60   ~ 0
MOSFET gate R is 100R on prototype board, 33R on further boards.\nMostly because I ran out of 100R resistors and it's only there to stop ringing.\n
Text Notes 3750 5100 0    60   ~ 0
Hall effect controls have 33k internal pull-up resistors to prevent line floating.
Text Label 9550 4850 0    60   ~ 0
+3.3V
Text Notes 7250 5750 0    60   ~ 0
On Gen 3 boards, pins 20-23 are connected to dip switches, floating off, GND on.\n\nPIN 20 - DIP 4\nPIN 21 - DIP 3\nPIN 22 - DIP 2\nPIN 23 - DIP 1
Text Notes 3950 6650 0    60   ~ 0
Switch ideas:\n- Performer / Dancer\n- Photography mode (sensors up/down/rotate)\n- Sonar select\n- ??
$Comp
L SPST SW?
U 1 1 5768C1BE
P 10350 2150
F 0 "SW?" H 10350 2250 50  0000 C CNN
F 1 "DIP2" H 10350 2050 50  0000 C CNN
F 2 "" H 10350 2150 50  0000 C CNN
F 3 "" H 10350 2150 50  0000 C CNN
	1    10350 2150
	0    -1   -1   0   
$EndComp
$Comp
L SPST SW?
U 1 1 5768C273
P 10650 2150
F 0 "SW?" H 10650 2250 50  0000 C CNN
F 1 "DIP3" H 10650 2050 50  0000 C CNN
F 2 "" H 10650 2150 50  0000 C CNN
F 3 "" H 10650 2150 50  0000 C CNN
	1    10650 2150
	0    -1   -1   0   
$EndComp
$Comp
L SPST SW?
U 1 1 5768C29E
P 10950 2150
F 0 "SW?" H 10950 2250 50  0000 C CNN
F 1 "DIP4" H 10950 2050 50  0000 C CNN
F 2 "" H 10950 2150 50  0000 C CNN
F 3 "" H 10950 2150 50  0000 C CNN
	1    10950 2150
	0    -1   -1   0   
$EndComp
$Comp
L SPST SW?
U 1 1 5768C177
P 10050 2150
F 0 "SW?" H 10050 2250 50  0000 C CNN
F 1 "DIP1" H 10050 2050 50  0000 C CNN
F 2 "" H 10050 2150 50  0000 C CNN
F 3 "" H 10050 2150 50  0000 C CNN
	1    10050 2150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9700 1150 9700 2250
Wire Wire Line
	9700 2250 8100 2250
Wire Wire Line
	9700 1050 10100 1050
Wire Wire Line
	9800 2400 9800 1050
Wire Wire Line
	8100 2400 9800 2400
Connection ~ 9800 1050
Connection ~ 9550 2400
Wire Wire Line
	9550 2400 9550 3850
Wire Wire Line
	8100 2550 8100 1800
Wire Wire Line
	6100 2250 6100 1750
Wire Wire Line
	10050 1150 10050 1050
Connection ~ 10050 1050
Wire Wire Line
	9700 950  10100 950 
Wire Wire Line
	10100 950  10100 800 
Wire Wire Line
	9950 850  9950 950 
Connection ~ 9950 950 
Wire Wire Line
	9700 1650 9350 1650
Connection ~ 9700 1650
Wire Wire Line
	4100 1550 4100 1200
Wire Wire Line
	4100 2650 4100 2450
Wire Wire Line
	4100 3050 4100 3400
Wire Wire Line
	4400 2850 4950 2850
Wire Wire Line
	5450 2850 6100 2850
Wire Wire Line
	9550 4250 9550 4850
Wire Wire Line
	8100 3150 10950 3150
Wire Wire Line
	10950 3150 10950 2650
Wire Wire Line
	8100 3000 10650 3000
Wire Wire Line
	10650 3000 10650 2650
Wire Wire Line
	8100 2850 10350 2850
Wire Wire Line
	10350 2850 10350 2650
Wire Wire Line
	8100 2700 10050 2700
Wire Wire Line
	10050 2700 10050 2650
Wire Wire Line
	9800 1650 10950 1650
Connection ~ 10350 1650
Connection ~ 10650 1650
Connection ~ 10050 1650
Connection ~ 9800 1650
Wire Wire Line
	9550 3950 8900 3950
Wire Wire Line
	8900 3950 8900 3300
Wire Wire Line
	8900 3300 8100 3300
Wire Wire Line
	9550 4050 8750 4050
Wire Wire Line
	8750 4050 8750 3450
Wire Wire Line
	8750 3450 8100 3450
Wire Wire Line
	9550 4150 8600 4150
Wire Wire Line
	8600 4150 8600 3600
Wire Wire Line
	8600 3600 8100 3600
Text Notes 7950 6250 0    60   ~ 0
DIPless boards:\n- Big Board: PIN 0 is shorted to GND\n- Little Baord: PIN 1 is shorted to GND\n
$EndSCHEMATC
