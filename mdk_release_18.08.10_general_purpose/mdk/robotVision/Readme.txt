------------------------
robotVision Readme
------------------------

This contains Myriad application and PC visualisations for:
1. vTrack evaluation
2. Stereo depth evaluation
3. Saving camera images

Before building the applicationn, ensure that libusb and opencv 3 are installed on your system. 

Folder structure is as follows:
|---> MyriadApp  --  The application to run on the Myriad
|---> PcViz -- Applications that send the required config to the Myriad and display visualisations
|---> PcWxPython -- (recommended) GUI Application that sends config to the Myriad and displays visualisations
|---> HAL -- source code used by PcViz app
|---> utils -- various scripts and utilities

To evaluate vTrack and Stereo using the python GUI, 
1. Run the Myriad application (in MyriadApp)
2. Run the python GUI application (in PcWxPython)
3. Select the configuration required using the 'Connection' menu in the python GUI
4. Click Connection -> Connect
