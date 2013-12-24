#include "ofMain.h"
#include "ofxQuadWarp.h"

class Plane {
	
public:
    
    void init(int i);
    void setup();
    void update();
    void draw();
    void drawConfig();
    
    ofVboMesh mesh;
    ofxQuadWarp keystone;
    
    void keyPressed(int key);
    void keyReleased(int key);
    void onMouseDragged(ofMouseEventArgs& mouseArgs);
    void onMousePressed(ofMouseEventArgs& mouseArgs);
    void onMouseReleased(ofMouseEventArgs& mouseArgs);
    void load(ofXml &xml);
    void save(ofXml &xml);
    
    vector<float>position;
    
    float value = 1;
    
    bool keystoneActive;
    bool gridActive;


    ofMatrix4x4 mat;
    
    vector<ofVec3f> orgVerts;
    vector<ofVec3f> keyVerts;
    vector<ofVec3f> gridVerts;
    
    vector<ofPoint> keyVals;
    
    
private:
    
    float index;
    bool shift;
    ofPoint lastM;
    map<int, bool> sel;
    bool group = false;
    bool drawBox = false;
    ofPoint boxOrigin;
    ofPoint boxUpdate;
    int xRes = 10;
    int yRes = 10;
    int pointIndex = -1;
    
    
};

