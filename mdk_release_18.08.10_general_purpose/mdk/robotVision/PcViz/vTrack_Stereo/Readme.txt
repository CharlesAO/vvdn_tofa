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
./vTrackStereo --device [myriad] --alg [vtrack|stereo] [OPTION1]...
Options:
  --vis               [basic|detailed]
  --res               [VGA|720P]
  --of                [movi|mest]
  -h                  [path to stereo calibration file]

Note: The --res argument is mandatory for running stereo on ma2x5x, as the supported resolution is VGA in this case:
./vTrackStereo --device [myriad] --res VGA --alg [vtrack|stereo] [OPTION1]...

Software description
------------------------
vTrackStereo is an example PC application to use the visionHal
The application has the following features:
    1. Initialize visionHal (set up communication with myriad application)
    2. Receive images from the camera @30Hz
    3. Configure and get the data from the IMU (400Hz Gyro and 500Hz Accel)
    4. Configure vTrack algorithm and receive data
    5. Configure Stereo algorithm and receive data
    6. Visualize vTrack data
    7. Visualize Stereo data 


vTrackStereo takes the images from myriad and displays:
1) VGA image with all features. Feature tracks are visualized with lines. The Features that are considered
as inliers are drawn in green and the outliers are drawn in red.
The lines will show a maximum of 200 (configurable) previous locations of the feature.
The FPS is also shown in the upper left corner of the image.
2) Histograms over the last 500 (configurable) frames:
     - Number of feature histogram: shows a histogram of the feature count over the last 500 frames
     - Tracking length histogram: shows a histogram of the tracking lengths.
     The track length is considered to be the value of feature age in the last frame where it was found.
     Note that this histogram will show pretty short track lengths with a still camera.
     Most of the features will not be lost, so they will not be shown on this histogram
     - Coverage histogram: shows a histogram of the coverage values over the last 500 frames
     - inlier percentage: showing the percentage of inliers for the last 500 frames. The inliers are calculated using opencv.
3) Histogram on the current frame:
     - current age histogram: a histogram of the current feature ages
4) Live graphs: OX axis progresses together with the frames
     - current coverage
     - myriad latency: consists of two functions
                          - total myriad latency (camera readout + conversion + vTrack + system latency)
                          - vTrack processing time
     - feature count

Possible configurations:
- The histograms can be independently disabled using the defines in featuresDrawer.cpp or with the --vis option
- The number of last frames used in the histograms can be configured with NR_FRAMES define in featuresDrawer.h




  