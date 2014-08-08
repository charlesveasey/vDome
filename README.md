vDome
=====

Multi-channel projection software designed for domes. Provides real-time warping and slicing for domemaster input.  

Developed by Charles Veasey for the Institute of Indian American Arts (IAIA).  

##Overview

vDome is an application designed to calibrate multiple projectors on a hemispherical dome surface and display a domemaster formatted video, image, or interactive application. vDome is generally used in two ways: 1) as a media player and 2) as a background process that listens to software/hardware input streams.

As a media player vDome utilizes native os media libraries and accepts most common file formats and codecs of the operating system. In general:  
  - Mac OS X: QuickTime / AV Foundation / HAP
  - Win 7/8: DirectShow / Windows Media Foundation / QuickTime
  - Linux: GStreamer

There can be major performance differences between codecs at high resolutions.

Capture and camera inputs are hardware video stream solutions. A capture card is a flexible solution that allows one to run any application on the dome by sending the video output of one computer into another machine running vDome. With this, vDome essentially becomes the 2nd monitor on your production machine. Drag the After Effects or Unity preview window onto the dome and edit in real-time, play videos through your favorite media player: Quicktime, VLC, etc. 

Syphon is useful a on single Mac computer where your production application supports the Syphon technology. There are a number of applications which support the Syphon texture sharing including After Effects, Resolume, MaxMSP, etc. 
http://syphon.v002.info/  

##Compiling
####All
  -  Download the latest version of openFrameworks (currently v0.8.3): http://www.openframeworks.cc/download/
  -  Clone this (vDome) repository to the openFrameworks/apps/myApps folder
  -  Clone to: openFrameworks/addons:
     - https://github.com/charlesveasey/ofxMultiGLFWWindow
     - https://github.com/charlesveasey/ofxM3U
     - https://github.com/charlesveasey/ofxBezierSurface
     - https://github.com/arturoc/ofxPBO
     - https://github.com/bakercp/ofxMediaType

####Mac
  -  Clone to: openFrameworks/addons:
     - https://github.com/charlesveasey/ofxCocoaWindowUtils
     - https://github.com/kronick/ofxAVFVideoPlayer
     - https://github.com/bangnoise/ofxHapPlayer
     - https://github.com/astellato/ofxSyphon

  - Suggested IDE: Xcode v5.1.1
  - Syphon needs to be copied to Frameworks:
    -  Under target, add a Copy Files Build Phase. Drag the Syphon.framework into this phase.

####Win
  -  Clone to: openFrameworks/addons:
     - https://github.com/charlesveasey/ofxWinWindow
     - https://github.com/secondstory/ofxWMFVideoPlayer
     - https://github.com/ofTheo/ofDirectShowVideoPlayer
         
  - Suggested IDE: Microsoft Visual Studio Express 2012
  - ofxWMFPLayer requries updated GLEW library, follow instructions on its repository

####Linux
  - Install openFrameworks dependencies, see INSTALL.md in openFrameworks linux package
  - Suggested IDE: Code::Blocks 10.04 
  - Can also run make in root directory

##Installation
####Mac
  - Install HAP codec:
     - https://github.com/vidvox/hap-qt-codec/

####Win
  - To allow QuickTime video playback install the K-Lite Mega Codec Pack 10.2 
    - http://www.free-codecs.com/download/k_lite_mega_codec_pack.htm

####Linux
  - Install gstreamer codecs, see INSTALL.md in openFrameworks linux package

##Setup and Calibration
Initial setup is done in XML (setting resolution, number of projectors, and input type). Calibration is done directly on the dome. For more information see the manual: 
https://docs.google.com/document/d/1EHPpExjznFF6X0YTY5acLS0MNkEbtVFsLBCoJ2HHQlQ/edit#


##Tested
####Mac
  - 10.8.x
  - 10.9.x

####Win
  - 7.x
  - 8.1.x

####Linux
  - Ubuntu 12.04 
  - Ubuntu 14.04

##Future Development
  - Support for Spout sharing technology on Windows
  - Support for panorama and 16:9 input formats
  - Auto-calibration via camera  
  - Support for Nvidia Warp and Intensity API
  
---
Funding provided by the United States Department of Defence. Based on prior research from the University of New Mexico (UNM) and IAIA with funding provided by the National Science Foundation.

Thanks to research by the ARTS Labs at UNM and Paul Bourke at the University of Western Australia.
