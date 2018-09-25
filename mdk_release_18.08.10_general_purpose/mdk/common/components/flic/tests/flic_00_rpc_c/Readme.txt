Roles:
 PlgA : data generator
 PlgB : buffer (propagates input on output)
 PlgC : sink
 Stub/Skel : RMI helper plugins

Example diagram for N = 2               |
     _      ____   _____                |         _      _____ _______
    | |    / __ \ / ____|               |        | |    |  __ \__   __|
    | |   | |  | | (___                 |        | |    | |__) | | |
    | |   | |  | |\___ \                |        | |    |  _  /  | |
    | |___| |__| |____) |               |        | |____| | \ \  | |
    |______\____/|_____/                |        |______|_|  \_\ |_|
                                        |
                                        |
                                        |
#############          =============    |    =============
#  PlgA     # >>>>>>>> | osStub[0] | >>>>>>> | rtSkel[0] | >>>>>>>\
#############          =============    |    =============         #############
                                        |                          #  PlgB[0]  #
                       =============    |    =============         #############
             /<<<<<<<< | osSkel[0] | <<<<<<< | rtStub[0] | <<<<<<</
#############          =============    |    =============
#  PlgB[0]  #                           |
#############          =============    |    =============
             \>>>>>>>> | osStub[1] | >>>>>>> | rtSkel[1] | >>>>>>>\
                       =============    |    =============         #############
                                        |                          #  PlgB[1]  #
#############          =============    |    =============         #############
#  PlgC     # <<<<<<<< | osSkel[1] | <<<<<<< | rtStub[1] | <<<<<<</
#############          =============    |    =============

Hardware needed
==================
This software should run on Myriad X and Myriad 2 boards.
Not tested on moviSim.

Build
=====
This project was migrated to the Next Generation build system. You may still use the legacy build system, but the Next Generation build system is now the recommended way to build and maintain your project. Please see the sections below for the build steps corresponding to your choice.

Build using the Next Generation build system
============================================
The Next Generation build system main makefile is named `newMakefile.mk`. Please use the `-f newMakefile.mk` upon make invocation so that you kick-in the Next Generation build system for all the targets you may need and not mentioned in this section.
Alternatively, please consider using a SHELL alias in order to reduce typos.

Please type "make -f newMakefile.mk help" if you want to learn available targets.

Prepare the Next Generation build system:

	$ make -f newMakefile.mk prepare-kconfig

In case of errors, please refer to the log file mentioned on that command's output. Usually, errors occur when some pre-requisites are not installed.

To inspect the settings and eventually adjust them:

	$ make -f newMakefile.mk [menuconfig|nconfig|qconfig]

To build the application:

	$ make -f newMakefile.mk -j

To run or debug it, open two terminals.
	In the first terminal launch the debug server:

		$ make -f newMakefile.mk start_server

	In the second terminal, invoke the run or the debug target:

		$ make -f newMakefile.mk [run|debug]

To clean the build artefacts:

	$ make -f newMakefile.mk clean

To completely remove the build artefacts and directories:

	$ rm -rf mvbuild

Build using the legacy build system
===================================

Please type "make help" if you want to learn available targets.
Before cross-compiling make sure you do a "make clean"!

To build the project open one terminal and please type:
	$ make clean
	$ make all MV_SOC_REV={Myriad_version}
where {Myriad_version} may be ma2480, ma2150 or ma2450

To run or debug the project, open two terminals:
	In the first terminal launch the debug server:
    make start_server
  In the second terminal, invoke the run or the debug target:
    make run MV_SOC_REV={Myriad_version}
where {Myriad_version} may be ma2480, ma2150 or ma2450
