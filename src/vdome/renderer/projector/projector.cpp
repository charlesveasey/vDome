#include "projector.h"
#include "commands.h"
namespace vd {

//--------------------------------------------------------------
extern int maxHistory;
extern CommandHistory history;
    
//--------------------------------------------------------------
void Projector::init(int i, int pStartingIndex){
    // defaults
    index = i;
    projectorStartingIndex = pStartingIndex;
    keyboard = false;
    mouse = false;
    brightness = 1;
    contrast = 1;
    saturation = 1;
	active = false;
    enable = true;
    x = y = 0;
  
    cameraPosition.set(0,0,1);         // azi, ele, dis
    cameraOrientation.set(0,0,0);     // roll, tilt, pan
    cameraFov = 90;
    cameraOffset.set(0,0);
	camera.setNearClip(.1);
	camera.setFarClip(1000);
    
	camera.setAutoDistance(false);
    camera.disableMouseInput();
    camera.disableMouseDoubleClick();
    camera.setTranslationKey('t');
    camera.setDrag(0);
	camera.setRotationSensitivity(-.25, -.25, .25);
    
    fboSample = 4;

	curves.init(i);
    
    // create warps
    mWarps.push_back( WarpBilinear::create() );
    mWarps[0]->setBrightness(1.0f);
    mWarps[0]->setContrast(1.0f);
    mWarps[0]->setSaturation(1.0f);

    // set window's warp settings file path
    warpSettings = "settings/warp/warp-" + ofToString(index+1) +".xml";
}

//--------------------------------------------------------------
void Projector::setup() {
	camera.setDistance(1);

    // create camera
    camera.setFov(cameraFov);

    // create camera view
    view.setWidth(width);
    view.setHeight(height);
    
    // set up color curves
    curves.setup();

	// set up warp
	Warp::setSize(mWarps, width, height);
    mWarps[0]->setLutTexture(curves.colorlutTextureRef());
}

//--------------------------------------------------------------
void Projector::begin() {
    mWarps[0]->begin();
        ofClear(0, 0, 0, 0);
        camera.begin(view);
}

//--------------------------------------------------------------
void Projector::end() {
        camera.end();
    mWarps[0]->end();
}

//--------------------------------------------------------------
void Projector::mousePressed(ofMouseEventArgs& mouseArgs) {
    if (!active) return;
	if (editMode == GRID) {
		mouseArgs.x -= x;
		Warp::handleMouseDown(mWarps, mouseArgs);
	}
	else if (editMode == FOV) {
		mouseArgs.x -= x;
		camera.mousePressed(mouseArgs);
		camera.update(mouseArgs);
	}
}

//--------------------------------------------------------------
void Projector::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (!active) return;
	if (editMode == GRID) {
		mouseArgs.x -= x;
		Warp::handleMouseDrag(mWarps, mouseArgs);
	}
	else if (editMode == FOV) {
		mouseArgs.x -= x;
		camera.mouseDragged(mouseArgs);
		camera.update(mouseArgs);
	}
}

//--------------------------------------------------------------
void Projector::mouseReleased(ofMouseEventArgs& mouseArgs) {
    if (!active) return;
	if (editMode == GRID) {
		mouseArgs.x -= x;
		Warp::handleMouseUp(mWarps, mouseArgs);
	}
	else if (editMode == FOV) {
		mouseArgs.x -= x;
		camera.mouseReleased(mouseArgs);
		camera.update(mouseArgs);
	}
}

//--------------------------------------------------------------
void Projector::mouseMoved(ofMouseEventArgs& mouseArgs) {
	if (!active) return;
	if (editMode == GRID) {
		mouseArgs.x -= x;
		Warp::handleMouseMove(mWarps, mouseArgs);
	}
}

//--------------------------------------------------------------
void Projector::mouseScrolled(ofMouseEventArgs& mouseArgs) {
	if (!active) return;
	if (editMode == FOV) {
		mouseArgs.x -= x;
		camera.mouseScrolled(mouseArgs);
		camera.update(mouseArgs);
	}
}

//--------------------------------------------------------------
void Projector::keyPressed(int key) {
    if (!keyboard) return;
	ofKeyEventArgs event;
	event.key = key;
	if (editMode == CURVES) {
		curves.keyPressed(key);
	}
	else if (editMode == GRID) {
		if (key == 'l' && mWarps.size())
			mWarps[0]->toggleMappingMode();
		Warp::handleKeyDown(mWarps, event);
	}
}

//--------------------------------------------------------------
void Projector::keyReleased(int key) {
	if (!active) return;
	ofKeyEventArgs event;
	event.key = key;
	if (editMode == GRID) {
		Warp::handleKeyUp(mWarps, event);
	}
}

//--------------------------------------------------------------
Command* Projector::reset() {
	Command* cmd = NULL;

    switch (editMode) {
        case ENABLE:			cmd = new SetEnable(*this, true);															break;
        case BRIGHTNESS:		cmd = new SetBrightness(*this, 1);															break;
        case CONTRAST:			cmd = new SetContrast(*this, 1);															break;
        case SATURATION:		cmd = new SetSaturation(*this, 1);															break;
        case GRID:				cmd = new ResetGrid(*this);																	break;
        case FOV:				cmd = new SetCameraFov(*this, 90);
            camera.reset(); // FIX
            break;
    }

	return cmd;
}

//--------------------------------------------------------------
Command* Projector::execute(float v) {
	bool b = ((v == 1) ? true : false);	
	
	v *= value;

	Command* cmd = NULL;

	switch (editMode) {
        case NONE:				break;
        case ENABLE:			cmd = new SetEnable(*this, b);																					break;
        case BRIGHTNESS:		cmd = new SetBrightness(*this, brightness + v * .1);															break;
        case CONTRAST:			cmd = new SetContrast(*this, contrast + v * .1);																break;
        case SATURATION:		cmd = new SetSaturation(*this, saturation + v * .1);															break;
       // case GRID:				cmd = new SetGridPoints(*this, plane.getGridPoints(), lastGrid);												break;
        case FOV:				cmd = new SetCameraFov(*this, cameraFov + v);																	break;
    }
    
    return cmd;
}

//--------------------------------------------------------------
void Projector::loadXML(ofXml &xml) {
    string str;
    
	// projector resolution
    if (xml.exists("[@resolution]")) {
        str = xml.getAttribute("[@resolution]");
        width = ofToFloat(ofSplitString(str, ",")[0]);
        height = ofToFloat(ofSplitString(str, ",")[1]);
    }    
}

//--------------------------------------------------------------
void Projector::loadXML2(ofXml &xml) {
    string str;
    float val;
	ofXml xw;
    // warp setttings
    if (xw.load(warpSettings)){
        mWarps.clear();
        mWarps = Warp::readSettings( warpSettings );
    }
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
        float x = ofToFloat(ofSplitString(str, ",")[0]);
        float y = ofToFloat(ofSplitString(str, ",")[1]);
        float z = ofToFloat(ofSplitString(str, ",")[2]);
        setCameraOrientation(x, y, z);
    }
    // camera lens fov
    if (xml.exists("[@fov]")) {
        val = ofToFloat( xml.getAttribute("[@fov]") );
        setCameraFov(val);
    }
    // color curves settings
	curves.load(projectorStartingIndex);


	// init warp settings
	Warp::setSize(mWarps, width, height);
	mWarps[0]->setLutTexture(curves.colorlutTextureRef());
}

//--------------------------------------------------------------
void Projector::saveXML(ofXml &xml) {
    //camera settings
    xml.setAttribute("position", ofToString(roundTo(camera.getPosition().x, .01)) +  "," + ofToString(roundTo(camera.getPosition().y, .01)) +  "," + ofToString(roundTo(camera.getPosition().z, .01)) );
    xml.setAttribute("orientation", ofToString(roundTo(camera.getOrientationEuler().x, .01)) +  "," + ofToString(roundTo(camera.getOrientationEuler().y, .01)) +  "," + ofToString(roundTo(camera.getOrientationEuler().z, .01)) );
    xml.setAttribute("fov", ofToString(roundTo(cameraFov, .01)));
    
    // warp settings
    Warp::writeSettings( mWarps, warpSettings );
    
    // color curve settings
	curves.save(projectorStartingIndex);
}

//--------------------------------------------------------------
int Projector::getWidth(){
    return width;
}

//--------------------------------------------------------------
int Projector::getHeight(){
    return height;
}
    
//--------------------------------------------------------------
ofVec3f Projector::getCameraPosition(){
	return camera.getPosition();
}
    
//--------------------------------------------------------------
void Projector::setCameraPosition(float x, float y, float z){
    cameraPosition.set(x, y, z);
	camera.setPosition(x, y, z);
}

//--------------------------------------------------------------
ofVec3f Projector::getCameraOrientation(){
    return camera.getOrientationEuler();
}
    
//--------------------------------------------------------------
void Projector::setCameraOrientation(float x, float y, float z){
	cameraOrientation.set(x, y, z);
	camera.setOrientation(ofVec3f(x, y, z));
}

//--------------------------------------------------------------
float Projector::getCameraFov(){
    return cameraFov;
}
    
//--------------------------------------------------------------
void Projector::setCameraFov(float v){
    cameraFov = v;
    camera.setFov(v/2);
}

//--------------------------------------------------------------
void Projector::setValue(float v) {
    value = v;
}
    
//--------------------------------------------------------------
void Projector::setPlanePosition(int x, int y){
    this->x = x;
    this->y = y;
}

//--------------------------------------------------------------
ofPoint Projector::getPlanePosition(){
    return ofPoint(x, y);
}
    
//--------------------------------------------------------------
ofTexture& Projector::getTextureReference(){
	return fbo.getTexture();
}

//--------------------------------------------------------------
float Projector::round(float d) {
    return floorf(d + 0.5);
}

//--------------------------------------------------------------
float Projector::roundTo(float val, float n){
    return round(val * 1/n) * n;
}
    
//--------------------------------------------------------------
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
}
    
//--------------------------------------------------------------
void Projector::setWarpActive(bool b){
	b ? mWarps[0]->enableEditMode() : mWarps[0]->disableEditMode();
}
    
//--------------------------------------------------------------
void Projector::setBrightness(float b){
    brightness = b;
    if(mWarps.size()){
        mWarps[0]->setBrightness(b);
    }
}

//--------------------------------------------------------------
void Projector::setContrast(float b){
    contrast = b;
    if(mWarps.size()){
        mWarps[0]->setContrast(b);
    }
}


//--------------------------------------------------------------
void Projector::setSaturation(float b){
    saturation = b;
    if(mWarps.size()){
        mWarps[0]->setSaturation(b);
    }
}
    

}/////////
