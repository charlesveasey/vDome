#pragma once
#include "ofMain.h"
#include "ofxMultiGLFWWindow.h"

#ifdef TARGET_OSX
	#include "ofxCocoaWindowUtils.h"
#endif
namespace vd {

class Window {

public:
    Window();
    void setup();
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);

    ofPoint getPosition();
    void setPosition(int x, int y);

    ofPoint getResolution();
    void setResolution(int w, int h);
    
    int firstProjector;
    int lastProjector;
    GLFWwindow* glfwWindow;
    
private:
    int     x;
    int     y;
    int     width;
    int     height;
	bool    fullscreen;
    bool    border;
    bool    top;
   
    ofxMultiGLFWWindow *glfw;
    
    #ifdef TARGET_OSX
        ofxCocoaWindowUtils	cocoaWindowUtils;
    #endif    
};

}
