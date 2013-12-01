#include "projector.h"

void Projector::init(int i){
    
    // defaults
    index = i;

    keyboard = false;
    xmlPrefix = "projectors/projector[";
    
    width = 1024;
    height = 768;
    
    azimuth = 0;
    elevation = 0;
    distance = 5;

    roll = 0;
    tilt = -16;
    pan = 0;
   
    fov = 33;
    
    lensOffsetX = 0;
    lensOffsetY = 0;
    
    brightness = 1;
    contrast = 1;
    saturation = 1;
}


void Projector::setup() {
    
    // convert camera position -> spherical to cartesian
    ofVec3f sph, car;
    sph.set(azimuth, elevation, distance);
    car = sphToCar(sph);
    
    // create camera
    camera.resetTransform();

    camera.setScale(1,-1,1); // legacy oF oddity
    camera.setFov(fov);
    camera.setPosition(car);
    camera.setNearClip(5);
        
    camera.tilt(tilt);
    camera.pan(azimuth + pan);
    camera.roll(roll);
    
    camera.setLensOffset(ofVec2f(lensOffsetX,lensOffsetY));
    
    // create view
    view.setWidth(width);
    view.setHeight(height);
    
    // create fbo
    if (fbo.getWidth() != width || fbo.getHeight() != height) {
        fbo.allocate(width, height, GL_RGBA);
    }
    
    fbo.begin();
    ofClear(255);
    fbo.end();
    fboTexture = fbo.getTextureReference();
    
    // create render plane
    plane.set(width, height);
    plane.setPosition(width * index + width/2, height/2, 0);
    plane.setResolution(50, 50);
}


void Projector::cameraBegin() {
    camera.begin(view);
}
void Projector::cameraEnd() {
     camera.end();
}


void Projector::fboBegin() {
    fbo.begin();
    
}
void Projector::fboEnd() {
    fbo.end();
}

void Projector::fboBind() {
    fboTexture.bind();
}
void Projector::fboUnbind() {
    fboTexture.unbind();
}

void Projector::draw() {
    plane.mapTexCoordsFromTexture(fboTexture);
    plane.setPosition(width * index + width/2, height/2, 0);
    plane.draw();
}

void Projector::drawWireframe() {
    plane.mapTexCoordsFromTexture(fboTexture);    
    plane.setPosition(width * index + width/2, height/2, 0);    
    plane.drawWireframe();
}


// converts spherical to cartesian coordinates
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



void Projector::loadXML(ofXml &xml) {
    
    string pre = xmlPrefix + ofToString(index);
    
    if (xml.exists(pre + "][@azimuth]"))
        azimuth = ofToFloat( xml.getAttribute(pre + "][@azimuth]") );
    if (xml.exists(pre + "][@brightness]"))
        brightness = ofToFloat( xml.getAttribute(pre + "][@brightness]") );
    if (xml.exists(pre + "][@contrast]"))
        contrast = ofToFloat( xml.getAttribute(pre + "][@contrast]") );
    if (xml.exists(pre + "][@distance]"))
        distance = ofToFloat( xml.getAttribute(pre + "][@distance]") );
    if (xml.exists(pre + "][@elevation]"))
        elevation = ofToFloat( xml.getAttribute(pre + "][@elevation]") );
    if (xml.exists(pre + "][@fov]"))
        fov = ofToFloat( xml.getAttribute(pre + "][@fov]") );
    if (xml.exists(pre + "][@height]"))
        height = ofToInt( xml.getAttribute(pre + "][@height]") );
    if (xml.exists(pre + "][@lensOffsetX]"))
        lensOffsetX = ofToFloat( xml.getAttribute(pre + "][@lensOffsetX]") );
    if (xml.exists(pre + "][@lensOffsetY]"))
        lensOffsetY = ofToFloat( xml.getAttribute(pre + "][@lensOffsetY]") );
    if (xml.exists(pre + "][@saturation]"))
        saturation = ofToFloat( xml.getAttribute(pre + "][@saturation]") );
    if (xml.exists(pre + "][@roll]"))
        roll = ofToFloat( xml.getAttribute(pre + "][@roll]") );
    if (xml.exists(pre + "][@tilt]"))
        tilt = ofToFloat( xml.getAttribute(pre + "][@tilt]") );
    if (xml.exists(pre + "][@pan]"))
        pan = ofToFloat( xml.getAttribute(pre + "][@pan]") );
    if (xml.exists(pre + "][@width]"))
        width = ofToInt( xml.getAttribute(pre + "][@width]") );
    
    setup();
}


void Projector::saveXML(ofXml &xml) {
    string pre = xmlPrefix + ofToString(index);
    
    xml.setAttribute(pre + "][@azimuth]", ofToString(azimuth));
    xml.setAttribute(pre + "][@brightness]", ofToString(brightness));
    xml.setAttribute(pre + "][@contrast]", ofToString(contrast));
    xml.setAttribute(pre + "][@distance]", ofToString(distance));
    xml.setAttribute(pre + "][@elevation]", ofToString(elevation));
    xml.setAttribute(pre + "][@fov]", ofToString(fov));
    xml.setAttribute(pre + "][@height]", ofToString(height));
    xml.setAttribute(pre + "][@lensOffsetX]", ofToString(lensOffsetX));
    xml.setAttribute(pre + "][@lensOffsetY]", ofToString(lensOffsetY));
    xml.setAttribute(pre + "][@saturation]", ofToString(saturation));
    xml.setAttribute(pre + "][@roll]", ofToString(roll));
    xml.setAttribute(pre + "][@tilt]", ofToString(tilt));
    xml.setAttribute(pre + "][@pan]", ofToString(pan));
    xml.setAttribute(pre + "][@width]", ofToString(width));
}