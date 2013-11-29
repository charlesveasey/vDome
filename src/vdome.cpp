#include "vdome.h"


// SETUP

void vdome::setup(){
    
    // window
    window.init();
    window.setup();
    
    // render
    render.init();
    render.setup();
    
    
    // input
    // 0 = image
    // 1 = video
    // 2 = hap
    // 3 = syphon
    // 4 = capture
    
    input.mode = 0;
    input.render = render;    
    input.init();
    input.setup();

    
    // dome mesh
    mesh.init();
	mesh.setup();
    
    
    // projectors
    pCount = 3;
    
    for(int i=0; i<pCount; i++) {
        Projector p;
        p.init();
        p.setup(i);
        projectors.push_back(p);
    }
    
    
    // projection shader
	shader.load("shaders/vdome.vert", "shaders/vdome.frag");
 
    
    // xml settings
    xmlFile = "settings.xml";
    loadXML(xmlFile);
}







// FRAME UPDATE

void vdome::update() {
    input.update();
}






// FRAME DRAW 

void vdome::draw(){
	ofSetColor(255); 

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

    ofSetHexColor(0xFFFFFF);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
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
}


void vdome::saveXML(string file) {
    xml.setAttribute("projectors[@count]", ofToString(pCount));
    
    input.saveXML(xml);
    render.saveXML(xml);
    window.saveXML(xml);
    mesh.saveXML(xml);
    
    xml.save(file);
}







// EVENTS

bool modKey = false;

void vdome::keyPressed(int key){
    //cout << "keyPressed " << key << endl;
    switch(key){
        case OF_KEY_LEFT:
            break;
      
        case OF_KEY_UP:
            
            //window.x += 100;
            //window.update();
            mesh.radius--;
            mesh.setup();
            
            break;
        case OF_KEY_RIGHT:
            break;
        
        case OF_KEY_DOWN:
           
            mesh.radius++;
            mesh.setup();
            
            break;
        case OF_KEY_PAGE_UP:
            break;
        case OF_KEY_PAGE_DOWN:
            break;
        case OF_KEY_LEFT_SHIFT:
            break;
        case OF_KEY_LEFT_CONTROL:
            break;
        case OF_KEY_LEFT_SUPER: // mod
            modKey = true;
            break;
        case OF_KEY_RIGHT_SHIFT:
            break;
        case OF_KEY_RIGHT_CONTROL:
            break;
        case OF_KEY_RIGHT_ALT:
            break;
        case OF_KEY_RIGHT_SUPER: // mod
            modKey = true;
            break;
    }
    
}



void vdome::keyReleased(int key){
    //cout << "keyReleased " << key << endl;
    switch(key){
        case OF_KEY_LEFT:
            break;
        case OF_KEY_UP:
            break;
        case OF_KEY_RIGHT:
            break;
        case OF_KEY_DOWN:
            break;
        case OF_KEY_PAGE_UP:
            break;
        case OF_KEY_PAGE_DOWN:
            break;
        case OF_KEY_LEFT_SHIFT:
            break;
        case OF_KEY_LEFT_CONTROL:
            break;
        case OF_KEY_LEFT_SUPER: // mod
            modKey = false;
            break;
        case OF_KEY_RIGHT_SHIFT:
            break;
        case OF_KEY_RIGHT_CONTROL:
            break;
        case OF_KEY_RIGHT_ALT:
            break;
        case OF_KEY_RIGHT_SUPER: // mod
            modKey = false;
            break;
        case 115:  // s
            if (modKey) { // mod + s = save file
                cout << "saveXML " << endl;
                saveXML(xmlFile);
            }
            break;
    }
}
