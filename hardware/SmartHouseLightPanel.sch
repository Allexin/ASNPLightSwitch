EESchema Schematic File Version 2
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
LIBS:SmartHouseComponents
LIBS:SmartHouseLightPanel-cache
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
L ArduinoProMini ArduinoProMini1
U 1 1 59DE2210
P 3250 3400
F 0 "ArduinoProMini1" H 3250 3400 60  0000 C CNN
F 1 "ArduinoProMini" H 3250 3400 60  0000 C CNN
F 2 "SmartHouseLib:ArduinoProMini" H 3250 3400 60  0001 C CNN
F 3 "" H 3250 3400 60  0001 C CNN
	1    3250 3400
	1    0    0    -1  
$EndComp
$Comp
L USBASP_5x2 USBASP1
U 1 1 59DE225D
P 5200 4200
F 0 "USBASP1" H 5200 4200 60  0000 C CNN
F 1 "USBASP_5x2" H 5150 4400 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x05_Pitch2.54mm" H 5200 4200 60  0001 C CNN
F 3 "" H 5200 4200 60  0001 C CNN
	1    5200 4200
	1    0    0    -1  
$EndComp
$Comp
L ROTARY_ENCODER_SWITCH EC12D1
U 1 1 59DE2304
P 1450 3400
F 0 "EC12D1" H 1450 3660 50  0000 C CNN
F 1 "ROTARY_ENCODER_SWITCH" H 1450 3140 50  0000 C CNN
F 2 "SmartHouseLib:ENCODER_EC12D" H 1350 3560 50  0001 C CNN
F 3 "" H 1450 3660 50  0001 C CNN
	1    1450 3400
	-1   0    0    1   
$EndComp
$Comp
L R R3
U 1 1 59DE291D
P 2050 3000
F 0 "R3" V 2130 3000 50  0000 C CNN
F 1 "R10k" V 2050 3000 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 1980 3000 50  0000 C CNN
F 3 "" H 2050 3000 50  0000 C CNN
	1    2050 3000
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 59DE2960
P 1000 3100
F 0 "R1" V 1080 3100 50  0000 C CNN
F 1 "R5k1" V 1000 3100 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 930 3100 50  0000 C CNN
F 3 "" H 1000 3100 50  0000 C CNN
	1    1000 3100
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 59DE2B2A
P 1950 3800
F 0 "R2" V 2030 3800 50  0000 C CNN
F 1 "R5k1" V 1950 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 1880 3800 50  0000 C CNN
F 3 "" H 1950 3800 50  0000 C CNN
	1    1950 3800
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 59DE2B5F
P 2100 3800
F 0 "R4" V 2180 3800 50  0000 C CNN
F 1 "R5k1" V 2100 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2030 3800 50  0000 C CNN
F 3 "" H 2100 3800 50  0000 C CNN
	1    2100 3800
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 serial1
U 1 1 59DE33B0
P 3150 1450
F 0 "serial1" H 3150 1600 50  0000 C CNN
F 1 "CONN_01X02" V 3250 1450 50  0000 C CNN
F 2 "SmartHouseLib:CONN_2_3.5mm" H 3150 1450 50  0000 C CNN
F 3 "" H 3150 1450 50  0000 C CNN
	1    3150 1450
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X02 power1
U 1 1 59DE34ED
P 3400 1450
F 0 "power1" H 3400 1600 50  0000 C CNN
F 1 "CONN_01X02" V 3500 1450 50  0000 C CNN
F 2 "SmartHouseLib:CONN_2_3.5mm" H 3400 1450 50  0000 C CNN
F 3 "" H 3400 1450 50  0000 C CNN
	1    3400 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1150 3300 1150 3150
Wire Wire Line
	1150 3150 2400 3150
Wire Wire Line
	2400 3150 2400 3400
Wire Wire Line
	1150 3500 1000 3500
Wire Wire Line
	1000 3500 1000 3250
Wire Wire Line
	1000 1800 1000 2950
Wire Wire Line
	4300 1800 4300 3100
Wire Wire Line
	1900 3000 1900 3150
Connection ~ 1900 3150
Wire Wire Line
	2300 3000 2200 3000
Wire Wire Line
	2300 1900 2300 3000
Wire Wire Line
	1750 3400 1800 3400
Wire Wire Line
	1800 3400 1800 2850
Wire Wire Line
	1800 2850 2300 2850
Connection ~ 2300 2850
Wire Wire Line
	1950 3650 1950 3300
Connection ~ 1950 3300
Connection ~ 2100 3500
Wire Wire Line
	800  3950 2100 3950
Wire Wire Line
	800  3950 800  2850
Wire Wire Line
	800  2850 1000 2850
Connection ~ 1000 2850
Connection ~ 1950 3950
Wire Wire Line
	4050 2900 5350 2900
Wire Wire Line
	4150 2900 4150 1650
Wire Wire Line
	4150 1650 3450 1650
Wire Wire Line
	2300 1900 3450 1900
Wire Wire Line
	3450 1900 3450 1650
Wire Wire Line
	1000 1800 4300 1800
Wire Wire Line
	3350 1800 3350 1650
Connection ~ 3350 1800
Wire Wire Line
	3100 1650 3100 2350
Wire Wire Line
	3200 2350 3200 1650
Wire Wire Line
	4950 3100 4950 3400
Wire Wire Line
	5350 2900 5350 3400
Connection ~ 4150 2900
Wire Wire Line
	4950 4500 4400 4500
Wire Wire Line
	4400 4500 4400 3800
Wire Wire Line
	4400 3800 4050 3800
Wire Wire Line
	4050 3700 4300 3700
Wire Wire Line
	4300 3700 4300 4750
Wire Wire Line
	4300 4750 5350 4750
Wire Wire Line
	5350 4750 5350 4500
Wire Wire Line
	5250 4500 5250 4600
Wire Wire Line
	5250 4600 4500 4600
Wire Wire Line
	4500 4600 4500 3600
Wire Wire Line
	4500 3600 4050 3600
Wire Wire Line
	5150 4500 5150 4900
Wire Wire Line
	5150 4900 2350 4900
Wire Wire Line
	2350 3000 2500 3000
Wire Wire Line
	2400 3400 2500 3400
Wire Wire Line
	4300 3100 4950 3100
Wire Wire Line
	3300 2350 3300 1800
Connection ~ 3300 1800
Wire Wire Line
	1750 3500 2500 3500
Wire Wire Line
	1750 3300 2500 3300
Wire Wire Line
	2100 3650 2100 3500
Wire Wire Line
	2350 4900 2350 3000
$EndSCHEMATC
