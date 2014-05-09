#include "vdome.h"
namespace vd {

// global variables
float projCount = 1;
float projWidth = 1024;
float projHeight = 768;
int maxHistory = 10;

/******************************************

 CONSTRUCTOR

 ********************************************/

vdome::vdome() {
    menu.input = &input;
    menu.dome = &dome;
    menu.projectors = &projectors;
}

/******************************************

 SETUP

 ********************************************/

void vdome::setup(){

    ofSetEscapeQuitsApp(false);
    ofHideCursor();

    //window.setup();
    render.setup();
    dome.setup();

    // input
    // 0 = image
	// 1 = capture
    // 2 = video
    // 3 = hap
    // 4 = syphon

    input.source = 0;
    input.frameRate = render.getFrameRate();
    input.setup();

    // projection shader
	shader.load("settings/shaders/vdome.vert", "settings/shaders/vdome.frag");

    // xml settings
    xmlFile = "settings.xml";
    
    if (xml.load(xmlFile)) {
        loadXML(xml);
    }
    else {
        for(int i=0; i<projCount; i++) {
            Projector p;
            p.init(i);
            projectors.push_back(p);
        }
    }
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
    
    ofSetHexColor(0xFFFFFF);

	for(int i=0; i<projCount; i++){
        if (!projectors[i].enable) continue;
        
        projectors[i].begin();
            input.bind();
                dome.draw();
            input.unbind();
        projectors[i].end();
	}
    
    for(int i=0; i<projCount; i++){
        if (!projectors[i].enable) continue;

        ofDisableNormalizedTexCoords();
        projectors[i].renderFbo.begin();
        ofClear(0);
        projectors[i].bind();
       
            projectors[i].draw();

        projectors[i].unbind();
        projectors[i].renderFbo.end();
        ofEnableNormalizedTexCoords();
	}

	for(int i=0; i<projCount; i++) {
        if (!projectors[i].enable) continue;

        projectors[i].renderFbo.getTextureReference().bind();
        
            if (projectors[i].active) {
                projectors[i].mask.draw();
            }
        
            shader.begin();

                shader.setUniform1f("brightness", projectors[i].brightness);
                shader.setUniform1f("contrast", projectors[i].contrast);
                shader.setUniform1f("blackLevel", projectors[i].blackLevel);
                shader.setUniform1f("whiteLevel", projectors[i].whiteLevel);

                shader.setUniform1f("hue", projectors[i].hue);
                shader.setUniform1f("saturation", projectors[i].saturation);
                shader.setUniform1f("lightness", projectors[i].lightness);

                shader.setUniform1f("gamma", projectors[i].gamma);
                shader.setUniform1f("gammaR", projectors[i].gammaR);
                shader.setUniform1f("gammaG", projectors[i].gammaG);
                shader.setUniform1f("gammaB", projectors[i].gammaB);

                shader.setUniformTexture("texsampler", projectors[i].renderFbo.getTextureReference(), 0);
                shader.setUniformTexture("maskTex", projectors[i].mask.maskFbo.getTextureReference(), 1);

                projectors[i].renderPlane.draw();
    
            shader.end();

        projectors[i].renderFbo.getTextureReference().unbind();
	}

    menu.draw();
}

/******************************************

 SETTINGS

 *******************************************/

void vdome::loadXML(ofXml &xml) {
    if (xml.exists("projectors[@count]")) {
        projCount = ofToInt( xml.getAttribute("projectors[@count]") );
        projCount = projCount;
    }

    if (xml.exists("projectors[@dimensions]")) {
        string str = xml.getAttribute("projectors[@dimensions]");
        projWidth = ofToFloat(ofSplitString(str, ",")[0]);
        projHeight = ofToFloat(ofSplitString(str, ",")[1]);
    }

    for(int i=0; i<projCount; i++) {
        Projector p;
        p.init(i);
        projectors.push_back(p);
    }

    input.loadXML(xml);
    render.loadXML(xml);
    window.loadXML(xml);
    dome.loadXML(xml);

    for(int i=0; i<projCount; i++) {
        projectors[i].loadXML(xml);
    }
}

void vdome::saveXML(ofXml &xml) {
    xml.setTo("projectors");
    xml.setAttribute("count", ofToString(projCount));
    xml.setToParent();

    tcp.saveXML(xml);
    input.saveXML(xml);
    render.saveXML(xml);
    window.saveXML(xml);
    dome.saveXML(xml);

    for(int i=0; i<projCount; i++) {
		projectors[i].saveXML(xml);
	}

    if (xml.save(xmlFile)) {
        menu.saved = true;
    }
}

/******************************************

 MOUSE

 ********************************************/

void vdome::mousePressed(ofMouseEventArgs& mouseArgs) {
    menu.mousePressed(mouseArgs);
}

void vdome::mouseDragged(ofMouseEventArgs& mouseArgs) {
    menu.mouseDragged(mouseArgs);

}

void vdome::mouseReleased(ofMouseEventArgs& mouseArgs) {
    menu.mouseReleased(mouseArgs);
}

/******************************************

 KEYBOARD

 ********************************************/

void vdome::keyPressed(int key){
    if (key == 115) { // s
        if (menu.active && menu.ctrl)  { // ctrl + s = save file
            saveXML(xml);
            cout << "saveXML " << endl;
        }
    }
    menu.keyPressed(key);
}

void vdome::keyReleased(int key){
    menu.keyReleased(key);
}

/******************************************
 
 FILE
 
 ********************************************/

void vdome::dragEvent(ofDragInfo dragInfo){
    input.dragEvent(dragInfo);
}


/******************************************

 EXIT

 ********************************************/

void vdome::exit(){
    cout << "exit" << endl;
}

}
