#pragma once
#include "ofMain.h"
#include "cornerpin.h"
#include "ofxBezierSurface.h"
namespace vd {

class Plane {

public:
    Plane();
    
    int     width;
    int     height;
    
    void    setup(int i);
    void    update();
    void    draw();
    void    drawConfig();

    void    resetCornerpin();
    void    resetGrid();
    
    void    keyPressed(int key);
    void    keyReleased(int key);
    void    onMouseDragged(ofMouseEventArgs& mouseArgs);
    void    onMousePressed(ofMouseEventArgs& mouseArgs);
    void    onMouseReleased(ofMouseEventArgs& mouseArgs);
    
    void    load(ofXml &xml, int projectorStartingIndex);
    void    save(ofXml &xml);

    void    setCornerpinPoints(vector<ofPoint> pts);
    void    setGridPoints(vector<ofVec3f> v);

    vector<ofVec3f>     getGridPoints();

    ofXml   *wXml;
    float   value;
    
    ofMatrix4x4 lm;
    bool        bfirst;
    
    QuadWarp            cornerpin;
    ofxBezierSurface    grid;
    
    bool                cornerpinActive;
    bool                gridActive;
    vector<ofPoint>     cornerpinValues;
    vector<ofPoint>     getCornerpinPoints();
    vector<float>       position;


private:
    float   index;
    bool    shift;
    ofPoint lastM;
    bool    group;
    bool    drawBox;
    ofPoint boxOrigin;
    ofPoint boxUpdate;
    int     xRes;
    int     yRes;
    int     pointIndex;
    
    map<int, bool>  sel;
    vector<ofPoint> cornerpinPoints;
};

}
