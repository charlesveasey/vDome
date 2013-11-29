#include "window.h"

void Window::init(){
    x = 0;
    y = 0;
    width = 2048;
    height = 768;
}

void Window::setup(){
    ofSetWindowPosition(x, y);
    ofSetWindowShape(width, height);
}

void Window::loadXML(ofXml &xml) {
    if (xml.exists("window[@x]"))
        x = ofToInt( xml.getAttribute("window[@x]") );
    if (xml.exists("window[@y]"))
        y = ofToInt( xml.getAttribute("window[@y]") );
    if (xml.exists("window[@width]"))
        width = ofToInt( xml.getAttribute("window[@width]") );
    if (xml.exists("window[@height]"))
        height = ofToInt( xml.getAttribute("window[@height]") );
    setup();
}

void Window::saveXML(ofXml &xml) {
    xml.setAttribute("window[@x]", ofToString(ofGetWindowPositionX() ));
    xml.setAttribute("window[@y]", ofToString(ofGetWindowPositionY() ));
    xml.setAttribute("window[@width]", ofToString(ofGetWindowWidth() ));
    xml.setAttribute("window[@height]",   ofToString(ofGetWindowHeight() ));
}
