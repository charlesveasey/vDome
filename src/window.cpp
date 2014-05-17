#include "window.h"
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Window::Window(){
    position.set(0,0);
    dimensions.set(1920,1080);
    fullscreen = false;
    border = false;
    floatToTop = false;
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
    
   /* #ifdef TARGET_OSX
        int fMode;
        if (floatToTop) fMode = ofxCocoaWindowUtils::SCREENSAVER;
        else            fMode = ofxCocoaWindowUtils::NORMAL;
        cocoaWindowUtils.setup(fMode, getPosition().x, getPosition().y, getDimensions().x, getDimensions().y, border);
    
    #else*/
        ofSetWindowPosition(position.x,  position.y);
        ofSetWindowShape(dimensions.x, dimensions.y);
        ofSetFullscreen(fullscreen);
    //#endif
    
}
  
/******************************************

 ACCESSORS

 ********************************************/

ofVec2f Window::getPosition() {
    return ofVec2f(position.x, position.y);
}
void Window::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
    ofSetWindowPosition(position.x, position.y);
}

ofVec2f Window::getDimensions() {
    return ofVec2f(dimensions.x, dimensions.y);
}
void Window::setDimensions(int w, int h) {
    dimensions.x = w;
    dimensions.y = h;
    ofSetWindowShape(dimensions.x, dimensions.y);
}

/******************************************

 SETTINGS

 ********************************************/

void Window::loadXML(ofXml &xml) {
    string str = "";
    if (xml.exists("[@x]"))
        position.x = ofToInt( xml.getAttribute("[@x]") );
    if (xml.exists("[@y]"))
        position.y = ofToInt( xml.getAttribute("[@y]") );
    if (xml.exists("[@width]"))
        dimensions.x = ofToInt( xml.getAttribute("[@width]") );
    if (xml.exists("[@height]"))
        dimensions.y = ofToInt( xml.getAttribute("[@height]") );
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
    if (xml.exists("[@float]")) {
        str = ofToString( xml.getAttribute("z[@float]") );
        if (str == "on") floatToTop = true;
        else             floatToTop = false;
    }
    setup();
}

void Window::saveXML(ofXml &xml) {
    xml.setAttribute("x", ofToString(position.x));
    xml.setAttribute("y", ofToString(position.y));
    xml.setAttribute("width", ofToString(dimensions.x));
    xml.setAttribute("height",  ofToString(dimensions.y));

    if (fullscreen) xml.setAttribute("fullscreen", "on" );
    else            xml.setAttribute("fullscreen", "off" );

    if (border) xml.setAttribute("border", "on" );
    else            xml.setAttribute("border", "off" );
    
    if (floatToTop) xml.setAttribute("float", "on" );
    else            xml.setAttribute("float", "off" );    
}
}
