This python GUI requires python 2.7

-------------------------------------------------------------
Dependencies
-------------------------------------------------------------

The following packages should be installed:

Fedora:
    dnf install python2-opencv

Ubuntu:
    apt-get install python-opencv


wxPython is available at:
    https://extras.wxpython.org/wxPython4/extras/linux/gtk3/

Download the proper python wheel and then:
    pip install wxPython-4.0.*.whl

other dependencies:
    pip install pyopengl pyopengl-accelerate

-------------------------------------------------------------
Instructions
-------------------------------------------------------------

Run the script with:
    python vTS_GUI.py 

For Ubuntu users, please press Alt+C in order to make the menu appear.

Configure desired options:
    Connection
        Calib - choose calibration file for stereo
        Algorithm - choose vTrack and/or Stereo; by default both algorithms are selected
        Resolution - choose VGA or 720p for ma2x8x; choose VGA for ma2x5x stereo

Start the streaming with:
    Connection -> Connect
