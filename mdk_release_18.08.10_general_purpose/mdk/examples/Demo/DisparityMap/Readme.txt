DisparityMap

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
Compute the Census Disparity Map from two stereo images.

Software description
=======================
The algorithm consists in the following steps:
	0. 	(LOS) 		Initialize USB and start LRT
    1. 	(LRT) 		Initialize board and imx208 sensors. Resolution used is VGA 640x480.
	2. 	(SHAVE 0) 	Preprocessing step: compute the inverse of Homography and (LRT) compute rectified coordinates for right image
	3. 	(LRT) 		Initialize two SIPP pipelines, one for input conversion and the other for median filtering
	4. 	(LRT) 		Capture RAW10 images from the cameras and (SHAVE 0) convert them to YUV400 for further processing
	5. 	(HW block) 	Rectify right image
	6. 	(LRT) 		Split images into 4 patches
	7. 	(SHAVES 0-3)	Start 4 SHAVEs in order to process the 4 patches in parallel. The following filters are applied:
			- Census Transform 5x5 on left image -> the result is a binary number to encode local spatial structure
			- Census Transform 5x5 on right image -> the result is a binary number to encode local spatial structure
			- Census Matching of left and right images -> matches one pixel from the left image with 64 pixels from the right image
			- Census Minimum -> chooses the closest match out of the 64 presumed matches (Winner Takes All)
	8. 	(LRT) 		Reconstruct image from patches
	9. 	(HW block) 	Apply median filtering on disparity map
	10. (SHAVE 0) 	Postprocessing step: convert resulted disparity map from YUV400 to YUV422
	11. (LOS) 		Stream disparity map on USB

Hardware needed
=====================
A MV182 or MV212 board, housing the Myriad2 chip.
A MV200 board with IMX208 sensors connected to the CAMB connector of the MV182 or MV212 board.
A USB cable connection from MV182 to a PC.

Build
=====================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Build configurations:
PROFILING	- Run algorithm on static images and profiling on Leon and Shave: instruction and stalls
CAMERA		- Camera streaming and USB streaming


Usage:
(ma2X50 can be ma2150)
a) make all MV_SOC_REV=ma2X50 RUN_MODE=PROFILING // will run on static images and show profiling information and unit test result
b) make all MV_SOC_REV=ma2X50 RUN_MODE=CAMERA	// will run in Demo mode and results are streamed via USB
c) make all MV_SOC_REV=ma2X50 // will run in CAMERA mode by default

Setup
====================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"


Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
====================
a) Profiling information and the message "PIPE:LRT: moviUnitTest:PASSED"
b) The displayed output can be seen with a player that can output the USB streaming (i.e. AmCap, VLC, Skype, Cheese)

PC project
===================
The PC project uses as input two Left and Right static images from the Middlebury dataset and computes the disparity map (steps 6-9 from above).

Build & run
    - cd pc/linux/
    - make all
    - ./DisparityMap

Expected output of the PC project is:
    "PC unit test: PASSED
    Algorithm finished successfully!"

The resulted disparity map is located in pc/linux/disparityMapMedian.yuv and can be opened with IrfanViewer (dimensions 640 x 480).



