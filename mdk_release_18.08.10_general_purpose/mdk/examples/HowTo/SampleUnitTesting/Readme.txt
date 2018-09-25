Sample Project for Creating Unit Tests

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
This is a simple project that tests a Myriad2 asm file against a C code.

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 boards.


Build
==================
Please type "make help" if you want to learn available targets.

Myriad2 {Myriad_version} simulator - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"
Myriad2 {Myriad_version} silicon - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version} DBGI=jtag"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Myriad2 simulator - To run the application:
    - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run_test MV_SOC_REV={Myriad_version}"
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run_test MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Documentation
==================
For further information about the unittest framework please see the ”MDK Kernel Unit Testing Guide”

Expected output
==================
[==========] Running 11 tests from 2 test cases.
[----------] Global test environment set-up.
[----------] 4 tests from SampleAppTest
[ RUN      ] SampleAppTest.RandomInputAddition
L: Application terminated successfully.
[       OK ] SampleAppTest.RandomInputAddition (2570 ms)
[ RUN      ] SampleAppTest.RandomInputSubtraction
L: Application terminated successfully.
[       OK ] SampleAppTest.RandomInputSubtraction (906 ms)
[ RUN      ] SampleAppTest.UniformInputAddition
L: Application terminated successfully.
[       OK ] SampleAppTest.UniformInputAddition (1025 ms)
[ RUN      ] SampleAppTest.UniformInputSubtraction
L: Application terminated successfully.
[       OK ] SampleAppTest.UniformInputSubtraction (953 ms)
[----------] 4 tests from SampleAppTest (5454 ms total)

[----------] 7 tests from RandomInputs/SampleAppTest
[ RUN      ] RandomInputs/SampleAppTest.TestWidthRandomData/0
L: Application terminated successfully.
[       OK ] RandomInputs/SampleAppTest.TestWidthRandomData/0 (976 ms)
[ RUN      ] RandomInputs/SampleAppTest.TestWidthRandomData/1
L: Application terminated successfully.
[       OK ] RandomInputs/SampleAppTest.TestWidthRandomData/1 (875 ms)
[ RUN      ] RandomInputs/SampleAppTest.TestWidthRandomData/2
L: Application terminated successfully.
[       OK ] RandomInputs/SampleAppTest.TestWidthRandomData/2 (954 ms)
[ RUN      ] RandomInputs/SampleAppTest.TestWidthRandomData/3
L: Application terminated successfully.
[       OK ] RandomInputs/SampleAppTest.TestWidthRandomData/3 (1009 ms)
[ RUN      ] RandomInputs/SampleAppTest.TestWidthRandomData/4
L: Application terminated successfully.
[       OK ] RandomInputs/SampleAppTest.TestWidthRandomData/4 (1259 ms)
[ RUN      ] RandomInputs/SampleAppTest.TestWidthRandomData/5
L: Application terminated successfully.
[       OK ] RandomInputs/SampleAppTest.TestWidthRandomData/5 (1290 ms)
[ RUN      ] RandomInputs/SampleAppTest.TestWidthRandomData/6
L: Application terminated successfully.
[       OK ] RandomInputs/SampleAppTest.TestWidthRandomData/6 (1765 ms)
[----------] 7 tests from RandomInputs/SampleAppTest (8144 ms total)

[----------] Global test environment tear-down
[==========] 11 tests from 2 test cases ran. (13606 ms total)
[  PASSED  ] 11 tests.
Average measured cycles per pixel: 2.641373

