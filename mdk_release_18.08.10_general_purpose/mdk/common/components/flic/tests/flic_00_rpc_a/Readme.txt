Inter-Leon barrier test.
Test contains 4 barriers which are waited for by both Leons.
First 2 barriers are delayed by LRT, the last 2 by LOS.

Hardware needed
==================
This software should run on Myriad X and Myriad 2 boards.
Not tested on moviSim.

Bash command to run test multiple times:
for i in {1..100}; do echo "######## run" $i; make run; done

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
