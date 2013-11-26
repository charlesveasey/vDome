#include "projector.h"

void Projector::setup(int i){
    
    index = i;
    
    // defaults
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
    plane.setPosition(width * i + width/2, height/2, 0);    
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

