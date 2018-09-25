simpleRTEMS_tempSensor

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The example initializes the Temperature Sensor Driver in Posix_Init in the following order:
- CSS Sensor in Manual(One-Shot) mode;
- MSS Sensor in Manual(One-Shot) mode;
- UPA0 Sensor in Continuous Mode for temperature samples;
- UPA1 Sensor in Continuous Mode for voltage samples;
The execution is run by 5 threads: first 4 threads read samples for their respective sensor
and print the read values, the last thread installs a callback on a temperature threshold interrupt request(UPA0 sensor).

Hardware needed
==================
Myriad2 - This software should run on MV0182 or MV0212 boards.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal

Expected output
==================
UART:
UART: ---- Temperature Sensor App Init ----
UART: Start thread b010002
UART: /dev/tempsensor0: 0.876943 Volts
UART: Start thread b010003
UART: Start thread b010004
UART: Start thread b010005
UART: Start thread b010006
UART: /dev/tempsensor1: 43.226711 degrees Celsius
UART: IRQ Triggered Sensor: 2, context: b010004, sample: 44.675610 degrees Cel
UART: sius
UART: /dev/tempsensor2: 44.469261 degrees Celsius
UART: /dev/tempsensor3: 0.880695 Volts
UART: Finished thread id b010006
UART: /dev/tempsensor0: 0.876943 Volts
UART: /dev/tempsensor1: 43.226711 degrees Celsius
UART: /dev/tempsensor0: 0.876943 Volts
UART: /dev/tempsensor1: 46.114143 degrees Celsius
UART: /dev/tempsensor2: 46.523270 degrees Celsius
UART: /dev/tempsensor3: 0.880695 Volts
UART: /dev/tempsensor0: 0.876407 Volts
UART: /dev/tempsensor1: 45.498890 degrees Celsius
UART: /dev/tempsensor0: 0.876943 Volts
UART: /dev/tempsensor1: 45.909271 degrees Celsius
UART: /dev/tempsensor2: 46.318810 degrees Celsius
UART: /dev/tempsensor3: 0.881231 Volts
UART: /dev/tempsensor0: 0.876407 Volts
UART: /dev/tempsensor1: 46.523270 degrees Celsius
UART: /dev/tempsensor0: 0.876943 Volts
UART: /dev/tempsensor1: 46.523270 degrees Celsius
UART: /dev/tempsensor2: 46.523270 degrees Celsius
UART: /dev/tempsensor3: 0.880159 Volts
UART: /dev/tempsensor0: 0.876407 Volts
UART: /dev/tempsensor1: 46.318810 degrees Celsius
UART: /dev/tempsensor0: 0.876943 Volts
UART: /dev/tempsensor1: 45.498890 degrees Celsius
UART: /dev/tempsensor2: 46.523270 degrees Celsius
UART: /dev/tempsensor3: 0.881231 Volts
UART: /dev/tempsensor0: 0.876407 Volts
UART: /dev/tempsensor1: 46.114143 degrees Celsius
UART: Finished thread id b010002
UART: Finished thread id b010003
UART: /dev/tempsensor2: 46.523270 degrees Celsius
UART: /dev/tempsensor3: 0.881231 Volts
UART: /dev/tempsensor2: 47.339016 degrees Celsius
UART: /dev/tempsensor3: 0.881767 Volts
UART: /dev/tempsensor2: 46.523270 degrees Celsius
UART: /dev/tempsensor3: 0.881231 Volts
UART: /dev/tempsensor2: 46.114143 degrees Celsius
UART: /dev/tempsensor3: 0.881231 Volts
UART: /dev/tempsensor2: 46.318810 degrees Celsius
UART: /dev/tempsensor3: 0.881231 Volts
UART: Finished thread id b010004
UART: Finished thread id b010005
LOS: Application terminated successfully.

User interaction
==================
