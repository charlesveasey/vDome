#include "vdome.h"

/*********************
 *********************
 
    MENU
    ----
 
    (key) Edit
    ----------------------
    (alt + key) Mode
 
 
    (i) Input
    --------------
    (1) Scale


    (d) Dome
    ---------------
    (1) Radius


    (p) Projector
    ---------------
    (1) Intensity
    (2) Color
    (3) Keystone
    (4) Grid
    (5) Position
    (6) Orientation
    (7) Field of View
    (8) Offset
    (9) Scale
    (10) Shear 1
    (11) Shear 2

 *********************
 *********************/



// keyboard
bool shKey = false;
bool altKey = false;
bool ctrKey = false;
bool superKey = false;

// add values
float value = 1;
float orgValue = 1;
float shiftValue = .1;
float altValue = .01;

// config draw
int frameCnt= 0;
bool saved = false;



// SETUP

void vdome::setup(){
    
    // tcp server
    tcp.init();
    tcp.setup();
    
    // window
    window.init();
    window.setup();
    
    // render
    render.init();
    render.setup();
        
    // input
    // 0 = image
    // 1 = video
    // 2 = capture
    // 3 = hap
    // 4 = syphon
    
    input.mode = 0;
    input.render = render;    
    input.init();
    input.setup();
    
    // dome mesh
    mesh.init();
	mesh.setup();
    
    // projectors
    pCount = 2;
    
    for(int i=0; i<pCount; i++) {
        Projector p;
        p.init(i);
        p.setup();
        projectors.push_back(p);
    }
    
    // projection shader
	shader.load("shaders/vdome.vert", "shaders/vdome.frag");
 
    // xml settings
    xmlFile = "settings.xml";
    loadXML(xmlFile);    
    
    // default config settings
    config = false;
    showConfig = false;
    active = 1;
    editMode = 1;
    editGroup = 2;
    showFrameRate = true;
}


// FRAME UPDATE

void vdome::update() {
    input.update();
}


// FRAME DRAW 

void vdome::draw(){
    if (showConfig) {
        drawConfig();
    }
        
    ofSetHexColor(0xFFFFFF);
    
	for(int i=0; i<pCount; i++){
        projectors[i].begin();
        input.bind();
        mesh.draw();
        input.unbind();
        projectors[i].end();
	}

	for(int i=0; i<pCount; i++) {
		projectors[i].bind();
		shader.begin();
		shader.setUniform1f("brightness", projectors[i].brightness);
		shader.setUniform1f("contrast", projectors[i].contrast);
		shader.setUniform1f("saturation", projectors[i].saturation);
		shader.setUniformTexture("texsampler", projectors[i].fbo.getTextureReference(), 0);
        projectors[i].draw();
        shader.end();
        projectors[i].unbind();
	}
}


// FRAME DRAW CONFIG

void vdome::drawConfig() {
    for(int i=0; i<pCount; i++) {

        if (editGroup == 2 && projectors[i].mouse) {
            if (editMode == 3) {
                projectors[i].plane.keystone.draw();
            }
            else if (editMode == 4) {
                projectors[i].plane.drawConfig();
            }
        }
        
        // debug positions
        int pw = 200;
        int ph = 135;
        
        int px = projectors[i].plane.position[0]+1024/2 - pw/2;
        int py = projectors[i].plane.position[1]+768/2 - ph/2;
        
        int padx = 15;
        int pady = 15;

        // debug text
        if (saved) {
            ofSetHexColor(0xFFFFFF);
            ofDrawBitmapString("SAVED", px+padx+125, py+pady*1.75);
            frameCnt++;
            if (frameCnt == 60) {
                saved = false;
                frameCnt = 0;
            }
             ofSetHexColor(0xAA0000);
        }
        else {
            ofSetHexColor(0x000000);
        }
        
        // debug background square
        ofFill();
        ofRect(px, py, 1, pw, ph);
        
        ofSetHexColor(0xFFFFFF);
        ofDrawBitmapString("Projector #" + ofToString(i+1), px+padx, py+pady*1.75);
        //ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), px+padx, py+pady*2.75);
    
        //tcp.x = px+padx;
        //tcp.y = py+pady*4.5;
        //tcp.draw();
        
        // MESH
        // 1 = mesh radius
        
        // PROJECTOR
        // 1 = azimuth, elevation, distance
        // 2 = pan, tilt, roll
        // 3 = lensOffsetX, lensOffsetY
        // 4 = fov
        // 5 = texture size
        // 6 = keystone
        // 7 = grid
        // 8 = brightness
        // 9 = contrast
        // 10 = saturation
       
        ofSetHexColor(0xFFFFFF);
        
        string title;
        string sub;
        string str;
        
        switch (editGroup) {
      
            case 1: // mesh
                title = "Edit: Dome";
                 switch (editMode) 
                    case 1:
                        sub = "";             
                        str = "Radius: " + ofToString( roundTo(mesh.radius, .01) );
                    break;
                    default:
                        sub = "";
                        str = "";
                    break;
                break;
                
            case 2: // projector
                title = "Edit: Projector";                    
                switch (editMode) {
                    case 1:
                        sub = "Intensity";
                        str =   "Brightness: " + ofToString( roundTo(projectors[i].brightness, .001) ) + "\n" +
                        "Contrast: " + ofToString( roundTo(projectors[i].contrast, .001) );
                        break;
                        
                    case 2:
                        sub = "Color";
                        str =   "Saturation: " + ofToString( roundTo(projectors[i].saturation, .001) );
                        break;
                        
                    case 3:
                        sub = "Keystone";
                        str = "";
                        break;
                        
                    case 4:
                        sub = "Grid";
                        str = "";
                        break;
                        
                    case 5:
                        sub = "Position";
                        str =   "Azimuth: "+ ofToString( roundTo(projectors[i].azimuth, .01) ) + "\n" +
                        "Elevation: "+ ofToString( roundTo(projectors[i].elevation, .01) ) + "\n" +
                        "Distance: "+ ofToString( roundTo(projectors[i].distance, .01) );
                        break;
                        
                    case 6:
                        sub = "Orientation";
                        str =   "Roll: "+ ofToString( roundTo(projectors[i].roll, .01) ) + "\n" +
                        "Tilt: "+ ofToString( roundTo(projectors[i].tilt, .01) ) + "\n" +
                        "Pan: "+ ofToString( roundTo(projectors[i].pan, .01) );
                        break;
                        
                    case 7:
                        sub = "Lens";
                        str =   "Field of View: "+ ofToString( roundTo(projectors[i].fov, .01) );
                        break;
                        
                    case 8:
                        sub = "Offset";
                        str =   "X: "+ ofToString( roundTo(projectors[i].lensOffsetX, .001) ) + "\n" +
                                "Y: "+ ofToString( roundTo(projectors[i].lensOffsetY, .001 ) );
                        break;
                     
                    case 9:
                        sub = "Scale";
                        str = "X: "+ ofToString( roundTo(projectors[i].scale[0], .01) ) + "\n" +
                              "Y: "+ ofToString( roundTo(projectors[i].scale[1], .01) );
                        break;
                        
                    case 10:
                        sub = "Shear 1";
                        str =   "YX: "+ ofToString( roundTo(projectors[i].shear[3], .001) ) + "\n" +
                        "ZX: "+ ofToString( roundTo(projectors[i].shear[4], .001) ) + "\n" +
                        "XZ: "+ ofToString( roundTo(projectors[i].shear[1], .001) );
                        break;
                        
                    case 11:
                        sub = "Shear 2";
                        str =   "ZY: "+ ofToString( roundTo(projectors[i].shear[5], .001) ) + "\n" +
                        "YX: "+ ofToString( roundTo(projectors[i].shear[2], .001) ) + "\n" +
                        "XY: "+ ofToString( roundTo(projectors[i].shear[0], .001) );
                        break;
                        
                    default:
                        sub = "";
                        str = "";
                        break;
                }
                break;
                
            case 3: // input
                title = "Edit: Input";
                switch (editMode) {
                    case 1:
                        sub = "";
                        str = "Scale: " + ofToString( roundTo(mesh.textureScale, .01) );
                        break;
                    case 2:
                        sub = "Offset";
                        str =   "X: \nY:";
                        break;
                    break;
                    default:
                        sub = "";
                        str = "";
                        break;
                }
            break;
        }
    
        if (editGroup == 2 && config && !saved) {
            if (projectors[i].keyboard || projectors[i].mouse) {
                ofSetHexColor(0xFFF000);
                ofDrawBitmapString("Active", px+padx+120, py+pady*1.75);
                ofSetHexColor(0xFFFFFF);
            }
            else if (showFrameRate) {
                ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), px+padx+125, py+pady*1.75);
            }
        }
        else if (showFrameRate && !saved)  {
            ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), px+padx+125, py+pady*1.75);
        }
        
        ofDrawBitmapString(title, px+padx, py+pady*3.5);
        ofDrawBitmapString("Mode: " + sub, px+padx, py+pady*4.5);
        ofDrawBitmapString(str, px+padx, py+pady*6);
    }
}




// XML SETTINGS

void vdome::loadXML(string file) {
    xml.load(file);
    
    if (xml.exists("projectors[@count]"))
        pCount = ofToInt( xml.getAttribute("projectors[@count]") );
    
    input.loadXML(xml);
    render.loadXML(xml);
    window.loadXML(xml);
    mesh.loadXML(xml);
    
    for(int i=0; i<pCount; i++) {
		projectors[i].loadXML(xml);
	}
}


void vdome::saveXML(string file) {
    xml.setAttribute("projectors[@count]", ofToString(pCount));
    
    tcp.saveXML(xml);
    input.saveXML(xml);
    render.saveXML(xml);
    window.saveXML(xml);
    mesh.saveXML(xml);
    mesh.saveMesh("models/dome-mesh.ply");
    
    for(int i=0; i<pCount; i++) {
		projectors[i].saveXML(xml);
	}
    
    if (xml.save(file)) {
        saved = true;
    }
}


// MOUSE EVENTS

void vdome::mousePressed(ofMouseEventArgs& mouseArgs) {
    if (config) {
        for (int i=0; i<pCount; i++) {
            projectors[i].mousePressed(mouseArgs);
        }
    }
}

void vdome::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (config) {
        for (int i=0; i<pCount; i++) {
            projectors[i].mouseDragged(mouseArgs);
        }
    }
}

void vdome::mouseReleased(ofMouseEventArgs& mouseArgs) {
    if (config) {
        for (int i=0; i<pCount; i++) {
            projectors[i].mouseReleased(mouseArgs);
        }
    }
}


// KEYBOARD EVENTS

void vdome::keyPressed(int key){
    //cout << "keyPressed " << key << endl;
    
    switch(key){
         
        case 102: // f
            showFrameRate = !showFrameRate;
            break;
            
        case 99: // c
            config = !config;
            break;
            
        case 104: // h
            showConfig = !showConfig;
            if (showConfig) {
                config = true;
            }
            break;
            
        case 105: // i
            editGroup = 3;
            break;
            
        case 100: // d = edit group
            editGroup = 1;
            break;
            
        case 112: // p = edit group
            editGroup = 2;
            break;
            
        case OF_KEY_LEFT_ALT: // alt
            value = altValue;
            altKey = true;
            break;
            
        case OF_KEY_RIGHT_ALT:
            value = altValue;
            altKey = true;
            break;
            
        case OF_KEY_LEFT_CONTROL: // control
            ctrKey = true;
            break;
            
        case OF_KEY_RIGHT_CONTROL:
            ctrKey = true;
            break;
            
        case OF_KEY_LEFT_SHIFT: // shift
            value = shiftValue;
            shKey = true;
            break;
            
        case OF_KEY_RIGHT_SHIFT:
            value = shiftValue;
            shKey = true;
            break;
            
        case OF_KEY_LEFT_SUPER: // super
            superKey = true;
            break;
            
        case OF_KEY_RIGHT_SUPER:
            superKey = true;
            break;    
    }    
    
    if (!config) {
        return;
    }
    
    // 1 - 9 = projectors
        
    // 1 - 9 + alt = projectors modes
    // 1 = mesh radius
    // 2 = azimuth, elevation, distance
    // 3 = pan, tilt, roll
    // 4 = lensOffsetX, lensOffsetY
    // 5 = fov
    // 6 = brightness
    // 7 = contrast
    // 8 = saturation

    
    if (key >= 48 && key <= 57) {
        // assign edit mode
        if (altKey) {
            if (key == 48)
                editMode = 10;
            else
                editMode = key-48;
            
            
            if (editGroup == 2) {
                for (int i=0; i<pCount; i++) {
                    if (editMode == 3) {
                        projectors[i].plane.keystoneActive = true;
                        projectors[i].plane.gridActive = false;
                    }
                    else if (editMode == 4) {
                        projectors[i].plane.keystoneActive = false;
                        projectors[i].plane.gridActive = true;
                    }
                    else {
                        projectors[i].plane.keystoneActive = false;
                        projectors[i].plane.gridActive = false;
                    }
                }
            }
            
            return;
        }
        if (key == 48)
            active = 10;
        else
            active = key-49;
        
        // shift groups, otherwise reset
        if (!shKey) {
            for (int i=0; i<pCount; i++) {
                projectors[i].keyboard = false;
                projectors[i].mouse = false;
            }
            projectors[active].keyboard = true;
            projectors[active].mouse = true;
        }
        else {
            projectors[active].keyboard = !(projectors[active].keyboard);
            projectors[active].mouse = !(projectors[active].mouse);
        }
        
        if (editGroup == 2) {
            for (int i=0; i<pCount; i++) {
                if (editMode == 3) {
                    projectors[i].plane.keystoneActive = true;
                    projectors[i].plane.gridActive = false;
                }
                else if (editMode == 4) {
                    projectors[i].plane.keystoneActive = false;
                    projectors[i].plane.gridActive = true;
                }
                else {
                    projectors[i].plane.keystoneActive = false;
                    projectors[i].plane.gridActive = false;
                }
            }
        }
        
        return;
    }
    else if (key == 45) {
        editMode = 11;
    }
    
    
    switch (editGroup) {
        case 1: // d = dome mesh
            mesh.editMode = editMode;
            mesh.value = value;
            mesh.keyPressed(key);
            break;
        case 2: // p = projector
            for (int i=0; i<pCount; i++) {
                projectors[i].editMode = editMode;
                projectors[i].superKey = superKey;
                projectors[i].superKey = superKey;
                projectors[i].value = value;
                projectors[i].keyPressed(key);
            }
            break;
        case 3: // i = input
            mesh.editMode = editMode;
            mesh.value = value;
            mesh.keyPressedInput(key);
            break;
    }
    
}



void vdome::keyReleased(int key) {
    
    //cout << "keyReleased " << key << endl;
    
    for (int i=0; i<pCount; i++) {
        projectors[i].keyReleased(key);
    }
    
    mesh.keyReleased(key);
    
    switch(key){
            
        case 115: // s
            if (config)  { // mod + s = save file
                cout << "saveXML " << endl;
                saveXML(xmlFile);
            }
            break;
            
        case 109: // m
            superKey = false;
            break;
         
        case OF_KEY_LEFT_ALT: // alt
            value = orgValue;
            altKey = false;
            break;
            
        case OF_KEY_RIGHT_ALT:
            value = orgValue;
            altKey = false;
            break;
            
        case OF_KEY_LEFT_CONTROL: // control
            ctrKey = false;
            break;
        case OF_KEY_RIGHT_CONTROL:
            ctrKey = false;
            break;
            
        case OF_KEY_LEFT_SHIFT: // shift
            value = orgValue;
            shKey = false;
            break;
            
        case OF_KEY_RIGHT_SHIFT:
            value = orgValue;
            shKey = false;
            break;
            
        case OF_KEY_LEFT_SUPER:  // super
            superKey = false;
            break;
            
        case OF_KEY_RIGHT_SUPER:
            superKey = false;
            break;
    }
}


float vdome::roundTo(float val, float n){
    return round(val * 1/n) * n;
}
