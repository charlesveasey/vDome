vDome
=====

Multi-channel projection software designed for domes. Provides real-time warping and slicing for domemaster input.  

Developed by Charles Veasey for the Institute of Indian American Arts (IAIA).  

##Overview

vDome is designed to run as background process and accepts any domemaster formatted input as follows:  
  1.  capture  
  2.  syphon  
  3.  image  
  4.  video  

Capture is the recommended input type:
  - It allows you to put anything on the dome by hooking up another computer to vDome through a capture card.  
  - vDome becomes the 2nd monitor on your production machine. Drag your After Effects or Unity preview window onto the dome and edit in real-time.  
  - Playing videos is done through your favorite media player: Quicktime, VLC, etc.  

Syphon is useful for a single Mac computer system where your production application supports the Syphon technology.

The image and video file input types are currently only used for testing and calibrating. The only way to change the file name is in code.

##Compiling
####All
  -  Clone openFrameworks fork: https://github.com/charlesveasey/openFrameworks  
  -  Clone https://github.com/memo/ofxMSAInteractiveObject and place in openFrameworks/addons folder  
  -  Clone this (vDome) repository and place in the openFrameworks/apps/myApps folder
  
####Mac
  -  Clone to: openFrameworks/addons:  
     - https://github.com/astellato/ofxSyphon  
     - https://github.com/bangnoise/ofxHapPlayer
  - Suggested IDE: Xcode v4.6.3
  - Syphon needs to be copied to Frameworks:
    -  Under target, add a Copy Files Build Phase. Drag the Syphon.framework into this phase.


####Win
  - Suggested IDE: Microsoft Visual Studio Express 2012
  - Video playback utilizes: https://github.com/ofTheo/ofDirectShowVideoPlayer  
    - No need to download seperately, a copy is included in src

####Linux
  - Not yet tested

##Installation
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
  - Calibration is done directly on the dome. This eliminates the need for a control monitor and saves the headache caused by looking back and forth from control monitor to dome.

##Future Development
  - Advanced color correction  
  - Edge-blending  
  - Auto-calibration via camera  
  - Support for Nvidia Warp and Intensity API
  
---
Funding provided by the United States Department of Defence. Based on prior research from the University of New Mexico (UNM) and IAIA with funding provided by the National Science Foundation.

Thanks to research by the ARTS Labs at UNM and Paul Bourke at the University of Western Australia.
