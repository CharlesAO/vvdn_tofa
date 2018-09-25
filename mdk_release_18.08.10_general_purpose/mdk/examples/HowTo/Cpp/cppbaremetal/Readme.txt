cppbaremetal

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Compiling and running c++ files in Bare Metal environment

Software description
=======================
This application uses Leon OS which is running simple c++ functionality like object creation, method overwriting polymorphism...

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 boards.

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
Myriad2 simulator - To run the application:
    - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The application output should be similar to:
UART: Shape object created at address, 0x701930f0
UART: Geometry object created at address, 0x701930f0
UART:
UART:
UART: Hello from LEON!
UART:
UART:
UART: Creating obj statically...
UART: Shape object created at address, 0x701bfebc
UART: Geometry object created at address, 0x701bfebc
UART: obj somefun returned: 42
UART: Creating dynobj dynamically...
UART: Shape object created at address, 0x701918f8
UART: Geometry object created at address, 0x701918f8
UART: dynobj somefun returned: 42
UART: Accessing inherited method: of dynobj
UART: Dynobj shape type: 6
UART: Testing method overriding
UART: Shape object created at address, 0x70191908
UART: Geometry object created at address, 0x70191908
UART: Shape object created at address, 0x70191918
UART: Geometry object created at address, 0x70191918
UART: Hello from Shape
UART: Hello from Geometry
UART: Printme from global...
UART: Hello from Geometry
UART: Printme from global dynamically...
UART: Shape object created at address, 0x70191928
UART: Geometry object created at address, 0x70191928
UART: Hello from Geometry
UART: Destroying objects
UART: Geometry object: 0x701918f8 destroyed.
UART: Shape object: 0x701918f8 destroyed.
UART: Shape object: 0x70191908 destroyed.
UART: Geometry object: 0x70191918 destroyed.
UART: Shape object: 0x70191918 destroyed.
UART: Destroying Static objects
UART: ....................
UART: Geometry object: 0x701bfebc destroyed.
UART: Shape object: 0x701bfebc destroyed.
UART: Geometry object: 0x701930f0 destroyed.
UART: Shape object: 0x701930f0 destroyed.
