#include "ofMain.h"
#include "QuadWarp.h"

class Plane {
	
public:
    Plane();
    void setup(int i);
    void update();
    void draw();
    void drawConfig();
    
    ofVboMesh mesh;
    QuadWarp keystone;
    
    void keyPressed(int key);
    void keyReleased(int key);
    void onMouseDragged(ofMouseEventArgs& mouseArgs);
    void onMousePressed(ofMouseEventArgs& mouseArgs);
    void onMouseReleased(ofMouseEventArgs& mouseArgs);
    void load(ofXml &xml);
    void save(ofXml &xml);
    
    void resetKeystone();
    void resetGrid();
    
    vector<float>position;
    
    float value;
    
    bool keystoneActive;
    bool gridActive;

    
    ofMatrix4x4 mat;
    
    vector<ofVec3f> orgVerts;
    vector<ofVec3f> gridVerts;
    vector<ofPoint> keyVals;
    
    vector<ofPoint> getKeystonePoints();
    void setKeystonePoints(vector<ofPoint> pts);
    
    vector<ofVec3f> getGridPoints();
    void setGridPoints(vector<ofVec3f> v);
    
private:
    
    float index;
    bool shift;
    ofPoint lastM;
    map<int, bool> sel;
    bool group;
    bool drawBox;
    ofPoint boxOrigin;
    ofPoint boxUpdate;
    int xRes;
    int yRes;
    int pointIndex;
    
    int width;
    int height;
    
    vector<ofPoint> keystonePoints;
 
};

