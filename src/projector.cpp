#include "projector.h"

void Projector::init(){
    
    // defaults
    index = 0;

    width = 1024;
    height = 768;
    
    azimuth = 0;
    elevation = 0;
    distance = 5;
   
    tilt = -16;
    
    fov = 45;
    
    lensOffsetX = 0;
    lensOffsetY = .31;
    
    brightness = 1;
    contrast = 1;
    saturation = 1;
}


void Projector::setup(int i) {
    index = i;
    
    // convert camera position -> spherical to cartesian
    ofVec3f sph, car;
    sph.set(azimuth, elevation, distance);
    car = sphToCar(sph);
    
    // create camera
    camera.setScale(1,-1,1); // legacy oF oddity
    camera.setFov(fov);
    camera.setPosition(car);
    camera.rotate(azimuth, 0, 1, 0);
    camera.tilt(tilt);
    camera.setLensOffset(ofVec2f(lensOffsetX,lensOffsetY));
    
    // create view
    view.setWidth(width);
    view.setHeight(height);
    
    // create fbo
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255);
    fbo.end();
    fboTexture = fbo.getTextureReference();
    
    // create render plane
    plane.set(width, height);
    plane.setPosition(width * index + width/2, height/2, 0);
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
    if (xml.exists("projectors/projector["+ofToString(index)+"][@azimuth]"))
        azimuth = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@azimuth]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@brightness]"))
        brightness = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@brightness]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@contrast]"))
        contrast = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@contrast]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@distance]"))
        distance = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@distance]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@elevation]"))
        elevation = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@elevation]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@fov]"))
        fov = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@fov]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@height]"))
        height = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@height]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@lensOffsetX]"))
        lensOffsetX = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@lensOffsetX]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@lensOffsetY]"))
        lensOffsetY = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@lensOffsetY]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@saturation]"))
        saturation = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@saturation]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@tilt]"))
        tilt = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@tilt]") );
    if (xml.exists("projectors/projector["+ofToString(index)+"][@width]"))
        width = ofToInt( xml.getAttribute("projectors/projector["+ofToString(index)+"][@width]") );
    setup(index);
}


void Projector::saveXML(ofXml &xml) {
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@azimuth]", ofToString(azimuth));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@brightness]", ofToString(brightness));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@contrast]", ofToString(contrast));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@distance]", ofToString(distance));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@elevation]", ofToString(elevation));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@fov]", ofToString(fov));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@height]", ofToString(height));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@lensOffsetX]", ofToString(lensOffsetX));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@lensOffsetY]", ofToString(lensOffsetY));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@saturation]", ofToString(saturation));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@tilt]", ofToString(tilt));
    xml.setAttribute("projectors/projector["+ofToString(index)+"][@width]", ofToString(width));
}