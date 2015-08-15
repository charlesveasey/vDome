#include "plane.h"
namespace vd {

extern float projCount;

/******************************************

 CONSTRUCTOR

 ********************************************/

Plane::Plane(){
    cornerpinValues.push_back(ofPoint(0,0));
    cornerpinValues.push_back(ofPoint(1,0));
    cornerpinValues.push_back(ofPoint(0,1));
    cornerpinValues.push_back(ofPoint(1,1));
    cornerpinPoints.push_back(ofPoint(0,0));
    cornerpinPoints.push_back(ofPoint(1,0));
    cornerpinPoints.push_back(ofPoint(0,1));
    cornerpinPoints.push_back(ofPoint(1,1));

	bfirst = true;
    group = false;
    drawBox = false;
    xRes = 10;
    yRes = 10;
    pointIndex = -1;
	value = 1;
    width = 1024;
    height = 768;
	cornerpinActive = false;
	gridActive = false;
    
    wXml = new ofXml;
}
    
/******************************************

 SETUP

 ********************************************/

void Plane::setup(int i){
    index = i;

    int w = width;
    int h = height;

    int x = position[0];
    int y = position[1];

    value = 1;
    
    ofPoint tl(cornerpinValues[0].x, cornerpinValues[0].y);
    ofPoint tr(cornerpinValues[1].x, cornerpinValues[1].y);
    ofPoint bl(cornerpinValues[2].x, cornerpinValues[2].y);
    ofPoint br(cornerpinValues[3].x, cornerpinValues[3].y);

    cornerpin.setAnchorSize(w/2, h/2);
    cornerpin.setSourceRect(ofRectangle(0,0,w,h));
    cornerpin.setTopLeftCornerPosition(tl);
    cornerpin.setTopRightCornerPosition(tr);
    cornerpin.setBottomLeftCornerPosition(bl);
    cornerpin.setBottomRightCornerPosition(br);
    cornerpin.setup();

    grid.setup(width, height, 6, 20);
    
    vector<ofVec3f> v = grid.getVertices();
    for (int i=0; i<v.size(); i++) {
        v[i] = cornerpin.getMatrix().preMult(v[i]);
    }
    grid.setVertices(v);
}

/******************************************

 RESET

 ********************************************/

void Plane::resetCornerpin(){
    cornerpin.reset();
}

void Plane::resetGrid(){
    grid.reset();
}

/******************************************

 DRAW

 ********************************************/

void Plane::draw(){
	if (cornerpinActive || gridActive) {
        vector<ofVec3f> c = grid.getControlPnts();
        
        ofMatrix4x4 m = cornerpin.getMatrix();

		/*
        if (bfirst) {
            lm.makeIdentityMatrix();
            bfirst = false;
        }
        else lm = lm.getInverse();
        
        for (int i=0; i<c.size(); i++) {
            c[i] = lm.preMult(c[i]);
		}
        for (int i=0; i<c.size(); i++) {
            c[i] = m.preMult(c[i]);
		}*/
        
        grid.setControlPnts(c);
        
		vector<ofVec3f> v = grid.getVertices();
		for (int i=0; i<v.size(); i++) {
			v[i] = cornerpin.getMatrix().preMult(v[i]);
		}
		
		grid.setVertices(v);

        lm = m;
	}
    grid.draw();
}

void Plane::drawConfig(){    
    if (gridActive) {
        ofPushMatrix();
        ofTranslate(position[0], position[1]);
            grid.drawControls();
        ofPopMatrix();
    }
}

/******************************************

 KEYBOARD

 ********************************************/

void Plane::keyPressed(int key){
    ofKeyEventArgs keyArgs;
    keyArgs.key = key;
    if (cornerpinActive)
        cornerpin.keyPressed(keyArgs);
    else if (gridActive) {
        grid.keyPressed(keyArgs);
        /*if (key == 45){ // - = decrease points
            if (grid.getControlPntDim() > 2)
                grid.setControlPntDim(grid.getControlPntDim()-1);
        }
        else if (key == 61){ // - = increase points
            grid.setControlPntDim(grid.getControlPntDim()+1);
        }*/
    }
}

void Plane::keyReleased(int key){
    ofKeyEventArgs keyArgs;
    keyArgs.key = key;
    if (cornerpinActive)
        cornerpin.keyReleased(keyArgs);
    else if (gridActive)
        grid.keyReleased(keyArgs);
}

/******************************************

 MOUSE

 ********************************************/

void Plane::onMouseDragged(ofMouseEventArgs& mouseArgs){
    mouseArgs.x -= position[0];
    mouseArgs.y -= position[1];

    if (cornerpinActive)
        cornerpin.onMouseDragged(mouseArgs);
    else if (gridActive)
        grid.mouseDragged(mouseArgs);
}

void Plane::onMousePressed(ofMouseEventArgs& mouseArgs){
    mouseArgs.x -= position[0];
    mouseArgs.y -= position[1];
    if (cornerpinActive)
        cornerpin.onMousePressed(mouseArgs);
    else if (gridActive)
        grid.mousePressed(mouseArgs);
}

void Plane::onMouseReleased(ofMouseEventArgs& mouseArgs){
    mouseArgs.x -= position[0];
    mouseArgs.y -= position[1];
    if (gridActive)
        grid.mouseReleased(mouseArgs);
}

/******************************************

 SETTINGS

 ********************************************/

void Plane::load(ofXml &xml) {
    wXml->clear();
    wXml->load("settings/warp/warp-"+ofToString(index+1)+".xml");

    if (wXml->exists("cornerpin[@points]")) {
        string str = wXml->getAttribute("cornerpin[@points]");
        cornerpinValues[0].x = ofToFloat(ofSplitString(str, ",")[0]);
        cornerpinValues[0].y = ofToFloat(ofSplitString(str, ",")[1]);
        cornerpinValues[1].x = ofToFloat(ofSplitString(str, ",")[2]);
        cornerpinValues[1].y = ofToFloat(ofSplitString(str, ",")[3]);
        cornerpinValues[2].x = ofToFloat(ofSplitString(str, ",")[4]);
        cornerpinValues[2].y = ofToFloat(ofSplitString(str, ",")[5]);
        cornerpinValues[3].x = ofToFloat(ofSplitString(str, ",")[6]);
        cornerpinValues[3].y = ofToFloat(ofSplitString(str, ",")[7]);
    }
    
    wXml->setTo("bezier");
    string str;
    vector<ofVec3f> vec;
    for (int i = 0; i<wXml->getNumChildren(); i++) {
        if (wXml->exists("point["+ofToString(i)+"][@xyz]")) {
            str = wXml->getAttribute("point["+ofToString(i)+"][@xyz]");
            int x = ofToInt(ofSplitString(str, ",")[0]);
            int y = ofToInt(ofSplitString(str, ",")[1]);
            int z = ofToInt(ofSplitString(str, ",")[2]);
            vec.push_back(ofVec3f(x,y,z));
        }
    }
    
    int w = width;
    int h = height;
    
    int x = index*w;
    int y = 0;
    
    ofPoint tl(cornerpinValues[0].x*w, cornerpinValues[0].y*h);
    ofPoint tr(cornerpinValues[1].x*w, cornerpinValues[1].y*h);
    ofPoint bl(cornerpinValues[2].x*w, cornerpinValues[2].y*h);
    ofPoint br(cornerpinValues[3].x*w, cornerpinValues[3].y*h);
    
    cornerpin.setAnchorSize(w/2, h/2);
    cornerpin.setSourceRect(ofRectangle(0,0,w,h));
    cornerpin.setTopLeftCornerPosition(tl);
    cornerpin.setTopRightCornerPosition(tr);
    cornerpin.setBottomLeftCornerPosition(bl);
    cornerpin.setBottomRightCornerPosition(br);

    grid.setup(width, height, 6, 20);

    if (vec.size() > 0) {
        grid.setControlPnts(vec);
    }
    
    gridActive = true;
    draw();
    gridActive = false;

}

void Plane::save(ofXml &xml) {
    int w = width;
    int h = height;
    int x = index*w;
    int y = 0;
        
    wXml->clear();

    wXml->addChild("projector");
    wXml->setTo("projector");
    
    wXml->addChild("cornerpin");
    wXml->setTo("cornerpin");
    
    wXml->setAttribute("points",
                         ofToString((cornerpin.dstPoints[0].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[0].y)/h) +  "," +
                         ofToString((cornerpin.dstPoints[1].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[1].y)/h) +  "," +
                         ofToString((cornerpin.dstPoints[3].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[3].y)/h) +  "," +
                         ofToString((cornerpin.dstPoints[2].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[2].y)/h) );
    
    wXml->setToParent();
    wXml->addChild("bezier");
    wXml->setTo("bezier");
    
    vector<ofVec3f> vec = grid.getControlPnts();
    for (int i = 0; i<vec.size(); i++) {
        wXml->addChild("point");
        wXml->setToChild(i);
        wXml->setAttribute("xyz", ofToString(vec[i].x) + "," + ofToString(vec[i].y) + "," + ofToString(vec[i].z));
        wXml->setToParent();
    }
}

vector<ofPoint> Plane::getCornerpinPoints() {
    for (int i=0; i<4; i++) {
        cornerpinPoints[i] = cornerpin.dstPoints[i];
    }
    return cornerpinPoints;
}
void Plane::setCornerpinPoints(vector<ofPoint> pts){
    for (int i=0; i<4; i++) {
        cornerpinPoints[i] = pts[i];
    }
    cornerpin.setTopLeftCornerPosition(cornerpinPoints[0]);
    cornerpin.setTopRightCornerPosition(cornerpinPoints[1]);
    cornerpin.setBottomLeftCornerPosition(cornerpinPoints[3]);
    cornerpin.setBottomRightCornerPosition(cornerpinPoints[2]);
}

vector<ofVec3f> Plane::getGridPoints() {
    return grid.getControlPnts();
}
void Plane::setGridPoints(vector<ofVec3f> v) {
    grid.setControlPnts(v);
}

}
