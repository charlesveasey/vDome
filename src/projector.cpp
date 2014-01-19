#include "projector.h"

bool all = false;


/******************************************
 
 INIT
 
 ********************************************/

void Projector::init(int i){
    
    // defaults
    index = i;

    keyboard = false;
    mouse = false;
    xmlPrefix = "projectors/projector[";
    
    
    // intensity
    brightness = 1;
    contrast = 1;
    
    
    // color
    saturation = 1;
    
    
    // plane position
    x = 0;
    y = 0;
    
    // plane dimensions
    width = 1024;
    height = 768;
    
    // plane scale
    scaleX = 1;
    scaleY = 1;
    
    
    // camera position
    azimuth = 0;
    elevation = 0;
    distance = 5;

    // camera orienation
    roll = 0;
    tilt = 0;
    pan = 0;
    
    // camera lens
    fov = 36;
    
    offsetX = 0;
    offsetY = 0;
    
    /* camera shear
     1,   xy,   xz,  0,
     yx,   1,   yz,  0,
     zx,  zy,   1,   0,
     0,    0,   0,   1
     */
    shear.push_back(0); // 0 = xy
    shear.push_back(0); // 1 = xz
    shear.push_back(0); // 2 = yx
    shear.push_back(0); // 3 = yz
    shear.push_back(0); // 4 = zx
    shear.push_back(0); // 5 = zy
    
    setup();
}




/******************************************
 
 SETUP
 
 ********************************************/

void Projector::setup() {
    
    // create camera
    camera.resetTransform();
    camera.setScale(1,-1,1); // legacy oF oddity
    camera.setNearClip(5);
    
    // create camera view
    view.setWidth(width);
    view.setHeight(height);
    
    // create dome master fbo
    if (fbo.getWidth() != width || fbo.getHeight() != height) {
        fbo.allocate(width, height, GL_RGB);
    }
    
    fbo.begin();
    ofClear(255);
    fbo.end();

    // projection plane
    plane.init(index);
    
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
      scaleX,   shear[0],   shear[1],   0,
      shear[2],   scaleY,   shear[3],   0,
      shear[4],   shear[5],          1,   0,
             0,          0,          0,   1
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

void Projector::drawKeystone(){
    plane.keystone.draw();
}







/******************************************
 
 MOUSE
 
 ********************************************/

void Projector::mousePressed(ofMouseEventArgs& mouseArgs) {
    if (mouse)
        plane.onMousePressed(mouseArgs);
}

void Projector::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (mouse)
        plane.onMouseDragged(mouseArgs);
}

void Projector::mouseReleased(ofMouseEventArgs& mouseArgs) {
    if (mouse)
        plane.onMouseReleased(mouseArgs);
}




/******************************************
 
 KEYBOARD
 
 ********************************************/

void Projector::keyPressed(int key) {
    if (!keyboard) {
        return;
    }
    
    plane.keyPressed(key);
    
    switch (key) {
        case 122:
            history.undo();
            break;
        case 121:
            history.redo();
            break;

            
        case 161: // ~ = de/select all projectors
            all = !all;
            keyboard = all;
            break;
            
        case 114: // reset
            switch (editMode) {
                    
                case 1: // projector intensity
                    history.execute( new SetContrast(*this, 1) );
                    history.execute( new SetBrightness(*this, 1) );
                    break;
                    
                case 2: // projector color
                    history.execute( new SetSaturation(*this, 1) );
                    break;
                    
                case 3:
                    // projector keystone
                    plane.resetKeystone();
                    break;
                    
                case 4:
                    // projector grid
                    plane.resetGrid();
                    break;
                    
                case 5: // projector position
                    history.execute( new SetAzimuth(*this, 0) );
                    history.execute( new SetElevation(*this, 0) );
                    history.execute( new SetDistance(*this, 5) );
                    break;
                    
                case 6: // projector orientation
                    history.execute( new SetRoll(*this, 0) );
                    history.execute( new SetTilt(*this, 0) );
                    history.execute( new SetPan(*this, 0) );
                    break;
                    
                case 7: // projector fov
                    history.execute( new SetFov(*this, 33) );
                    break;
                    
                case 8: // projector offset
                    history.execute( new SetOffsetX(*this, 0) );
                    history.execute( new SetOffsetY(*this, 0) );
                    break;
                    
                case 9: // projector scale
                    history.execute( new SetScaleX(*this, 1) );
                    history.execute( new SetScaleY(*this, 1) );
                    break;
                    
                case 10: // projector shear 1
                    shear[3] = 0;
                    shear[1] = 0;
                    shear[4] = 0;
                    break;
                    
                case 11: // projector shear 2
                    shear[5] = 0;
                    shear[0] = 0;
                    shear[2] = 0;
                    break;
            }
            break;
            
            
        case OF_KEY_UP:  // up = switch on mode
            switch (editMode) {

                case 1: // projector brightness
                    history.execute( new SetBrightness(*this, brightness + value * .1) );
                    break;
                    
                case 2: // projector saturation
                    history.execute( new SetSaturation(*this, saturation + value * .1) );
                    break;
                    
                case 3:
                    // keystone
                    break;
                
                case 4:
                    // grid
                    break;
                    
                case 5: // projector elevation / distance (super)
                    if (!superKey)
                        history.execute( new SetElevation(*this, elevation + value) );
                    else
                        history.execute( new SetDistance(*this, distance + value) );
                    break;
                    
                case 6: // projector tilt
                    history.execute( new SetTilt(*this, tilt + value) );
                    break;
                    
                case 7: // projector fov
                    history.execute( new SetFov(*this, fov + value) );
                    break;
                    
                case 8: // projector offset y
                    history.execute( new SetOffsetY(*this, offsetY + value * .1) );
                    break;

                case 9: // projector scale
                    if (!superKey)
                        history.execute( new SetScaleY(*this, scaleY + value * .1) );
                    else {
                        history.execute( new SetScale(*this, scaleX + value * .1, scaleY + value * .1) );
                    }
                    break;
                    
                case 10: // projector shear 1
                    if (!superKey)
                        shear[3] += value * .1;
                    else
                        shear[1] += value * .1;
                    break;              
                    
                case 11: // projector shear 2
                    if (!superKey)
                        shear[5] += value * .1;
                    else
                        shear[0] += value * .1;
                    break;                  

            }
            break;
            
            
        case OF_KEY_DOWN: // down = switch on mode
            
            switch (editMode) {

                case 1: // projector brightness
                    history.execute( new SetBrightness(*this, brightness - value * .1) );
                    break;
                    
                case 2: // projector saturation
                    history.execute( new SetSaturation(*this, saturation - value * .1) );
                    break;
                    
                case 3:
                    // keystone
                    break;
                    
                case 4:
                    // grid
                    break;
                    
                case 5: // projector elevation / distance (super)
                    if (!superKey)
                        history.execute( new SetElevation(*this, elevation - value) );
                    else
                        history.execute( new SetDistance(*this, distance - value) );
                    break;
                    
                case 6: // projector tilt
                    history.execute( new SetTilt(*this, tilt - value) );
                    break;
                    
                case 7: // projector fov
                    history.execute( new SetFov(*this, getFov() - value) );
                    break;
                    
                case 8: // projector lensOffsetY
                    history.execute( new SetOffsetY(*this, offsetY - value * .1) );
                    break;
                    
                case 9: // projector scale
                    if (!superKey)
                        history.execute( new SetScaleY(*this, scaleY - value * .1) );
                    else
                        history.execute( new SetScale(*this, scaleX - value * .1, scaleY - value * .1) );
                    break;
                    
                case 10: // projector shear 1
                    if (!superKey)
                        shear[3] -= value * .1;
                    else
                        shear[1] -= value * .1;
                    break;
                    
                case 11: // projector shear 2
                    if (!superKey)
                        shear[5] -= value * .1;
                    else
                        shear[0] -= value * .1;
                    break;
            }
            break;
            
            
            
            
            
            
        case OF_KEY_LEFT:  // left
            
            switch (editMode) {
                    
                case 1: // projector contrast
                    history.execute( new SetContrast(*this, contrast - value * .1) );
                    break;
                    
                case 5: // projector azimuth
                    history.execute( new SetAzimuth(*this, azimuth - value) );
                break;
                    
                case 6: // projector roll / pan (super)
                    if (!superKey)
                        history.execute( new SetRoll(*this, roll - value) );
                    else
                        history.execute( new SetPan(*this, pan - value) );
                    break;
                    
                case 8: // projector lens offset x
                    history.execute( new SetOffsetX(*this, offsetX - value * .1) );
                    break;
                    
                case 9: // projector scale
                    if (!superKey)
                        history.execute( new SetScaleX(*this, scaleX - value * .1) );
                    else
                        history.execute( new SetScale(*this, scaleX - value * .1, scaleY - value * .1));
                    break;
                    
                case 10: // projector shear 1
                    shear[4] -= value * .1;
                    break;
                    
                case 11: // projector shear 2
                    shear[2] -= value * .1;
                    break;

            }
            break;
        
            
            
            
        case OF_KEY_RIGHT:  // right
            
            switch (editMode) {
                    
                case 1: // projector contrast
                    history.execute( new SetContrast(*this, contrast + value * .1) );
                    break;
                    
                case 5: // projector azimuth
                    history.execute( new SetAzimuth(*this, azimuth + value) );
                    break;
                    
                case 6: // projector roll / pan (super)
                    if (!superKey)
                        history.execute( new SetRoll(*this, roll + value) );
                    else
                        history.execute( new SetPan(*this, pan + value) );
                    break;
                    
                case 8: // projector lensOffsetX
                    history.execute( new SetOffsetX(*this, offsetX + value * .1) );
                    break;
                    
                case 9: // projector scale
                    if (!superKey)
                        history.execute( new SetScaleX(*this, scaleX + value * .1) );
                    else
                        history.execute( new SetScale(*this, scaleX + value * .1, scaleY - value * .1));
                    break;
                    
                case 10: // projector shear 1
                    shear[4] += value * .1;
                    break;
                    
                case 11: // projector shear 2
                    shear[2] += value * .1;
                    break;
            }
            break;
            
        default:
            break;
    } 
}

void Projector::keyReleased(int key) {
    if (keyboard)
        plane.keyReleased(key);
}





/******************************************
 
 XML
 
 ********************************************/

void Projector::loadXML(ofXml &xml) {
    string str;
    float val;
    string pre = xmlPrefix + ofToString(index);
   
    
    // intensity
    if (xml.exists(pre + "][@brightness]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@brightness]") );
        history.execute( new SetBrightness(*this, val) );
    }
    if (xml.exists(pre + "][@contrast]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@contrast]") );
        history.execute( new SetContrast(*this, val) );
    }
    
    
    // color
    if (xml.exists(pre + "][@saturation]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@saturation]") );
        history.execute( new SetSaturation(*this, val) );
    }
    
    
    // plane position
    if (xml.exists(pre + "][@x]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@x]") );
        history.execute( new SetX(*this, val) );
    }
    if (xml.exists(pre + "][@y]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@y]") );
        history.execute( new SetY(*this, val) );
    }
    
    
    // plane dimensions
    if (xml.exists(pre + "][@width]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@x]") );
        history.execute( new SetWidth(*this, val) );
    }
    if (xml.exists(pre + "][@height]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@height]") );
        history.execute( new SetHeight(*this, val) );
    }
    
    
    // plane scale
    if (xml.exists(pre + "][@scaleX]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@scaleX]") );
        history.execute( new SetScaleX(*this, val) );
    }
    if (xml.exists(pre + "][@scaleY]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@scaleY]") );
        history.execute( new SetScaleY(*this, val) );
    }
    
    
    
    plane.load(xml);
   
    
    
    
    // camera position
    if (xml.exists(pre + "][@azimuth]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@azimuth]") );
        history.execute( new SetAzimuth(*this, val) );
    }
    if (xml.exists(pre + "][@elevation]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@elevation]") );
        history.execute( new SetElevation(*this, val) );
    }
    if (xml.exists(pre + "][@distance]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@distance]") );
        history.execute( new SetDistance(*this, val) );
    }
    
    // camera orientation
    if (xml.exists(pre + "][@roll]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@roll]") );
        history.execute( new SetRoll(*this, val) );
    }
    if (xml.exists(pre + "][@tilt]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@tilt]") );
        history.execute( new SetTilt(*this, val) );
    }
    if (xml.exists(pre + "][@pan]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@pan]") );
        history.execute( new SetPan(*this, val) );
    }
    
    // camera lens
    if (xml.exists(pre + "][@fov]")) {
        val = ofToFloat( xml.getAttribute(pre + "][@fov]") );
        history.execute( new SetFov(*this, val) );
    }
    if (xml.exists(pre + "][@offset]")) {
        str = xml.getAttribute(pre + "][@offset]");
        float offX  = ofToFloat(ofSplitString(str, ",")[0]);
        float offY  = ofToFloat(ofSplitString(str, ",")[1]);
        history.execute( new SetOffsetX(*this, offX) );
        history.execute( new SetOffsetY(*this, offY) );
    }
    
}

void Projector::saveXML(ofXml &xml) {
    string pre = xmlPrefix + ofToString(index);
    
    int x = width * index;
    int y = 0;
    
    xml.setAttribute(pre + "][@azimuth]", ofToString(azimuth));
    xml.setAttribute(pre + "][@brightness]", ofToString(brightness));
    xml.setAttribute(pre + "][@contrast]", ofToString(contrast));
    xml.setAttribute(pre + "][@distance]", ofToString(distance));
    xml.setAttribute(pre + "][@elevation]", ofToString(elevation));
    xml.setAttribute(pre + "][@fov]", ofToString(fov));
    xml.setAttribute(pre + "][@height]", ofToString(height));
    xml.setAttribute(pre + "][@offset]", ofToString(offsetX) +  "," + ofToString(offsetY) );
    xml.setAttribute(pre + "][@saturation]", ofToString(saturation));
    xml.setAttribute(pre + "][@roll]", ofToString(roll));
    xml.setAttribute(pre + "][@tilt]", ofToString(tilt));
    xml.setAttribute(pre + "][@pan]", ofToString(pan));
    xml.setAttribute(pre + "][@width]", ofToString(width));
    xml.setAttribute(pre + "][@scale]", ofToString(scaleX) +  "," + ofToString(scaleY) );
    
    plane.save(xml);
}



/******************************************
 
 ACCESSORS
 
 ********************************************/


// scalar value
void Projector::setValue(float v) {
    value = v;
    plane.value = value;
    plane.keystone.value = value;
}

// keystone active
bool Projector::getKeystoneActive() {
    return plane.keystoneActive;
}
void Projector::setKeystoneActive(bool v) {
    plane.keystoneActive = v;
}


// grid active
bool Projector::getGridActive() {
    return plane.gridActive;
}
void Projector::setGridActive(bool v) {
    plane.gridActive = v;
}


// intensity
float Projector::getBrightness(){
    return brightness;
}
void Projector::setBrightness(float v){
    brightness = v;
}

float Projector::getContrast(){
    return contrast;
}
void Projector::setContrast(float v){
    contrast = v;
}


// color
float Projector::getSaturation(){
    return saturation;
}
void Projector::setSaturation(float v){
    saturation = v;
}


// plane position
float Projector::getX(){
    return x;
}
void Projector::setX(float v){
    x = v;
}

float Projector::getY(){
    return y;
}
void Projector::setY(float v){
    y = v;
}

// plane dimensions
float Projector::getWidth(){
    return width;
}
void Projector::setWidth(float v){
    width = v;
}

float Projector::getHeight(){
    return height;
}
void Projector::setHeight(float v){
    height = v;
}


// position
void Projector::setCameraPosition(ofVec3f sph){
    ofVec3f car = sphToCar(sph);
    camera.setPosition(car);
}

float Projector::getAzimuth(){
    return azimuth;
}
void Projector::setAzimuth(float v){
    azimuth = v;
    setCameraPosition(ofVec3f(azimuth, elevation, distance));
}

float Projector::getElevation(){
    return elevation;
}
void Projector::setElevation(float v){
    elevation = v;
    setCameraPosition(ofVec3f(azimuth, elevation, distance));
}

float Projector::getDistance(){
    return distance;
}
void Projector::setDistance(float v){
    distance = v;
    setCameraPosition(ofVec3f(azimuth, elevation, distance));
}


// orientation
float Projector::getRoll(){
    return roll;
}
void Projector::setRoll(float v){
    roll = v;
    camera.roll(roll);    
}

float Projector::getTilt(){
    return tilt;
}
void Projector::setTilt(float v){
    tilt = v;
    camera.tilt(tilt);    
}

float Projector::getPan(){
    return pan;
}
void Projector::setPan(float v){
    pan = v;    
    camera.pan(azimuth + pan);
}


// lens
float Projector::getFov(){
    return fov;
}
void Projector::setFov(float v){
    fov = v;
    camera.setFov(v);
}


void Projector::setOffset(float vx, float vy){
    camera.setLensOffset(ofVec2f(vx, vy));
}

float Projector::getOffsetX(){
    return offsetX;
}
void Projector::setOffsetX(float v){
    offsetX = v;
    setOffset(offsetX, offsetY);
    
}

float Projector::getOffsetY(){
    return offsetY;
}
void Projector::setOffsetY(float v){
    offsetY = v;
    setOffset(offsetX, offsetY);
}


// scale
void Projector::setScale(float vx, float vy){
    scaleX = vx;
    scaleY = vy;
}

float Projector::getScaleX(){
    return scaleX;
}
void Projector::setScaleX(float v){
    scaleX = v;
}

float Projector::getScaleY(){
    return scaleY;
}
void Projector::setScaleY(float v){
    scaleY = v;
}


// texture
ofTexture& Projector::getTextureReference(){
	return fbo.getTextureReference();
}




/******************************************
 
 SPHERICAL TO CARTESIAN COORDINATES
 
 ********************************************/

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
