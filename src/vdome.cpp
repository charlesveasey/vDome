#include "vdome.h"


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
    projectors[0].keyboard = true;
    
    // projection shader
	shader.load("shaders/vdome.vert", "shaders/vdome.frag");
 
    
    // xml settings
    xmlFile = "settings.xml";
    loadXML(xmlFile);
    
    
    // default show config overlay
    config = false;

}







// FRAME UPDATE

void vdome::update() {
    input.update();
}





int frameCnt= 0;
bool saved = false;

// FRAME DRAW 

void vdome::draw(){
    
    if (config) {
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
        
    
    if (projectors[i].mouse) {
        projectors[i].keystone.draw();
    }
    
    // debug positions
    int px = projectors[i].plane.getX() - 150;
    int py = projectors[i].plane.getY() - 150;
    
    int padx = 25;
    int pady = 25;
    
    // debug background square
    ofFill();
    ofSetHexColor(0x000000);
    ofRect(px, py, 1, 300, 300);
    
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
    ofDrawBitmapString("Projector # " + ofToString(i+1), px+padx, py+pady*2);    
    ofDrawBitmapString("FPS: "+ofToString(ofGetFrameRate(), 2), px+padx, py+pady*3);
    
    
    // 1 = mesh radius
    // 2 = azimuth, elevation, distance
    // 3 = pan, tilt, roll
    // 4 = lensOffsetX, lensOffsetY
    // 5 = fov
    // 6 = brightness
    // 7 = contrast
    // 8 = saturation
    if (projectors[i].keyboard || editMode == 1) {
        ofSetHexColor(0xFFFFFF);
        
        string title;
        string str;
        
        switch (editMode) {
            case 1:
                title = "Dome";
                str = "Radius: " + ofToString(mesh.radius);
                break;
                
            case 2:
                title = "Projector Position";
                str =   "Azimuth: "+ ofToString(projectors[i].azimuth) + "\n" +
                "Elevation: "+ ofToString(projectors[i].elevation) + "\n" +
                "Distance: "+ ofToString(projectors[i].distance);
                break;
                
            case 3:
                title = "Projector Orientation";
                str =   "Roll "+ ofToString(projectors[i].roll) + "\n" +
                "Tilt "+ ofToString(projectors[i].tilt) + "\n" +
                "Pan "+ ofToString(projectors[i].pan);
                break;
                
            case 4:
                title = "Projector Lens";
                str =   "Offset X: "+ ofToString(projectors[i].lensOffsetX) + "\n" +
                "Offset Y "+ ofToString(projectors[i].lensOffsetY);
                break;
                
            case 5:
                title = "Projector Lens";
                str =   "Field of View: "+ ofToString(projectors[i].fov);
                break;
                
            case 6:
                title = "Projector Color";
                str =   "Brightness: "+ ofToString(projectors[i].brightness);
                break;
                
            case 7:
                title = "Projector Color";
                str =   "Contrast: "+ ofToString(projectors[i].contrast);
                break;
                
            case 8:
                title = "Projector Color";
                str =   "Saturation: "+ ofToString(projectors[i].saturation);
                break;
        }
        
        ofDrawBitmapString("Edit #"+ ofToString(editMode), px+padx, py+pady*5);
        ofDrawBitmapString(ofToUpper(title), px+padx, py+pady*6);
        ofDrawBitmapString(str, px+padx, py+pady*7);
    }
    
    tcp.x = px+padx;
    tcp.y = py+pady*10;
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


void vdome::mousePressed(ofMouseEventArgs& mouseArgs) {
    for (int i=0; i<pCount; i++) {
        if (projectors[i].mouse) {
            projectors[i].keystone.onMousePressed(mouseArgs);
        }
    }    
}
void vdome::mouseDragged(ofMouseEventArgs& mouseArgs) {
    for (int i=0; i<pCount; i++) {
        if (projectors[i].mouse) {
            projectors[i].keystone.onMouseDragged(mouseArgs);
        }
    }
}

// KEYBOARD EVENTS

bool shKey = false;
bool altKey = false;
bool ctrKey = false;
bool superKey = false;

float value = 1;
float orgValue = 1;
float shiftValue = .1;
float altValue = .01;

int active = 1;

void vdome::keyPressed(int key){
    cout << "keyPressed " << key << endl;
    
    
    // 1 - 9 = projectors / modes (mod)
    
    // modes
    // 1 = mesh radius
    // 2 = azimuth, elevation, distance
    // 3 = pan, tilt, roll
    // 4 = lensOffsetX, lensOffsetY
    // 5 = fov
    // 6 = brightness
    // 7 = contrast
    // 8 = saturation
    
    if (key >= 49 && key <= 57) {
        
        if (superKey) {
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

        
        return;
    }

    
    // ~ = select all projectors
    if (key == 161) {
        
        for (int i=0; i<pCount; i++) {
            projectors[i].keyboard = true;
        }
        return;
        
    }
    

    // switch others
    
    switch(key){
        
        case 99: // c
            config = !config;
            break;
            
        case 109: // m
            superKey = true;
            break;
            
            
        case OF_KEY_UP: // up
            
            
            switch (editMode) {
                    
                case 1: // mesh radius
                    
                    mesh.radius += value;
                    mesh.setup();
                    
                    break;
                    
                case 2: // projector elevation / distance (super)
                   
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            if (!superKey)
                                projectors[i].elevation += value;
                            else
                                projectors[i].distance += value;
                            
                            projectors[i].setup();

                       }
                    }
                    break;
                    
 
                case 3: // projector tilt
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {

                            projectors[i].tilt += value;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 4: // projector lensOffsetY
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].lensOffsetY += value * .1;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 5: // projector fov
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].fov += value;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 6: // projector brightness
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].brightness += value * .1;
                            
                        }
                    }
                    break;
                    
                case 7: // projector contrast
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].contrast += value * .1;
                            
                        }
                    }
                    break;
                    
                case 8: // projector saturation
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].saturation += value * .1;
                            
                        }
                    }
                    break;
            }
            break;
      
            
            
            
            
        
            
        case OF_KEY_DOWN: // down
            
            switch (editMode) {
                    
                case 1: // mesh radius
                    
                    mesh.radius -= value;
                    mesh.setup();
                    
                    break;
                    
                case 2: // projector elevation / distance (super)
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            if (!superKey)
                                projectors[i].elevation -= value;
                            else
                                projectors[i].distance -= value;
                            
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                    
                case 3: // projector tilt
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].tilt -= value;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 4: // projector lensOffsetY
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].lensOffsetY -= value * .1;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 5: // projector fov
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].fov -= value;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 6: // projector brightness
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].brightness -= value * .1;
                            
                        }
                    }
                    break;
                    
                case 7: // projector contrast
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].contrast -= value * .1;
                            
                        }
                    }
                    break;
                    
                case 8: // projector saturation
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].saturation -= value * .1;
                            
                        }
                    }
                    break;
            }
            break;
            
            
        

       
            
        case OF_KEY_LEFT:  // left
            
            switch (editMode) {
                    
                case 1: // not used
        
                    
                    break;
                    
                case 2: // projector azimuth
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].azimuth -= value;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 3: // projector roll / pan (super)
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            if (!superKey)
                                projectors[i].roll -= value;
                            else
                                projectors[i].pan -= value;
                            
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 4: // projector lensOffsetX
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].lensOffsetX -= value * .1;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
            }
            break;
            
            
            
            
            
       
            
            
        case OF_KEY_RIGHT:  // right

            switch (editMode) {
                    
                case 1: // not used
                    
                    break;
                    
                case 2: // projector azimuth
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].azimuth += value;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
               
                case 3: // projector roll / pan (super)
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            if (!superKey)
                                projectors[i].roll += value;
                            else
                                projectors[i].pan += value;
                            
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
                case 4: // projector lensOffsetX
                    
                    for (int i=0; i<pCount; i++) {
                        if (projectors[i].keyboard) {
                            
                            projectors[i].lensOffsetX += value * .11;
                            projectors[i].setup();
                            
                        }
                    }
                    break;
                    
            }
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
    
}



void vdome::keyReleased(int key){
    //cout << "keyReleased " << key << endl;
    
    switch(key){
            
            
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
        
            
            
        
        case 115: // s  
            if (superKey) { // mod + s = save file
                cout << "saveXML " << endl;
                saveXML(xmlFile);
            }
            break;
            
            
    }
}
