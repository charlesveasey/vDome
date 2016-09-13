#pragma once
#include "ofMain.h"
#include "curves.h"
#include "Warp.h"

namespace vd {
class Command;
class Projector {

public:
    // initializes class
    // params: projector index, window sum projector starting index
    void    init(int i, int pStartingIndex);
    void    setup();
    
    void    begin();
    void    end();
    
    void    mousePressed(ofMouseEventArgs& mouseArgs);
    void    mouseDragged(ofMouseEventArgs& mouseArgs);
    void    mouseReleased(ofMouseEventArgs& mouseArgs);
	void    mouseMoved(ofMouseEventArgs& mouseArgs);
	void    mouseScrolled(ofMouseEventArgs& mouseArgs);
    void    keyPressed(int key);
    void    keyReleased(int key);

    void    loadXML(ofXml &xml);
    void    loadXML2(ofXml &xml);
    void    saveXML(ofXml &xml);

    // execute undo / redo commands
	Command*    execute(float v);
    
    // reset undo / redo commnads
	Command*    reset();
    
    ofPoint getPlanePosition();
    void    setPlanePosition(int x, int y);

    int     getWidth();
    int     getHeight();
    
    ofVec3f getCameraPosition();
    void    setCameraPosition(float azi, float ele, float dis);
    
    ofVec3f getCameraOrientation();
    void    setCameraOrientation(float roll, float tilt, float pan);
    
    float   getCameraFov();
    void    setCameraFov(float v);

	ofPoint	getShear();
	void	setShear(float x, float y);
    
	void    setBrightness(float b);
    void    setContrast(float b);
    void    setSaturation(float b);

	void    setWarpActive(bool b);
    
    void    setPolar(float azi, float ele, float dis);

	void		setBlendEdges(float left, float right, float top, float bottom);
	glm::vec4&	getBlendEdges();


	ofTexture&      getTextureReference();
    
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
    
    float   brightness, contrast, saturation;
    

	// color curves
	Curves  curves;
    
    int     projectorStartingIndex;
    
    enum    editModes{
                BRIGHTNESS, CONTRAST, SATURATION, SHEARX, SHEARY,
                CURVES, GRID, FOV, NONE, WHITE, BLACK, ENABLE, 
				BLEND_LEFT, BLEND_RIGHT, BLEND_TOP, BLEND_BOTTOM };
    
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

    ofMatrix4x4     transform;
    
    WarpList        mWarps;
    string          warpSettings;

    int             x, y, width, height;
    
    ofVec3f         sphToCar(ofVec3f t);
    float           round(float d);
    float           roundTo(float val, float n);

	float           shearX;
	float           shearY;

	glm::vec4		blendEdges;

};



}////////////
