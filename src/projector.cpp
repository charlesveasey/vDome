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
    
    shearXY = 0;
    shearXZ = 0;
    shearYX = 0;
    shearYZ = 0;
    shearZX = 0;
    shearZY = 0;
    
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
      scaleX,   shearXY,   shearXZ,   0,
      shearYX,   scaleY,   shearYZ,   0,
      shearZX,   shearZY,          1,   0,
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
                    history.execute( new SetScale(*this, 1, 1) );
                    break;
                    
                case 10: // projector shear 1
                    history.execute( new SetShearXZ(*this, 0) );
                    history.execute( new SetShearYZ(*this, 0) );
                    history.execute( new SetShearZX(*this, 0) );
                    break;
                    
                case 11: // projector shear 2
                    history.execute( new SetShearZY(*this, 0) );
                    history.execute( new SetShearYX(*this, 0) );
                    history.execute( new SetShearXY(*this, 0) );
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
                    
                case 5: // projector elevation / distance (mod)
                    if (!mod)
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
                    if (!mod)
                        history.execute( new SetScaleY(*this, scaleY + value * .1) );
                    else {
                        history.execute( new SetScale(*this, scaleX + value * .1, scaleY + value * .1) );
                    }
                    break;
                    
                case 10: // projector shear 1
                    if (!mod)
                        history.execute( new SetShearYZ(*this, shearYZ + value * .1) );
                    else
                        history.execute( new SetShearXZ(*this, shearXZ + value * .1) );
                    break;
                    
                case 11: // projector shear 2
                    if (!mod)
                        history.execute( new SetShearZY(*this, shearZY + value * .1) );
                    else
                        history.execute( new SetShearXY(*this, shearXY + value * .1) );
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
                    
                case 5: // projector elevation / distance (mod)
                    if (!mod)
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
                    if (!mod)
                        history.execute( new SetScaleY(*this, scaleY - value * .1) );
                    else
                        history.execute( new SetScale(*this, scaleX - value * .1, scaleY - value * .1) );
                    break;
                    
                case 10: // projector shear 1
                    if (!mod)
                        history.execute( new SetShearYZ(*this, shearYZ - value * .1) );
                    else
                        history.execute( new SetShearXZ(*this, shearXZ - value * .1) );
                    break;
                    
                case 11: // projector shear 2
                    if (!mod)
                        history.execute( new SetShearZY(*this, shearZY - value * .1) );
                    else
                        history.execute( new SetShearXY(*this, shearXY - value * .1) );
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
                    
                case 6: // projector roll / pan (mod)
                    if (!mod)
                        history.execute( new SetRoll(*this, roll - value) );
                    else
                        history.execute( new SetPan(*this, pan - value) );
                    break;
                    
                case 8: // projector lens offset x
                    history.execute( new SetOffsetX(*this, offsetX - value * .1) );
                    break;
                    
                case 9: // projector scale
                    if (!mod)
                        history.execute( new SetScaleX(*this, scaleX - value * .1) );
                    else
                        history.execute( new SetScale(*this, scaleX - value * .1, scaleY - value * .1));
                    break;
                    
                case 10: // projector shear 1
                    history.execute( new SetShearZX(*this, shearZX - value * .1) );
                    break;
                    
                case 11: // projector shear 2
                    history.execute( new SetShearYX(*this, shearYX - value * .1) );
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
                    
                case 6: // projector roll / pan (mod)
                    if (!mod)
                        history.execute( new SetRoll(*this, roll + value) );
                    else
                        history.execute( new SetPan(*this, pan + value) );
                    break;
                    
                case 8: // projector lensOffsetX
                    history.execute( new SetOffsetX(*this, offsetX + value * .1) );
                    break;
                    
                case 9: // projector scale
                    if (!mod)
                        history.execute( new SetScaleX(*this, scaleX + value * .1) );
                    else
                        history.execute( new SetScale(*this, scaleX + value * .1, scaleY - value * .1));
                    break;
                    
                case 10: // projector shear 1
                    history.execute( new SetShearZX(*this, shearZX + value * .1) );
                    break;
                    
                case 11: // projector shear 2
                    history.execute( new SetShearYX(*this, shearYX + value * .1) );
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



// camera transform
void Projector::setCameraTransform(){
    camera.resetTransform();
    camera.roll(roll);
    camera.tilt(tilt);
    camera.pan(pan+azimuth);
    setCameraPosition(ofVec3f(azimuth, elevation, distance));    
}

// camera position
void Projector::setCameraPosition(ofVec3f sph){
    ofVec3f car = sphToCar(sph);
    camera.setPosition(car);
}

float Projector::getAzimuth(){
    return azimuth;
}
void Projector::setAzimuth(float v){
    azimuth = v;
    setCameraTransform();
}

float Projector::getElevation(){
    return elevation;
}
void Projector::setElevation(float v){
    elevation = v;
    setCameraTransform();
}

float Projector::getDistance(){
    return distance;
}
void Projector::setDistance(float v){
    distance = v;
    setCameraTransform();
}


// orientation
float Projector::getRoll(){
    return roll;
}
void Projector::setRoll(float v){
    roll = v;
    setCameraTransform();
}

float Projector::getTilt(){
    return tilt;
}
void Projector::setTilt(float v){
    tilt = v;
    setCameraTransform();
}

float Projector::getPan(){
    return pan;
}
void Projector::setPan(float v){
    pan = v;    
    setCameraTransform();
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


// camera shear
float Projector::getShearXY(){
    return shearXY;
}
void  Projector::setShearXY(float v){
    shearXY = v;
}

float Projector::getShearXZ(){
    return shearXZ;
}
void  Projector::setShearXZ(float v){
    shearXZ = v;
}

float Projector::getShearYX(){
    return shearYX;
}
void  Projector::setShearYX(float v){
    shearYX = v;
}

float Projector::getShearYZ(){
    return shearYZ;
}
void  Projector::setShearYZ(float v){
    shearYZ = v;
}

float Projector::getShearZX(){
    return shearZX;
}
void  Projector::setShearZX(float v){
    shearZX = v;
}

float Projector::getShearZY(){
    return shearZY;
}
void  Projector::setShearZY(float v){
    shearZY = v;
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
