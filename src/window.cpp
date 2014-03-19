#include "window.h"

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Window::Window(){
    position.set(0,0);
    dimensions.set(1920,1080);
    fullscreen = false;
}

/******************************************
 
 SETUP
 
 ********************************************/

void Window::setup(){
    ofSetWindowPosition(position.x,  position.y);
    ofSetWindowShape(dimensions.x, dimensions.y);
	ofSetFullscreen(fullscreen);
}

/******************************************
 
 ACCESSORS
 
 ********************************************/

ofVec2f Window::getPosition() {
    position.x = ofGetWindowPositionX();
    position.y = ofGetWindowPositionY();
    return ofVec2f(position.x, position.y);
}
void Window::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
    ofSetWindowPosition(position.x, position.y);
}

ofVec2f Window::getDimensions() {
    dimensions.x = ofGetWindowWidth();
    dimensions.y = ofGetWindowHeight();
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
    if (xml.exists("window[@x]"))
        position.x = ofToInt( xml.getAttribute("window[@x]") );
    if (xml.exists("window[@y]"))
        position.y = ofToInt( xml.getAttribute("window[@y]") );
    if (xml.exists("window[@width]"))
        dimensions.x = ofToInt( xml.getAttribute("window[@width]") );
    if (xml.exists("window[@height]"))
        dimensions.y = ofToInt( xml.getAttribute("window[@height]") );
	if (xml.exists("window[@fullscreen]")) {
        string str;
        if (str == "true")
            fullscreen = true;
        else
            fullscreen = false;        
    }
    setup();
}

void Window::saveXML(ofXml &xml) {
    xml.setAttribute("window[@x]", ofToString(ofGetWindowPositionX() ));
    xml.setAttribute("window[@y]", ofToString(ofGetWindowPositionY() ));
    xml.setAttribute("window[@width]", ofToString(ofGetWindowWidth() ));
    xml.setAttribute("window[@height]",  ofToString(ofGetWindowHeight() ));
    
    if (fullscreen)
        xml.setAttribute("window[@fullscreen]", "true" );
    else
        xml.setAttribute("window[@fullscreen]", "false" );    
}