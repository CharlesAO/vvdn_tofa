MasterSlave

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
The application uses the shaveFifoComm mdk component for the master-slave communication 
It will print the execution time and check if the read was successful or not.
Shave 0 is the master and SHAVEs 1, 2 ... 11 are slaves.
The master shave adds tasks to the fifo and waits for the slaves to execute the tasks.
In this example we only use one fifo and one type of task.

Software description
=======================

This application uses both Leon and Shave code. Basically, these are the few steps made during the application:
    1. Initialize master, slaves and timer.
    2. Open master and shaves
    3. Start Timer
    4. Start Master
    5. Master will call n times the number of tasks on the slaves
    6. After master finishes we can stop the timer and print the execution time
    7. Check if the SHAVEs didn't try to read the same task at the same time 
    8. Print Passed/Failed depending on 7
    9. Close shaves
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
 Elapsed time:58506450
 Test passed
 


