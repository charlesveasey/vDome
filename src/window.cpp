#include "window.h"
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Window::Window(){
    x = y = 0;
    width = 1024;
    height = 768;
    fullscreen = false;
    border = false;
    top = false;
    firstProjector = 0;
    lastProjector = 0;
}

/******************************************

 SETUP

 ********************************************/

void Window::setup(){
    
    glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();
    glfw->setWindow(glfwWindow);
    glfw->initializeWindow();
    ofHideCursor();
    
    #ifdef TARGET_OSX
        int fMode;
        if (top) fMode = ofxCocoaWindowUtils::SCREENSAVER;
        else            fMode = ofxCocoaWindowUtils::NORMAL;
        cocoaWindowUtils.setup(fMode, x, y, width, height, border);
    
    #else
        ofSetWindowPosition(x,  y);
        ofSetWindowShape(width, height);
        ofSetFullscreen(fullscreen);
    #endif
    
}
  
/******************************************

 ACCESSORS

 ********************************************/

ofPoint Window::getPosition() {
    return ofPoint(x, y);
}
void Window::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    ofSetWindowPosition(x, y);
}

ofPoint Window::getResolution() {
    return ofPoint(width, height);
}
void Window::setResolution(int w, int h) {
    width = w;
    height = h;
    ofSetWindowShape(w, h);
}

/******************************************

 SETTINGS

 ********************************************/

void Window::loadXML(ofXml &xml) {
    string str = "";
    if (xml.exists("[@position]")) {
        str = xml.getAttribute("[@position]");
        x = ofToInt(ofSplitString(str, ",")[0]);
        y = ofToInt(ofSplitString(str, ",")[1]);
    }
    if (xml.exists("[@resolution]")) {
        str = xml.getAttribute("[@resolution]");
        width = ofToInt(ofSplitString(str, ",")[0]);
        height = ofToInt(ofSplitString(str, ",")[1]);
    }
    if (xml.exists("[@fullscreen]")) {
        str = ofToString( xml.getAttribute("[@fullscreen]") );
        if (str == "on") fullscreen = true;
        else             fullscreen = false;
    }
    if (xml.exists("[@border]")) {
        str = ofToString( xml.getAttribute("[@border]") );
        if (str == "on") border = true;
        else             border = false;
    }
    if (xml.exists("[@top]")) {
        str = ofToString( xml.getAttribute("[@top]") );
        if (str == "on") top = true;
        else             top = false;
    }
    setup();
}

void Window::saveXML(ofXml &xml) {
    xml.setAttribute("position", ( ofToString(x) + "," + ofToString(y) ) );
    xml.setAttribute("resolution", ( ofToString(width) + "," + ofToString(height) ) );

    if (fullscreen) xml.setAttribute("fullscreen", "on" );
    else            xml.setAttribute("fullscreen", "off" );

    if (border) xml.setAttribute("border", "on" );
    else            xml.setAttribute("border", "off" );
    
    if (top) xml.setAttribute("top", "on" );
    else            xml.setAttribute("top", "off" );
}
}
