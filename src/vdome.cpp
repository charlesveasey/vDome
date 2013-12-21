#include "vdome.h"


// keyboard
bool shKey = false;
bool altKey = false;
bool ctrKey = false;
bool superKey = false;

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
    
    editMode = 1;
    
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
    active = 0;
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
        projectors[i].fboBegin();
        ofClear(0, 0, 0, 0);
        projectors[i].cameraBegin();
        input.bind();
        mesh.draw();
        input.unbind();
        projectors[i].cameraEnd();
        projectors[i].fboEnd();
	}

	for(int i=0; i<pCount; i++) {
		projectors[i].fboBind();

		shader.begin();
        shader.setUniform1f("scale", projectors[i].scale);
		shader.setUniform1f("brightness", projectors[i].brightness);
		shader.setUniform1f("contrast", projectors[i].contrast);
		shader.setUniform1f("saturation", projectors[i].saturation);
		shader.setUniformTexture("texsampler", projectors[i].fboTexture, 0);

        projectors[i].draw();
        shader.end();
		
        projectors[i].fboUnbind();
	}
}


void vdome::drawConfig() {
    for(int i=0; i<pCount; i++) {

        if (editGroup == 2 && projectors[i].mouse) {
            if (editMode == 6) {
                projectors[i].keystone.draw();
            }
            if (editMode == 7) {
                projectors[i].grid.drawConfig();
            }
        }
        
        // debug positions
        int px = projectors[i].plane.getX() - 250/2;
        int py = projectors[i].plane.getY() - 250/2;
        
        int padx = 25;
        int pady = 20;
        
        // debug background square
        ofFill();
        
        ofSetHexColor(0x000000);
        ofRect(px, py, 1, 250, 250);
        
        // debug text
        if (saved) {
            ofSetHexColor(0xFFFFFF);
            ofDrawBitmapString("SAVED", px+padx*6, py+pady*2);
            frameCnt++;
            if (frameCnt == 120) {
                saved = false;
                frameCnt = 0;
            }
        }
    
        ofSetHexColor(0xFFFFFF);
        ofDrawBitmapString("Projector #" + ofToString(i+1), px+padx, py+pady*2);    
        ofDrawBitmapString("FPS: "+ofToString(ofGetFrameRate(), 2), px+padx, py+pady*3);
        
        
        // 1 = mesh radius
        // 2 = azimuth, elevation, distance
        // 3 = pan, tilt, roll
        // 4 = lensOffsetX, lensOffsetY
        // 5 = fov
        // 6 = brightness
        // 7 = contrast
        // 8 = saturation
       
        if (1) {
            ofSetHexColor(0xFFFFFF);
            
            string title;
            string sub;
            string str;
            
            switch (editGroup) {
            
                case 1: // mesh
                    title = "Dome Mesh";
                     switch (editMode) 
                        case 1:
                            sub = "";
                    str = "Radius: " + ofToString(mesh.radius);
                        break;
                    break;
                    
                case 2: // projector
                    title = "Projector";                    
                    switch (editMode) {

                        case 1:
                            sub = "Position";
                            str =   "Azimuth: "+ ofToString(projectors[i].azimuth) + "\n" +
                            "Elevation: "+ ofToString(projectors[i].elevation) + "\n" +
                            "Distance: "+ ofToString(projectors[i].distance);
                            break;
                            
                        case 2:
                            sub = "Orientation";
                            str =   "Roll: "+ ofToString(projectors[i].roll) + "\n" +
                            "Tilt: "+ ofToString(projectors[i].tilt) + "\n" +
                            "Pan: "+ ofToString(projectors[i].pan);
                            break;
                            
                        case 3:
                            sub = "Lens";
                            str =   "Offset X: "+ ofToString(projectors[i].lensOffsetX) + "\n" +
                            "Offset Y: "+ ofToString(projectors[i].lensOffsetY);
                            break;
                            
                        case 4:
                            sub = "Lens";
                            str =   "Field of View: "+ ofToString(projectors[i].fov);
                            break;
                         
                        case 5:
                            sub = "Texture";
                            str =   "Scale: "+ ofToString(projectors[i].scale);
                            break;
                            
                        case 6:
                            sub = "Keystone";
                            str =   "";
                            break;
                            
                        case 7:
                            sub = "Grid";
                            str =   "";
                            break;
                            
                        case 8:
                            sub = "Color";
                            str =   "Brightness: "+ ofToString(projectors[i].brightness);
                            break;
                            
                        case 9:
                            sub = "Color";
                            str =   "Contrast: "+ ofToString(projectors[i].contrast);
                            break;
                            
                        case 10:
                            sub = "Color";
                            str =   "Saturation: "+ ofToString(projectors[i].saturation);
                            break;
                            
                    }
                    
                    break;
            }
            
            if (i == active || editGroup == 1) {
                ofDrawBitmapString("Active", px+padx+145, py+pady);
            }
            
            ofDrawBitmapString(ofToUpper(title), px+padx, py+pady*6);
            ofDrawBitmapString(ofToString(editMode) + ": " + ofToUpper(sub), px+padx, py+pady*7);
            ofDrawBitmapString(str, px+padx, py+pady*8);
        }
    
        tcp.x = px+padx;
        tcp.y = py+pady*4;
        tcp.draw();
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
    
    for(int i=0; i<pCount; i++) {
		projectors[i].saveXML(xml);
	}
    
    if (xml.save(file)) {
        saved = true;
    }
}

// MOUSE EVENTS

void vdome::mousePressed(ofMouseEventArgs& mouseArgs) {
    if (!config) {
        return;
    }
    for (int i=0; i<pCount; i++) {
        projectors[i].mousePressed(mouseArgs);
    }    
}
void vdome::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (!config) {
        return;
    }
    for (int i=0; i<pCount; i++) {
        projectors[i].mouseDragged(mouseArgs);
    }
}
void vdome::mouseReleased(ofMouseEventArgs& mouseArgs) {
    if (!config) {
        return;
    }
    for (int i=0; i<pCount; i++) {
        projectors[i].mouseReleased(mouseArgs);
    }
}


// KEYBOARD EVENTS

void vdome::keyPressed(int key){
    cout << "keyPressed " << key << endl;
    
    switch(key){
        case 99: // c
            config = !config;
            break;
            
        case 104: // h
            showConfig = !showConfig;
            if (showConfig) {
                config = true;
            }
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
                projectors[i].value = value;
                projectors[i].keyPressed(key);
            }
            break;
    }
    

    
    // 1 - 9 = projectors
        
    // 1 - 9 + m = projectors modes
    // 1 = mesh radius
    // 2 = azimuth, elevation, distance
    // 3 = pan, tilt, roll
    // 4 = lensOffsetX, lensOffsetY
    // 5 = fov
    // 6 = brightness
    // 7 = contrast
    // 8 = saturation
    
    if (key >= 49 && key <= 57) {
        
        // assign edit mode
        if (altKey) {
            editMode = key-48;
            return;
        }
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
    }
}



void vdome::keyReleased(int key) {
    if (!config) {
        return;
    }
    
    //cout << "keyReleased " << key << endl;
    
    for (int i=0; i<pCount; i++) {
        projectors[i].keyReleased(key);
    }
    
    mesh.keyReleased(key);
    
    switch(key){
            
        case 109: // m
            superKey = false;
            break;
        
        case 115: // s
            if (superKey) { // mod + s = save file
                cout << "saveXML " << endl;
                saveXML(xmlFile);
            }
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
            
        case false:
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
