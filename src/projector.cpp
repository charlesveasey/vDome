#include "projector.h"

void Projector::init(int i){
    
    // defaults
    index = i;

    keyboard = false;
    mouse = false;
    xmlPrefix = "projectors/projector[";
    
    width = 1024;
    height = 768;
    
    azimuth = 0;
    elevation = 0;
    distance = 5;

    roll = 0;
    tilt = 0;
    pan = 0;
   
    fov = 33;
    
    lensOffsetX = 0;
    lensOffsetY = 0;


    scale.push_back(1);
    scale.push_back(1);
    
    brightness = 1;
    contrast = 1;
    saturation = 1;
    
    /*
     shear
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

    

    plane.indx = index;
    plane.setup();
}


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
      scale[0],   shear[0],   shear[1],   0,
      shear[2],   scale[1],   shear[3],   0,
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

void Projector::bind() {
    fbo.getTextureReference().bind();
}

void Projector::unbind() {
    fbo.getTextureReference().unbind();
}

void Projector::draw() {
    glEnable(GL_CULL_FACE);
    glCullFace( GL_FRONT );
        plane.draw();
    glDisable(GL_CULL_FACE);
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


// mouse events

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


// keyboard events

void Projector::keyPressed(int key) {
    switch (key) {
        case 161: // ~ = select all projectors
            keyboard = true;
            break;
    }
    
    if (!keyboard) {
        return;
    }
    
    plane.keyPressed(key);
    
    switch (key) {
               
        case OF_KEY_UP:  // up = switch on mode
            switch (editMode) {

                case 1: // projector brightness
                    brightness += value * .1;
                    break;
                    
                case 2: // projector saturation
                    saturation += value * .1;
                    break;
                    
                case 3:
                    // keystone
                    break;
                
                case 4:
                    // grid
                    break;
                    
                case 5: // projector elevation / distance (super)
                    if (!superKey)
                        elevation += value;
                    else
                        distance += value;
                    setup();
                    break;
                    
                case 6: // projector tilt
                        tilt += value;
                        setup();
                    break;
                    
                case 7: // projector fov
                    fov += value;
                    setup();
                    break;
                    
                case 8: // projector lensOffsetY
                        lensOffsetY += value * .1;
                        setup();
                    break;

                case 9: // projector scale
                    if (!superKey)
                        scale[1] += value * .1;
                    else {
                        scale[0] += value * .1;
                        scale[1] += value * .1;
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
                    brightness -= value * .1;
                    break;
                    
                case 2: // projector saturation
                    saturation -= value * .1;
                    break;
                    
                case 3:
                    // keystone
                    break;
                    
                case 4:
                    // grid
                    break;
                    
                case 5: // projector elevation / distance (super)
                    if (!superKey)
                        elevation -= value;
                    else
                        distance -= value;
                    setup();
                    break;
                    
                case 6: // projector tilt
                    tilt -= value;
                    setup();
                    break;
                    
                case 7: // projector fov
                    fov -= value;
                    setup();
                    break;
                    
                case 8: // projector lensOffsetY
                    lensOffsetY -= value * .1;
                    setup();
                    break;
                    
                case 9: // projector scale
                    if (!superKey)
                        scale[1] -= value * .1;
                    else {
                        scale[0] -= value * .1;
                        scale[1] -= value * .1;
                    }
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
                    
                case 2: // projector contrast
                    contrast -= value * .1;
                    break;
                    
                case 5: // projector azimuth
                    azimuth -= value;
                    setup();
                break;
                    
                case 6: // projector roll / pan (super)
                    if (!superKey)
                        roll -= value;
                    else
                        pan -= value;
                    setup();
                    break;
                    
                case 8: // projector lensOffsetX
                    lensOffsetX -= value * .1;
                    setup();
                    break;
                    
                case 9: // projector scale
                    if (!superKey)
                        scale[0] -= value * .1;
                    else {
                        scale[0] -= value * .1;
                        scale[1] -= value * .1;
                    }
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
                    
                case 2: // projector contrast
                    contrast += value * .1;
                    break;
                    
                case 5: // projector azimuth
                    azimuth += value;
                    setup();
                    break;
                    
                case 6: // projector roll / pan (super)
                    if (!superKey)
                        roll += value;
                    else
                        pan += value;
                    setup();
                    break;
                    
                case 8: // projector lensOffsetX
                    lensOffsetX += value * .1;
                    setup();
                    break;
                    
                case 9: // projector scale
                    if (!superKey)
                        scale[0] += value * .1;
                    else {
                        scale[0] += value * .1;
                        scale[1] += value * .1;
                    }
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



void Projector::loadXML(ofXml &xml) {
    string str;
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
    if (xml.exists(pre + "][@scale]")) {
        str = xml.getAttribute(pre + "][@scale]");
        scale[0] = ofToFloat(ofSplitString(str, ",")[0]);
        scale[1] = ofToFloat(ofSplitString(str, ",")[1]);
    }
    
    setup();
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
    xml.setAttribute(pre + "][@lensOffsetX]", ofToString(lensOffsetX));
    xml.setAttribute(pre + "][@lensOffsetY]", ofToString(lensOffsetY));
    xml.setAttribute(pre + "][@saturation]", ofToString(saturation));
    xml.setAttribute(pre + "][@roll]", ofToString(roll));
    xml.setAttribute(pre + "][@tilt]", ofToString(tilt));
    xml.setAttribute(pre + "][@pan]", ofToString(pan));
    xml.setAttribute(pre + "][@width]", ofToString(width));
    xml.setAttribute(pre + "][@scale]", ofToString(scale[0]) +  "," + ofToString(scale[1]) );
}