002_MultipleTaskTypes

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
The application uses the shaveFifoComm mdk component for the master-slave communication 
It will print the execution time and check if the read was successful or not for 2 types of tasks using all fifos.
Shave 0 is the master and SHAVEs 1, 2 ... 11 are slaves.
Slaves 1, 2, 3, 4 and 5 are able to execute task 1
Slave 6 can do both task 1 and 2
Slaves 7, 8, 9, 10 and 11 are able to execute task 2  
The master shave adds tasks to the fifos and waits for the slaves to execute the tasks.
In this example we use 2 fifos and 2 types of task.

Software description
=======================

This application uses both Leon and Shave code. Basically, these are the few steps made during the application:
    1. Initialize master, slaves and timer.
    2. Open master and shaves
    3. Add 2 tasks on the fifos using 3 different combinations of fifos
    4. Assigne tasks to slaves
    5. Start Timer
    6. Start Master
    7. Master will call n times the number of tasks on the slaves
    8. After master finishes we can stop the timer and print the execution time
    9. Check if the SHAVEs didn't read the same task at the same time 
    10. Print Passed/Failed depending on 7
    11. Close shaves
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
The default Myriad revision in this project is ma2150 so it is not necessary
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
The default Myriad revision in this project is ma2150 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
 The result consist in printf seen using the debugger.
 Elapsed time:58825271
 Test passed
 Elapsed time:58828085
 Test passed
 Elapsed time:58827143
 Test passed
 
