#include "ofMain.h"
#include "ofxQuadWarp.h"

class Plane {
	
public:
    
    void init();
    void setup();
    void update();
    void draw();
    void drawConfig();
    
    int indx;
    ofVboMesh mesh;
    ofxQuadWarp keystone;
    
    ofPoint topLeft;
    ofPoint topRight;
    ofPoint bottomLeft;
    ofPoint bottomRight;
    
    void keyPressed(int key);
    void keyReleased(int key);
    void onMouseDragged(ofMouseEventArgs& mouseArgs);
    void onMousePressed(ofMouseEventArgs& mouseArgs);
    void onMouseReleased(ofMouseEventArgs& mouseArgs);
    //void loadXML(ofXml &xml);
    //void saveXML(ofXml &xml);
    
    vector<float>position;
    
    float value = 1;

};

