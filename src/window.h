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

    ofVec2f getPosition();
    void setPosition(int x, int y);

    ofVec2f getDimensions();
    void setDimensions(int w, int h);
    
    int firstProjector;
    int lastProjector;
    GLFWwindow* glfwWindow;

    
private:
    ofVec2f position;
    ofVec2f dimensions;
	bool    fullscreen;
    bool    border;
    bool    floatToTop;
    ofxMultiGLFWWindow *glfw;
    #ifdef TARGET_OSX
        ofxCocoaWindowUtils	cocoaWindowUtils;
    #endif    
};

}
