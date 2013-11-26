#include "window.h"

void Window::setup(){
    
    x = 0;
    y = 0;
    width = 2048;
    height = 768;
    vSync = false;

    ofSetWindowPosition(x, y);
    ofSetWindowShape(width, height);
    ofSetVerticalSync(vSync);
}
