vDome
=====

Multi-channel projection software designed for domes. Provides real-time warping and slicing for domemaster input.  

Developed by Charles Veasey for the Institute of Indian American Arts (IAIA).  

Special thanks for prior technical research by: ARTS Labs at the University of New Mexico and Paul Bourke at the University of Western Australia.

##Overview

vDome is designed to run as background process and accepts any domemaster formatted input as follows:  
  1.  capture  
  2.  syphon  
  3.  image  
  4.  video  

Capture is the recommended input type:
  - It allows you to put anything on the dome by hooking up another computer to vDome through a capture card.  
  - vDome essentially becomes the 2nd monitor on your production machine. For example, drag your After Effects or Unity preview window onto the dome, and edit in real-time.  
  - Playing videos is done through your favorite media player: Quicktime, VLC, etc.  
  - Eliminates middle-ware such as Syphon.

Syphon is useful for a single Mac computer system where your production application supports the Syphon technology.

The image and video file input types are currently only used for testing and calibrating, so the only way to change the file name is in code.

##Compiling
####All
  -  Clone openFrameworks fork: https://github.com/charlesveasey/openFrameworks  
  -  Clone https://github.com/memo/ofxMSAInteractiveObject and place in openFrameworks/addons folder  
  -  Clone this (vDome) repository and place in the openFrameworks/apps/myApps folder
  
####Mac
  -  Clone to: openFrameworks/addons:  
     - https://github.com/astellato/ofxSyphon  
     - https://github.com/bangnoise/ofxHapPlayer

####Win
  - Video playback utilizes: https://github.com/ofTheo/ofDirectShowVideoPlayer
  - No need to download seperately, a copy is included in src

####Linux
  - Not yet tested

##Installing
####Mac
  - Install HAP codec:
     - https://github.com/vidvox/hap-qt-codec/

####Win
  - To allow QuickTime video playback install the K-Lite Mega Codec Pack 10.2 
    - http://www.free-codecs.com/download/k_lite_mega_codec_pack.htm

####Linux
  - Not yet tested

##Setup and Calibration
  - Initial setup is done in XML (setting resolution, number of projectors, and input type).
  - Calibration is done directly on the dome. This eliminates the need for a control monitor and saves the headache caused by looking back and forth from control monitor to dome. There are also some new calibration options (camera perspective shearing) that should simplify calibration compared to the last version (vDome-Max).

##Future Development
  - Advanced color correction  
    - mandatory goal  
    - color channel look up table  
  - Edge-blending  
    - not needed for our dome, but important for others who don't have hardware masking  
  - Auto-calibration via camera  
    - for us, auto-color correction has priority over auto-spatial b/c non-led projectors constantly shift spectrum  
  - Direct support for Nvidia warp and intensity API  
    - stretch goal, we are using AMD Firepro, but have Nvidia cards to test  
    - some Nvidia cards now support warping and blending directly (meaning you can conform the actual windows desktop to fit onto the dome)  
    - vDome would become the calibration tool that creates warp / blend data, but once saved the graphics card would actually do the warp/blend, not vDome
