----------------
RobotVision  Myriad application 
----------------
This application is the myriad part of the robotVision eval Kit. It runs vTrack and Stereo on the Myriad. The output of the application is controlled from the host side through the visionHAL

IMPORTANT NOTES:

This application can be executed on ma2450 or ma2480 architectures only.
This application it's running only on Ubuntu 16.04 LTS.

Setup:
1. To use this application you need the following:
  - MV0212 or MV0235 development board with stereo cameras (Mv0250)
  - USB cable connected to PC (using USB3 cable and port is preferable for greater speeds)
2. In order to have permissions for the usb device (without sudo), you need to set up the udev rules for the device (note that you need to do this only once on a computer):
  cd ../utils && sh installRule.sh
-------------------------------
Option 1 : JTAG Run
-------------------------------
Steps to run:
  1. run make start_server with the required MV_SOC_REV
     eg. make start_server MV_SOC_REV=ma2480 
  2. run make debug with the required MV_SOC_REV
     eg. make debug MV_SOC_REV=ma2480 
-------------------------------
Option 2 : USB Boot
-------------------------------
Steps to run:
  1. run make all with the required MV_SOC_REV
     eg. make all MV_SOC_REV=ma2480 
  2. power up the development board
  3. Boot the myriad mvcmd and start the PC app

NB. for USB boot, set switch 7 Up, all the rest Down

Software description
------------------------

The myriad application has the following features:
    1. Set up an USB connection with PC
    2. Receive general application configurations
    3. Receive a list of algorithms (i.e. vTrack, Stereo) and their configuration
    4. Set up stereo camera pair and receive images
    5. Configure and get the data from the IMU (400Hz Gyro and 500Hz Accel)
    6. Estimate gyro bias using the first 100 samples
    7. Generate a rotation matrix by integrating the gyro samples
    8. Feed the rotation matrix to vTrack to predict the feature locations (gyroAssist)
    9. Run vTrack on each frame
    10. Run Stereo on each stereo pair
    11. Run statistic algorithms on images for AEC
