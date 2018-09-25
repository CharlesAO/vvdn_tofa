-------------------------------------------
To build the application
-------------------------------------------
The application can be rebuilt with the following steps:
1. "mkdir build && cd build"
2. "cmake .." 
3. "make"


-------------------------------------------
To Run the application
-------------------------------------------
./dataRecorder --device myriad

Software description
------------------------
dataRecorder is an example PC application to use the visionHal

The application will initialize the myriad application (through the visionHal)
to get frames and IMU data. It will keep the data in RAM until the ESC key is
pressed. After this the gathered data will be checked and saved to the disk.

WARNING: as this application keeps the data in the RAM, monitor your RAM usage
when using this application. Keeping data in the RAM is required to prevent
frame drops while saving data to disk. 
