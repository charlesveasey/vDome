#include "vdome.h"

/********************************************
    
    MENU
 
 
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

 ********************************************/


/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

vdome::vdome() {
    // add values
    value = 1;
    orgValue = 1;
    shiftValue = .1;
    altValue = .01;
    
    // config draw
    frameCnt= 0;
    saved = false;
    
}

/******************************************
 
 SETUP
 
 ********************************************/

void vdome::setup(){
    
    ofSetEscapeQuitsApp(true);
    
    window.setup();
    render.setup();
    dome.setup();
    
    // input
    // 0 = image
	// 1 = capture
    // 2 = video
    // 3 = hap
    // 4 = syphon
    
    input.mode = 0;
    input.frameRate = render.getFrameRate();
    input.setup();
    
    // projectors
    pCount = 2; // FIXME: not dynamic with xml
    pActive = 1;
    
    for(int i=0; i<pCount; i++) {
        Projector p;
        p.init(i);
        projectors.push_back(p);
    }
    
    // projection shader
	shader.load("shaders/vdome.vert", "shaders/vdome.frag");
    
    
    // config
    config = false;
    showConfig = false;
    showFrameRate = true;
    editMode = 1;
    editGroup = 2;
    
    // xml settings
    xmlFile = "settings.xml";
    loadXML(xmlFile);
}




/******************************************
 
 UPDATE
 
 ********************************************/

void vdome::update() {
    input.update();
}




/******************************************
 
 DRAW
 
 ********************************************/

void vdome::draw(){
    if (showConfig) {
        drawConfig();
    }
        
    ofSetHexColor(0xFFFFFF);
    
	for(int i=0; i<pCount; i++){
        projectors[i].begin();
        input.bind();
        dome.draw();
        input.unbind();
        projectors[i].end();
	}

	for(int i=0; i<pCount; i++) {
		projectors[i].bind();
		shader.begin();
		shader.setUniform1f("brightness", projectors[i].brightness );
		shader.setUniform1f("contrast", projectors[i].contrast );
		shader.setUniform1f("saturation", projectors[i].saturation );
		shader.setUniformTexture("texsampler", projectors[i].getTextureReference(), 0);
        projectors[i].draw();
        shader.end();
        projectors[i].unbind();
	}
}




/******************************************
 
 DRAW CONFIG
 
 ********************************************/

void vdome::drawConfig() {
    
    for(int i=0; i<pCount; i++) {
        if (editGroup == 2 && projectors[i].mouse) {
            if (editMode == 3) {
                projectors[i].drawKeystone();
            }
            else if (editMode == 4) {
                projectors[i].drawPlaneConfig();
            }
        }
        
        // debug positions
        int pw = 200;
        int ph = 135;
        
        int px = projectors[i].getPlanePosition().x + 1024/2 - pw/2;
        int py = projectors[i].getPlanePosition().y + 768/2  - ph/2;
        
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
       
        ofSetHexColor(0xFFFFFF);
        
        string title;
        string sub;
        string str;
        
        switch (editGroup) {
      
            case 1: // dome mesh
                title = "Edit: Dome";
                        sub = "";             
                        str = "Radius: " + ofToString( roundTo(dome.radius, .01) );
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
                        str =   "Azimuth: "+ ofToString( roundTo(projectors[i].getCameraPosition().x, .01) ) + "\n" +
                                "Elevation: "+ ofToString( roundTo(projectors[i].getCameraPosition().y, .01) ) + "\n" +
                                "Distance: "+ ofToString( roundTo(projectors[i].getCameraPosition().z, .01) );
                        break;
                        
                    case 6:
                        sub =   "Orientation";
                        str =   "Roll: "+ ofToString( roundTo(projectors[i].getCameraOrientation().x, .01) ) + "\n" +
                                "Tilt: "+ ofToString( roundTo(projectors[i].getCameraOrientation().y, .01) ) + "\n" +
                                "Pan: "+ ofToString( roundTo(projectors[i].getCameraOrientation().z, .01) );
                        break;
                        
                    case 7:
                        sub = "Lens";
                        str =   "Field of View: "+ ofToString( roundTo(projectors[i].getCameraFov(), .01) );
                        break;
                        
                    case 8:
                        sub = "Offset";
                        str =   "X: "+ ofToString( roundTo(projectors[i].getCameraOffset().x, .001) ) + "\n" +
                                "Y: "+ ofToString( roundTo(projectors[i].getCameraOffset().y, .001 ) );
                        break;
                     
                    case 9:
                        sub = "Scale";
                        str = "X: "+ ofToString( roundTo(projectors[i].getCameraScale().x, .01) ) + "\n" +
                              "Y: "+ ofToString( roundTo(projectors[i].getCameraScale().y, .01) );
                        break;
                        
                    case 10:
                        sub = "Shear 1";
                        str =   "YZ: "+ ofToString( roundTo(projectors[i].getCameraShear()[3], .001) ) + "\n" +
                                "ZX: "+ ofToString( roundTo(projectors[i].getCameraShear()[4], .001) ) + "\n" +
                                "XZ: "+ ofToString( roundTo(projectors[i].getCameraShear()[1], .001) );
                        break;
                        
                    case 11:
                        sub = "Shear 2";
                        str =   "ZY: "+ ofToString( roundTo(projectors[i].getCameraShear()[5], .001) ) + "\n" +
                                "YX: "+ ofToString( roundTo(projectors[i].getCameraShear()[2], .001) ) + "\n" +
                                "XY: "+ ofToString( roundTo(projectors[i].getCameraShear()[0], .001) );
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
						if (input.mode == 1)		sub = "Capture";
						else if (input.mode == 2)   sub = "Video";
						else if (input.mode == 3)   sub = "Hap";
						else if (input.mode == 4)   sub = "Syphon";
						else						sub = "Image"; //  mode = 0
						str = "";
                        break;
                    case 2:
                        sub = "";
                        str = "Scale: " + ofToString( roundTo(dome.textureScale, .01) );
                        break;
                    case 3:
                        sub = "Offset";
                        str =   "X: \nY:";
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

/******************************************
 
 SETTINGS

 *******************************************/

void vdome::loadXML(string file) {
    if (xml.load(file)) {
        if (xml.exists("projectors[@count]"))
            pCount = ofToInt( xml.getAttribute("projectors[@count]") );
        
        input.loadXML(xml);
        render.loadXML(xml);
        window.loadXML(xml);
        dome.loadXML(xml);
        
        for(int i=0; i<pCount; i++) {
            projectors[i].loadXML(xml);
        }
    }
}

void vdome::saveXML(string file) {
    xml.setAttribute("projectors[@count]", ofToString(pCount));
    
    tcp.saveXML(xml);
    input.saveXML(xml);
    render.saveXML(xml);
    window.saveXML(xml);
    dome.saveXML(xml);
    
    for(int i=0; i<pCount; i++) {
		projectors[i].saveXML(xml);
	}
    
    if (xml.save(file)) {
        saved = true;
    }
}

/******************************************
 
 MOUSE
 
 ********************************************/

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

/******************************************
 
 KEYBOARD
 
 ********************************************/

bool all;
void vdome::keyPressed(int key){
    
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
            alt = true;
            break;
            
        case OF_KEY_RIGHT_ALT:
            value = altValue;
            alt = true;
            break;
            
        case OF_KEY_LEFT_CONTROL: // control
            ctrl = true;
            mod = true;
            break;
            
        case OF_KEY_RIGHT_CONTROL:
            ctrl = true;
            mod = true;
            break;
            
        case OF_KEY_LEFT_SHIFT: // shift
            value = shiftValue;
            shift = true;
            break;
            
        case OF_KEY_RIGHT_SHIFT:
            value = shiftValue;
            shift = true;
            break;
            
        case OF_KEY_LEFT_SUPER: // super
            mod = true;
            ctrl = true;
            break;
            
        case OF_KEY_RIGHT_SUPER:
            mod = true;
            ctrl = true;
            break;    
    }
    
    for (int i=0; i<pCount; i++) {
        projectors[i].setValue(value);
    }
    
    if (!config) {
        return;
    }

    if (key == 96) { // ~ = de/select all projectors
        all = !all;
    }
    
    if (all) {
        for (int i=0; i<pCount; i++) {
            projectors[i].keyboard = true;
            projectors[i].mouse = true;
            
        }
    }
    
    if (key >= 48 && key <= 57) {
        // assign edit mode
        if (alt) {
            if (key == 48)
                editMode = 10;
            else
                editMode = key-48;
            
            if (editGroup == 2) {
                for (int i=0; i<pCount; i++) {
                    if (editMode == 3) {
                        projectors[i].setKeystoneActive(true);
                        projectors[i].setGridActive(false);
                    }
                    else if (editMode == 4) {
                        projectors[i].setKeystoneActive(false);
                        projectors[i].setGridActive(true);
                    }
                    else {
                        projectors[i].setKeystoneActive(false);
                        projectors[i].setGridActive(false);
                    }
                }                
            }
            return;
        }
        if (key == 48)
            pActive = 10;
        else
            pActive = key-49;
        
        // shift groups, otherwise reset
        if (!shift) {
            for (int i=0; i<pCount; i++) {
                projectors[i].keyboard = false;
                projectors[i].mouse = false;
            }
            projectors[pActive].keyboard = true;
            projectors[pActive].mouse = true;
        }
        else {
            projectors[pActive].keyboard = !(projectors[pActive].keyboard);
            projectors[pActive].mouse = !(projectors[pActive].mouse);
        }
        
            
        
        if (editGroup == 2) {
            for (int i=0; i<pCount; i++) {
                if (editMode == 3) {
                    projectors[i].setKeystoneActive(true);
                    projectors[i].setGridActive(false);
                }
                else if (editMode == 4) {
                    projectors[i].setKeystoneActive(false);
                    projectors[i].setGridActive(true);
                }
                else {
                    projectors[i].setKeystoneActive(false);
                    projectors[i].setGridActive(false);
                }
            }
            
            // move mouse to new selection
            if (editMode == 3 || editMode == 4) {
                glutWarpPointer(projectors[pActive].getPlanePosition().x+projectors[pActive].getPlaneDimensions().x/2,
                               -(projectors[pActive].getPlanePosition().y+projectors[pActive].getPlaneDimensions().y/2));
            }
        }
        return;
    }
    else if (key == 45) {
        editMode = 11;
    }
    
    
    switch (editGroup) {
        case 1: // d = dome mesh
            dome.editMode = editMode;
            dome.value = value;
            dome.keyPressed(key);
            break;
        case 2: // p = projector
            for (int i=0; i<pCount; i++) {
                projectors[i].editMode = editMode;
                projectors[i].mod = mod;
                projectors[i].keyPressed(key);
            }
            break;
        case 3: // i = input
			if (editMode == 1)
				input.keyPressed(key);
			else {
				dome.editMode = editMode;
				dome.value = value;
				dome.keyPressedInput(key);
			}
            break;
    }
    
}

void vdome::keyReleased(int key) {
    
    for (int i=0; i<pCount; i++) {
        projectors[i].keyReleased(key);
    }
        
    switch(key){
            
        case 115: // s
            if (config)  { // mod + s = save file
                saveXML(xmlFile);
                cout << "saveXML " << endl;
            }
            break;
         
        case OF_KEY_LEFT_ALT: // alt
            value = orgValue;
            alt = false;
            break;
            
        case OF_KEY_RIGHT_ALT:
            value = orgValue;
            alt = false;
            break;
            
        case OF_KEY_LEFT_CONTROL: // control
            ctrl = false;
            mod = false;
            break;
        case OF_KEY_RIGHT_CONTROL:
            ctrl = false;
            mod = false;
            break;
            
        case OF_KEY_LEFT_SHIFT: // shift
            value = orgValue;
            shift = false;
            break;
            
        case OF_KEY_RIGHT_SHIFT:
            value = orgValue;
            shift = false;
            break;
            
        case OF_KEY_LEFT_SUPER:  // super
            mod = false;
            ctrl = false;
            break;
            
        case OF_KEY_RIGHT_SUPER:
            mod = false;
            ctrl = false;
            break;
    }
    
    for (int i=0; i<pCount; i++) {
        projectors[i].setValue(value);
    }
}

/******************************************
 
 MATH
 
 ********************************************/

float round(float d) {
  return floorf(d + 0.5);
}

float vdome::roundTo(float val, float n){
	return round(val * 1/n) * n;
}