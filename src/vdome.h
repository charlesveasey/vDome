#pragma once
#include "ofMain.h"
#include "window.h"
#include "tcp.h"
#include "menu.h"
namespace vd {

class vdome : public ofBaseApp {

public:
    vdome();

private:
    Menu menu;
    Tcp tcp;
    Input input;
    Render render;
    Window window;
    Dome dome;
    ofShader shader;

    vector<Projector> projectors;

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);

    ofVec3f sphToCar(ofVec3f t);

    ofXml xml;
    string xmlFile;
    void loadXML(string file);
    void saveXML(string file);

    void exit();
};

}
