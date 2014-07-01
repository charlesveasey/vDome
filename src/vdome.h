#pragma once
#include "ofMain.h"
#include "window.h"
#include "socket.h"
#include "menu.h"
#include "saveThread.h"
#include "ofxMultiGLFWWindow.h"

namespace vd {
  
class vdome : public ofBaseApp {

public:
    vdome();

private:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
   
    void dragEvent(ofDragInfo dragInfo);

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    void exit();
    
    Menu menu;
    Socket socket;
    Input input;
    Render render;
    Model model;
    ofShader shader;
    vector<Window> windows;
    vector<Projector> projectors;
    ofVec3f sphToCar(ofVec3f t);

    ofXml xml;
    string xmlFile;
    SaveThread saveThread;
    bool autosave;
    
    ofxMultiGLFWWindow *glfw;
    int wIndex;
    vector<GLFWwindow*> *glfwWindows;
};

}

