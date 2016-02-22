#pragma once
#include "ofMain.h"
#include "curves.h"
#include "Warp.h"

namespace vd {
class Command;
class Projector {

public:
    // initializes class
    // params: rojector index, window projector starting indes
    void    init(int i, int pStartingIndex);
    
    // setup class
    void    setup();
    
    // begin fbo capture
    void    begin();
    
    // end fbo capture
    void    end();
    
    // mouse pressed event callback
    void    mousePressed(ofMouseEventArgs& mouseArgs);
    
    // mouse dragged event callback
    void    mouseDragged(ofMouseEventArgs& mouseArgs);
    
    // mouse released event callback
    void    mouseReleased(ofMouseEventArgs& mouseArgs);

	// mouse moved event callback
	void    mouseMoved(ofMouseEventArgs& mouseArgs);

	// mouse scrolled event callback
	void    mouseScrolled(ofMouseEventArgs& mouseArgs);
        
    // keyboard pressed callback
    void    keyPressed(int key);
    
    // keyboard released event callback
    void    keyReleased(int key);
    
    // load xml settings stage 1
    void    loadXML(ofXml &xml);
    
    // load xml settings stage 2
    void    loadXML2(ofXml &xml);
    
    // save xml settings
    void    saveXML(ofXml &xml);

    // execute undo / redo commands
	Command*    execute(float v);
    
    // reset undo / redo commnads
	Command*    reset();
    
    // get render plane x,y position
    ofPoint getPlanePosition();
    
    // set plane x,y position
    void    setPlanePosition(int x, int y);

    // get width
    int     getWidth();
    
    // get height
    int     getHeight();

    // set warp active
    void    setWarpActive(bool b);
    
    // get camera position
    ofVec3f getCameraPosition();

    // set camera position
    void    setCameraPosition(float azi, float ele, float dis);
    
    // get camera orientation
    ofVec3f getCameraOrientation();
    
    // set camera orientation
    void    setCameraOrientation(float roll, float tilt, float pan);
    
    // get camera field of view
    float   getCameraFov();
    
    //set camera field of view
    void    setCameraFov(float v);
    
    // get camera lens offset
    ofVec2f getCameraOffset();
    
    // set camera lens offset
    void    setCameraOffset(float x, float y);
    
    // get texture reference
    ofTexture&      getTextureReference();

    // set brightness
    void    setBrightness(float b);
    
    // set contrast
    void    setContrast(float b);
    
    // set saturation
    void    setSaturation(float b);

    
    void onColorSourceEvent(int &source);
    
    // projector index
    int     index;
    
    bool    keyboard;
    bool    mouse;
    bool    active;
    bool    enable;
    int     editMode;
    bool    mod;
    bool    all;
    void    setValue(float v);
    ofFbo   renderFbo;
    
    float   brightness, contrast, saturation;
    
	// color curves
	Curves  curves;
    
    int     projectorStartingIndex;
    
    enum    editModes{
                BRIGHTNESS,CONTRAST,SATURATION,
                CURVES, GRID, FOV, NONE, WHITE, BLACK, ENABLE};
    
	ofEasyCam        camera;

private:
    ofFbo           fbo;
    ofRectangle     view;

    float           value;
    int             fboSample;
    string          xmlPrefix;

    ofVec3f         cameraPosition;
    ofVec3f         cameraOrientation;
    float           cameraFov;
    ofVec2f         cameraOffset;
    ofVec2f         cameraScale;

    ofMatrix4x4     transform;
    
    WarpList        mWarps;
    string          warpSettings;

    int             x, y, width, height;
    
    ofVec3f         sphToCar(ofVec3f t);
    float           round(float d);
    float           roundTo(float val, float n);
    
};



}////////////
