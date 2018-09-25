Optical Flow point tracking test

Overview
==========
The test uses OpenCV to generate a list of keypoints and to track them. The keypoints are uploaded on Myriad to compute the tracking with the MvCV implementation. Finally the tracked points are compared against those in the reference implementation.

Hardware needed
==================
This software should run on MV153 and MV182 boards.

Build
==================
To build the project, please type "make help" and follow the steps suggested there.
Myriad2: "make clean all"

Setup
==================
To use the application start moviDebugServer, then launch with "make run_test".

Expected output
==================
Two images with tracked points.


