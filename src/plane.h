#pragma once
#include "ofMain.h"
#include "quadWarp.h"
#include "ofxBezierSurface.h"
namespace vd {

class Plane {

public:
    Plane();
    
    int width;
    int height;
    
    void setup(int i);
    void update();
    void draw();
    void drawConfig();

    void resetCornerpin();
    void resetGrid();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void onMouseDragged(ofMouseEventArgs& mouseArgs);
    void onMousePressed(ofMouseEventArgs& mouseArgs);
    void onMouseReleased(ofMouseEventArgs& mouseArgs);
    
    void load(ofXml &xml);
    void save(ofXml &xml);

    ofXml *wXml;
    float value;
    ofMatrix4x4 lm;
    bool bfirst;
    QuadWarp cornerpin;
    ofxBezierSurface grid;
    bool cornerpinActive;
    bool gridActive;
    vector<ofPoint> cornerpinValues;
    vector<ofPoint> getCornerpinPoints();
    void setCornerpinPoints(vector<ofPoint> pts);
    vector<float>position;
    
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
    vector<ofPoint> cornerpinPoints;
};

}
