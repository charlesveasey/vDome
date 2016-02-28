#include "ofMain.h"
#include "vdome.h"

#define VDOME_DEBUG // define for debug console

int main( ){
    
#ifdef VDOME_DEBUG
    ofSetLogLevel(OF_LOG_VERBOSE);
#else
    ofSetLogLevel(OF_LOG_SILENT);
#endif
   
// hide console window
#ifdef TARGET_WIN32
#ifndef VDOME_DEBUG 
    HWND handleWindow;
    AllocConsole();
    handleWindow = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(handleWindow, 0);
#endif
#endif
    
	// start vdome
    vd::vdome vdome;
    vdome.setup();
}
