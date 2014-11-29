#pragma once
#include "projector.h"
#include "commands.h"

namespace vd {

extern float projCount;
extern int maxHistory;
extern CommandHistory history;
extern vector<ofPixels> maskHistory;

/******************************************

 INIT

 ********************************************/

void Projector::init(int i){

    // defaults
    index = i;

    keyboard = false;
    mouse = false;

    // intensity
    brightness = 1;
    contrast = 1;
    blackLevel = 0;
    whiteLevel = 255;

    // color
    hue = 1;
    saturation = 1;
    lightness = 1;

    gamma = 1;
    gammaR = 1;
    gammaG = 1;
    gammaB = 1;

	active = false;
    enable = true;
    
    fboSample = 4;

    width = 1024;
    height = 768;
    
    // camera
    cameraPosition.set(0,0,10);    // azi, ele, dis
    cameraOrientation.set(0,-20,0);    // roll, tilt, pan
    cameraFov = 72;
    cameraOffset.set(0,0);
    cameraScale.set(1,1);

    /* shear
     1,   xy,   xz,  0,
     yx,   1,   yz,  0,
     zx,  zy,   1,   0,
     0,    0,   0,   1
     */
    cameraShear.push_back(0); // 0=xy
    cameraShear.push_back(0); // 1=xz
    cameraShear.push_back(0); // 2=yx
    cameraShear.push_back(0); // 3=yz
    cameraShear.push_back(0); //1 4=zx
    cameraShear.push_back(0); // 5=zy

	curves.init(i);

    mask.tx = planePosition.x;
    mask.ty = planePosition.y;
    mask.init(i);    
}








/******************************************

 SETUP

 ********************************************/

void Projector::setup() {

    // projection plane
    plane.position.clear();
    plane.position.push_back(planePosition.x);
    plane.position.push_back(planePosition.y);
    plane.setup(index);
    
    // create camera
    camera.setScale(1,-1,1); // legacy oF oddity
    camera.setNearClip(5);
    camera.setLensOffset(cameraOffset);
    camera.setFov(cameraFov);
    setCameraTransform();

    // create camera view
    view.setWidth(width);
    view.setHeight(height);

    // create camera fbo
    if (fbo.getWidth() != width || fbo.getHeight() != height) {
        fbo.allocate(width, height, GL_RGBA);
        renderFbo.setUseTexture(true);
        renderFbo.allocate(width, height, GL_RGBA, fboSample);
        
        int x = planePosition.x;
        int y = planePosition.y;
        int w = width;
        int h = height;
        
        renderPlane = ofMesh::plane(w, h, 2, 2, OF_PRIMITIVE_TRIANGLES);
        vector<ofVec3f> v = renderPlane.getVertices();
        
        for (int i=0; i<v.size(); i++) {
            renderPlane.setVertex(i, ofVec3f(
                                      v[i].x + w/2 + x,
                                      v[i].y + h/2 + y,
                                      v[i].z    ));
            
            renderPlane.setTexCoord(i, ofVec2f(
                                        v[i].x + w/2,
                                        v[i].y + h/2    ));
        }
    }

    fbo.begin();
        ofClear(255);
    fbo.end();
    
    renderFbo.begin();
        ofClear(255);
    renderFbo.end();
    
	curves.setup();

    mask.tx = planePosition.x;
    mask.ty = planePosition.y;
    mask.setup();    
}

// camera transform
void Projector::setCameraTransform(){
    camera.resetTransform();

    camera.roll(cameraOrientation.x);
    camera.tilt(cameraOrientation.y*-1);
    camera.pan(cameraOrientation.z+cameraPosition.x*-1);

    // spherical coordinates: azi, ele, dis
    ofVec3f car = sphToCar(ofVec3f(cameraPosition.x*-1, cameraPosition.y, cameraPosition.z));
    camera.setPosition(car);
}


















/******************************************

 BEGIN

 ********************************************/

void Projector::begin() {
    ofMatrix4x4 mat = camera.getProjectionMatrix(view);
    ofMatrix4x4 transform;

    /*
    shear
        1,   xy,   xz,  0,
        yx,   1,   yz,  0,
        zx,  zy,   1,   0,
        0,    0,   0,   1
    */
    transform.set(
      cameraScale.x,   cameraShear[0],   cameraShear[1],   0,
      cameraShear[2],   cameraScale.y,   cameraShear[3],   0,
      cameraShear[4],   cameraShear[5],             1,   0,
             0,          0,          0,             1
    );

    ofMatrix4x4 m;
    m.makeFromMultiplicationOf(transform, mat);
    camera.setProjectionMatrix(m);

    fbo.begin();
        ofClear(0, 0, 0, 0);
        camera.begin(view);
}

void Projector::end() {
        camera.end();
    fbo.end();
}












/******************************************

 BIND

 ********************************************/

void Projector::bind() {
    fbo.getTextureReference().bind();
}

void Projector::unbind() {
    fbo.getTextureReference().unbind();
}












/******************************************

 UPDATE

 ********************************************/
void Projector::update() {
	if (curves.enabled){
		curves.update();
	}
}











/******************************************

 DRAW

 ********************************************/

void Projector::draw() {
    glEnable(GL_CULL_FACE);
    glCullFace( GL_FRONT );
    plane.draw();
    glDisable(GL_CULL_FACE);
}

void Projector::drawPlaneConfig(){
   plane.drawConfig();
}

void Projector::drawCurves(int x, int y){
	curves.draw(x,y);
}

void Projector::drawKeystone(){
    ofPushMatrix();
    ofTranslate(plane.position[0], plane.position[1]);
        plane.cornerpin.draw();
    ofPopMatrix();
}















/******************************************

 MOUSE

 ********************************************/
void Projector::mousePressed(ofMouseEventArgs& mouseArgs) {
    if (editMode == CORNERPIN || editMode == GRID) {
        if (editMode == CORNERPIN) {
            lastKey = getKeystonePoints();
        }
        else if (editMode == GRID) {
            lastGrid = getGridPoints();
        }
        plane.onMousePressed(mouseArgs);
    }
    else if (editMode == BRUSH_SCALE || editMode == BRUSH_OPACITY) {
        mask.mousePressed(mouseArgs);
    }
}

void Projector::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (editMode == CORNERPIN || editMode == GRID) {
        plane.onMouseDragged(mouseArgs);
    }
    else if (editMode == BRUSH_SCALE || editMode == BRUSH_OPACITY) {
        mask.mouseDragged(mouseArgs);
    }
}

void Projector::mouseReleased(ofMouseEventArgs& mouseArgs) {
    if (editMode == CORNERPIN || editMode == GRID) {
        plane.onMouseReleased(mouseArgs);
    }
    else if (editMode == BRUSH_SCALE || editMode == BRUSH_OPACITY) {
        mask.mouseReleased(mouseArgs);
        history.execute( new SetBrushPoints(*this) );
    }
}















/******************************************

 KEYBOARD

 ********************************************/

void Projector::keyPressed(int key) {

    if (!active) return;

	if (editMode == CURVES)
        curves.keyPressed(key);

    if (editMode == CORNERPIN || editMode == GRID)
        plane.keyPressed(key);
    else if (editMode == BRUSH_SCALE || BRUSH_OPACITY)
        mask.keyPressed(key);


}

void Projector::keyReleased(int key) {
    if (keyboard)
        plane.keyReleased(key);
}



Command* Projector::reset() {
	Command* cmd = NULL;

    switch (editMode) {
        case ENABLE:			cmd = new SetEnable(*this, true);															break;
        case BRIGHTNESS:		cmd = new SetBrightness(*this, 1);															break;
        case CONTRAST:			cmd = new SetContrast(*this, 1);															break;
        case BLACK:				cmd = new SetBlackLevel(*this, 0);															break;
        case WHITE:				cmd = new SetWhiteLevel(*this, 255);														break;
        case HUE:				cmd = new SetHue(*this, 1);																	break;
        case SATURATION:		cmd = new SetSaturation(*this, 1);															break;
        case LIGHTNESS:			cmd = new SetLightness(*this, 1);															break;
        case GAMMA:				cmd = new SetGamma(*this, 1);																break;
        case GAMMA_R:			cmd = new SetGammaR(*this, 1);																break;
        case GAMMA_G:			cmd = new SetGammaG(*this, 1);																break;
        case GAMMA_B:			cmd = new SetGammaB(*this, 1);																break;
		case BRUSH_OPACITY:		cmd = new ResetBrushOpacity(*this, 50);	 													break;
        case BRUSH_SCALE:		cmd = new ResetBrushScale(*this, 1);	 													break;
        case CORNERPIN:			cmd = new ResetCornerpin(*this);															break;
        case GRID:				cmd = new ResetGrid(*this);																	break;
        case AZIMUTH:			cmd = new SetCameraPosition(*this, 0, cameraPosition.y, cameraPosition.z );					break;
        case ELEVATION:			cmd = new SetCameraPosition(*this, cameraPosition.x, 0, cameraPosition.z );					break;
        case DISTANCE:			cmd = new SetCameraPosition(*this, cameraPosition.x, cameraPosition.y, 5 );					break;
        case TILT:				cmd = new SetCameraOrientation(*this, cameraPosition.x, 0, cameraPosition.z );				break;
        case ROLL:				cmd = new SetCameraOrientation(*this, 0, cameraOrientation.y, cameraOrientation.z );		break;
        case PAN:				cmd = new SetCameraOrientation(*this, cameraOrientation.x, cameraOrientation.y, 0);			break;
        case FOV:				cmd = new SetCameraFov(*this, 72);															break;
        case OFFSET_X:			cmd = new SetCameraOffset(*this, 0, cameraOffset.y);										break;
        case OFFSET_Y:			cmd = new SetCameraOffset(*this, cameraOffset.x, 0);										break;
        case SCALE:				cmd = new SetCameraScale(*this, 1, 1);														break;
        case SCALE_X:			cmd = new SetCameraScaleX(*this, 1);														break;
        case SCALE_Y:			cmd = new SetCameraScaleY(*this, 1);														break;
        case SHEAR_XY:			cameraShear[0] = 0;		cmd = new SetCameraShear(*this, cameraShear );						break;
        case SHEAR_XZ:			cameraShear[1] = 0;		cmd = new SetCameraShear(*this, cameraShear );						break;
        case SHEAR_YX:			cameraShear[2] = 0;		cmd = new SetCameraShear(*this, cameraShear );						break;
        case SHEAR_YZ:			cameraShear[3] = 0;		cmd = new SetCameraShear(*this, cameraShear );						break;
        case SHEAR_ZX:			cameraShear[4] = 0;		cmd = new SetCameraShear(*this, cameraShear );						break;
        case SHEAR_ZY:			cameraShear[5] = 0;		cmd = new SetCameraShear(*this, cameraShear );						break;
    }

	return cmd;
}


Command* Projector::execute(float v) {
	bool b = ((v == 1) ? true : false);	
	
	v *= value;

	Command* cmd = NULL;

	switch (editMode) {
        case NONE:				break;
        case ENABLE:			cmd = new SetEnable(*this, b);																					break;
        case BRIGHTNESS:		cmd = new SetBrightness(*this, brightness + v * .1);															break;
        case CONTRAST:			cmd = new SetContrast(*this, contrast + v * .1);																break;
        case BRUSH_SCALE:		cmd = new SetBrushScale(*this, mask.brushScale + v * .1);														break;
        case BRUSH_OPACITY:		cmd = new SetBrushOpacity(*this, mask.brushOpacity + v);														break;
        case BLACK:				cmd = new SetBlackLevel(*this, blackLevel + v);																	break;
        case WHITE:				cmd = new SetWhiteLevel(*this, whiteLevel + v);																	break;
        case HUE:				cmd = new SetHue(*this, hue + v * .1);																			break;
        case SATURATION:		cmd = new SetSaturation(*this, saturation + v * .1);															break;
        case LIGHTNESS:			cmd = new SetLightness(*this, lightness + v * .1);																break;
        case GAMMA:				cmd = new SetGamma(*this, gamma + v * .1);																		break;
        case GAMMA_R:			cmd = new SetGammaR(*this, gammaR + v * .1);																	break;
        case GAMMA_G:			cmd = new SetGammaG(*this, gammaG + v * .1);																	break;
        case GAMMA_B:			cmd = new SetGammaB(*this, gammaB + v * .1);																	break;
        case CORNERPIN:			cmd = new SetCornerpinPoints(*this, plane.getCornerpinPoints(), lastKey);										break;
        case GRID:				cmd = new SetGridPoints(*this, plane.getGridPoints(), lastGrid);												break;
        case AZIMUTH:			cmd = new SetCameraPosition(*this, cameraPosition.x + v, cameraPosition.y, cameraPosition.z);					break;
        case ELEVATION:			cmd = new SetCameraPosition(*this, cameraPosition.x, cameraPosition.y + v, cameraPosition.z);					break;
        case DISTANCE:			cmd = new SetCameraPosition(*this, cameraPosition.x, cameraPosition.y, cameraPosition.z + v);					break;
        case ROLL:				cmd = new SetCameraOrientation(*this, cameraOrientation.x + v, cameraOrientation.y, cameraOrientation.z);		break;
        case TILT:				cmd = new SetCameraOrientation(*this, cameraOrientation.x, cameraOrientation.y + v, cameraOrientation.z);		break;
        case PAN:				cmd = new SetCameraOrientation(*this, cameraOrientation.x, cameraOrientation.y, cameraOrientation.z + v);		break;
        case FOV:				cmd = new SetCameraFov(*this, cameraFov + v);																	break;
        case OFFSET_X:			cmd = new SetCameraOffset(*this, cameraOffset.x + v  * .1, cameraOffset.y);										break;
        case OFFSET_Y:			cmd = new SetCameraOffset(*this, cameraOffset.x, cameraOffset.y + v * .1);										break;
        case SCALE:				cmd = new SetCameraScale(*this, cameraScale.x + v * .1, cameraScale.y + v * .1);								break;
        case SCALE_X:			cmd = new SetCameraScale(*this, cameraScale.x + v * .1, cameraScale.y);											break;
        case SCALE_Y:			cmd = new SetCameraScale(*this, cameraScale.x, cameraScale.y + v * .1);											break;
        case SHEAR_XY:			cameraShear[0] += v *.1;	cmd = new SetCameraShear(*this, cameraShear );										break;
        case SHEAR_XZ:			cameraShear[1] += v *.1;	cmd = new SetCameraShear(*this, cameraShear );										break;
        case SHEAR_YX:			cameraShear[2] += v *.1;	cmd = new SetCameraShear(*this, cameraShear );										break;
        case SHEAR_YZ:			cameraShear[3] += v *.1;	cmd = new SetCameraShear(*this, cameraShear );										break;
        case SHEAR_ZX:			cameraShear[4] += v *.1;	cmd = new SetCameraShear(*this, cameraShear );										break;
        case SHEAR_ZY:			cameraShear[5] += v *.1;	cmd = new SetCameraShear(*this, cameraShear );										break;
    }
	
	return cmd;
}











/******************************************

 SETTINGS

 ********************************************/

void Projector::loadXML(ofXml &xml) {
    string str;
    
    if (xml.exists("[@resolution]")) {
        str = xml.getAttribute("[@resolution]");
        width = ofToFloat(ofSplitString(str, ",")[0]);
        height = ofToFloat(ofSplitString(str, ",")[1]);
    }
}
        
void Projector::loadXML2(ofXml &xml) {
    string str;
    float val;

    // intensity
    if (xml.exists("[@brightness]")) {
        val = ofToFloat( xml.getAttribute("[@brightness]"));
        brightness = val;
    }
    if (xml.exists("[@contrast]")) {
        val = ofToFloat( xml.getAttribute("[@contrast]") );
        contrast = val;
    }
    if (xml.exists("[@levels]")) {
        str = xml.getAttribute("[@levels]");
        blackLevel = ofToFloat(ofSplitString(str, ",")[0]);
        whiteLevel = ofToFloat(ofSplitString(str, ",")[1]);
    }

    // color
    if (xml.exists("[@hsl]")) {
        str = xml.getAttribute("[@hsl]");
        hue = ofToFloat(ofSplitString(str, ",")[0]);
        saturation = ofToFloat(ofSplitString(str, ",")[1]);
        lightness = ofToFloat(ofSplitString(str, ",")[2]);
    }
    if (xml.exists("[@gamma]")) {
        str = xml.getAttribute("[@gamma]");
        gamma = ofToFloat(ofSplitString(str, ",")[0]);
        gammaR = ofToFloat(ofSplitString(str, ",")[1]);
        gammaG = ofToFloat(ofSplitString(str, ",")[2]);
        gammaB = ofToFloat(ofSplitString(str, ",")[3]);
    }


    // plane warp
    plane.width = width;
    plane.height = height;
    plane.load(xml);


    // camera position
    if (xml.exists("[@position]")) {
        str = xml.getAttribute("[@position]");
        float azi  = ofToFloat(ofSplitString(str, ",")[0]);
        float ele  = ofToFloat(ofSplitString(str, ",")[1]);
        float dis  = ofToFloat(ofSplitString(str, ",")[2]);
        setCameraPosition(azi, ele, dis);
    }

    // camera orientation
    if (xml.exists("[@orientation]")) {
        str = xml.getAttribute("[@orientation]");
        float roll = ofToFloat(ofSplitString(str, ",")[0]);
        float tilt = ofToFloat(ofSplitString(str, ",")[1]);
        float pan = ofToFloat(ofSplitString(str, ",")[2]);
        setCameraOrientation(roll, tilt, pan);
    }

    // camera lens fov
    if (xml.exists("[@fov]")) {
        val = ofToFloat( xml.getAttribute("[@fov]") );
        setCameraFov(val);
    }

    //camera lens offset
    if (xml.exists("[@offset]")) {
        str = xml.getAttribute("[@offset]");
        float offX  = ofToFloat(ofSplitString(str, ",")[0]);
        float offY  = ofToFloat(ofSplitString(str, ",")[1]);
        setCameraOffset(offX, offY);
    }

    // camera scale
    if (xml.exists("[@scale]")) {
        str = xml.getAttribute("[@scale]");
        float sx  = ofToFloat(ofSplitString(str, ",")[0]);
        float sy  = ofToFloat(ofSplitString(str, ",")[1]);
        setCameraScale(sx, sy);
    }

    // camera shear
    if (xml.exists("[@shear]")) {
        str = xml.getAttribute("[@shear]");
        cameraShear[0] = ofToFloat(ofSplitString(str, ",")[0]);
        cameraShear[1] = ofToFloat(ofSplitString(str, ",")[1]);
        cameraShear[2] = ofToFloat(ofSplitString(str, ",")[2]);
        cameraShear[3] = ofToFloat(ofSplitString(str, ",")[3]);
        cameraShear[4] = ofToFloat(ofSplitString(str, ",")[4]);
        cameraShear[5] = ofToFloat(ofSplitString(str, ",")[5]);
    }


	curves.load();

    mask.width = width;
    mask.height = height;
    mask.load();
}

void Projector::saveXML(ofXml &xml) {
    // blend
    xml.setAttribute("brightness", ofToString(roundTo(brightness, .001)));
    xml.setAttribute("contrast", ofToString(roundTo(contrast, .001)));
    xml.setAttribute("levels", ofToString(blackLevel) +  "," + ofToString(whiteLevel));
    
    // color
    xml.setAttribute("hsl", ofToString(roundTo(hue, .001)) +  "," + ofToString(roundTo(saturation, .001)) +  "," + ofToString(roundTo(lightness, .001))  );
    xml.setAttribute("gamma", ofToString(roundTo(gamma, .001)) +  "," + ofToString(roundTo(gammaR, .001)) +  "," + ofToString(roundTo(gammaG, .001)) +  "," + ofToString(roundTo(gammaB, .001)) );

    //camera
    xml.setAttribute("position", ofToString(roundTo(cameraPosition.x, .01)) +  "," + ofToString(roundTo(cameraPosition.y, .01)) +  "," + ofToString(roundTo(cameraPosition.z, .01)) );
    xml.setAttribute("orientation", ofToString(roundTo(cameraOrientation.x, .01)) +  "," + ofToString(roundTo(cameraOrientation.y, .01)) +  "," + ofToString(roundTo(cameraOrientation.z, .01)) );
    xml.setAttribute("fov", ofToString(roundTo(cameraFov, .01)));
    xml.setAttribute("offset", ofToString(roundTo(cameraOffset.x, .001)) +  "," + ofToString(roundTo(cameraOffset.y, .001)) );
    xml.setAttribute("scale", ofToString(roundTo(cameraScale.x, .001)) +  "," + ofToString(roundTo(cameraScale.y, .001)) );
    xml.setAttribute("shear", ofToString(roundTo(cameraShear[0], .001)) +  "," + ofToString(roundTo(cameraShear[1], .001)) +  "," + ofToString(roundTo(cameraShear[2], .001)) +
                        "," + ofToString(roundTo(cameraShear[3], .001)) +  "," + ofToString(roundTo(cameraShear[4], .001)) +  "," + ofToString(roundTo(cameraShear[5], .001)) );
    // plane
    plane.save(xml);
	curves.save();
    mask.save();    
}













/******************************************

 ACCESSORS

 ********************************************/


// plane
ofVec2f Projector::getPlanePosition(){
    return planePosition;
}
void Projector::setPlanePosition(float x, float y){
    planePosition.set(x,y);
}

ofVec2f Projector::getPlaneDimensions(){
    return ofVec2f(width, height);
}
void Projector::setPlaneDimensions(float x, float y){
    width = x;
    height = y;
    setup();
}

// camera
ofVec3f Projector::getCameraPosition(){
    return cameraPosition;
}
void Projector::setCameraPosition(float azi, float ele, float dis){    
    cameraPosition.set(azi, ele, dis);
    setCameraTransform();
}

ofVec3f Projector::getCameraOrientation(){
    return cameraOrientation;
}
void Projector::setCameraOrientation(float roll, float tilt, float pan){
    cameraOrientation.set(roll, tilt, pan);
    setCameraTransform();
}

float Projector::getCameraFov(){
    return cameraFov;
}
void Projector::setCameraFov(float v){
    cameraFov = v;
    camera.setFov(v/2);
}

ofVec2f Projector::getCameraOffset(){
    return cameraOffset;
}
void Projector::setCameraOffset(float x, float y){
	cameraOffset.set(0,0);
    camera.setLensOffset(cameraOffset);
    cameraOffset.set(x,y);
    camera.setLensOffset(cameraOffset);
}

ofVec2f Projector::getCameraScale(){
    return cameraScale;
}
void Projector::setCameraScale(float x, float y){
    cameraScale.set(x,y);
}

vector<float> Projector::getCameraShear(){
    return cameraShear;
}
void Projector::setCameraShear(vector<float> v){
    cameraShear = v;
}

// scalar value
void Projector::setValue(float v) {
    value = v;
    plane.value = value;
    plane.cornerpin.value = value;
}

// keystone
bool Projector::getKeystoneActive() {
    return plane.cornerpinActive;
}
void Projector::setKeystoneActive(bool v) {
    plane.cornerpinActive = v;
}

vector<ofPoint> Projector::getKeystonePoints() {
    return plane.getCornerpinPoints();
}
void Projector::setKeystonePoints(vector<ofPoint> pts) {
    plane.setCornerpinPoints(pts);
}

// grid active
bool Projector::getGridActive() {
    return plane.gridActive;
}
void Projector::setGridActive(bool v) {
    plane.gridActive = v;
}

vector<ofVec3f> Projector::getGridPoints(){
    return plane.getGridPoints();
}
void Projector::setGridPoints(vector<ofVec3f> v){
    plane.setGridPoints(v);
}

// texture
ofTexture& Projector::getTextureReference(){
	return fbo.getTextureReference();
}



















/******************************************
 
 MATH
 
 ********************************************/

float Projector::round(float d) {
    return floorf(d + 0.5);
}

float Projector::roundTo(float val, float n){
    return round(val * 1/n) * n;
}
    
ofVec3f Projector::sphToCar(ofVec3f t) {
    float azi, ele, dis;
    float x, y, z;
    azi = ofDegToRad(t.x);
    ele = ofDegToRad(t.y+90);
    dis = t.z;
    x = sin(azi) * sin(ele) * dis;
    y = cos(ele) * dis;
    z = cos(azi) * sin(ele) * dis;
    return ofVec3f(x,y,z);
};



}
