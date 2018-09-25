SharingExample

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Just print hello world using shave compiled code using stdio library
-also makes sharing of variables between shaves
-on shave 4 uses swcStartShaveCC function to start shave and start the execution

Software description
=======================

This application uses both the Leon and the Shave code.
Basically, these are the few steps made during the application:
    1. The Clock Power reset module is initialized.
    Four cases are being discussed:

    A. Simple Leon - Shave0 Sharing using global data
        1. Set the input SHAVEs parameters.
        2. Run and wait for shave 0 to complete.
        3. Check the result.
        This example uses the diff_sharing shave module. In this case the simple add_simd function is being used in order to compute the data shared between leon and shave0.
        The rule for building the diff_sharing module is described in diff_sharing_rules.mk.
        This simple rule specifies which objects build up the said application. The application will be built into a library.


    B. Simple Leon - Shave1 Sharing using global data
        1. Set the input SHAVEs parameters.
        2. Run and wait for shave 1 to complete.
        3. Check the result.
        This example uses the diff_sharing shave module. In this case the simple add_simd function is being used in order to compute the data shared between leon and shave1.
        The rule for building the diff_sharing module is described in diff_sharing_rules.mk.
        This simple rule specifies which objects build up the said application. The application will be built into a library.

    C. Share data between SHAVEs 2 and 3
        1. Initialize the two arrays that will be shared between shave2 and shave3
        2. Run and wait for shave 2 to complete.
        3. Check the result.
        4. Run and wait for shave 3 to complete.
        5. Check the result.
        This example uses the global_sharing shave module. In this case the simple add_simd function is being used in order to compute the data shared between shave2 and shave2.
        The rule for building the global_sharing module is described in global_sharing_rules.mk.
        This simple rule specifies which objects build up the said application. The application will be built into a library.
        Beside the previous example, some symbols files (relative path to root of the project) are being used.
        The symuniq file contains a list of pars "old new" symbols. We need as many symuniq files as instances of the application we plan to use.
        In this case they are two: one on shave2 and the other one on shave3. In order to make use of them, they are declared using the extern identifier.

    D. Sharing data between the Leons

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
The results  consist in printfs seen using the debugger:

UART: LeonOS Started.
UART: LeonRT Start Addr: 0x701C1000
UART: Executing sharing test...
UART:
UART:
UART: <<< ExampleA : Simple Leon - Shave0 Sharing using global data  >>>
UART:
UART: Start Shave0 at addr:0x70000010
UART: res  : 8 55 713 513
UART:
UART:
UART: Diff_sharing Executed Successfully on Shave0
UART:
UART:
UART: <<< ExampleB : Simple Leon - Shave1 Sharing using global data  >>>
UART:
UART: Start Shave1 at addr:0x70020010
UART: res  : 8 55 713 513
UART:
UART:
UART: Diff_sharing Executed Successfully on Shave 1
UART:
UART:
UART: <<< ExampleC : Share data between SHAVEs 2 and 3  >>>
UART:
UART: Start Shave2 at addr:0x70040010
UART: res  : 8 55 713 513
UART:
UART:
UART: Global_sharing Executed Successfully on Shave 2
UART:
UART:
UART: <<< ExampleC - Shave 3 part >>>
UART:
UART: Start Shave3 at addr:0x70060010
UART: res  : 8 55 713 513
UART:
UART:
UART: Global_sharing Executed Successfully on Shave 3
UART:
UART:
UART: <<< ExampleD : Sharing data between the Leons  >>>
UART:
UART: Shared data between Leons: 8 55 713 513
UART:
UART: Done.

