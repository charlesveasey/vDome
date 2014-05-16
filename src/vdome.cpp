#include "vdome.h"
namespace vd {

// global variables
float projCount = 1;
float projWidth = 1024;
float projHeight = 768;
int maxHistory = 25;
vector<ofPixels> maskHistory;

/******************************************

 CONSTRUCTOR

 ********************************************/

vdome::vdome() {
    menu.input = &input;
    menu.dome = &dome;
    menu.projectors = &projectors;
    socket.input = &input;
    autosave = false;
}

/******************************************

 SETUP

 ********************************************/

void vdome::setup(){
    ofSetEscapeQuitsApp(false);
    ofHideCursor();

    render.setup();
    dome.setup();

    // input setup
    input.source = 0;
    input.frameRate = render.getFrameRate();
    input.setup();

    // projection shader
	shader.load("settings/shaders/vdome.vert", "settings/shaders/vdome.frag");

    // xml settings
    xmlFile = "settings.xml";
    saveThread.xml.push_back(&xml);
    saveThread.files.push_back(xmlFile);

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
    
    for(int i=0; i<projCount; i++) {
        Projector p = projectors[i];
        saveThread.xml.push_back(p.plane.wXml);
        saveThread.files.push_back("settings/warp/warp-"+ofToString(i+1)+".xml");
        saveThread.image.push_back(projectors[i].mask.maskFboImage);
        saveThread.imageFiles.push_back("settings/masks/mask-" + ofToString(i+1) + ".png");
    }
    
    maskHistory.clear();
    for (int i=0; i<=(maxHistory+2); i++) {
        ofPixels buffer;
        buffer.allocate(projWidth, projHeight, OF_IMAGE_COLOR_ALPHA);
        maskHistory.push_back(buffer);
    }
    
    menu.autosave = autosave;
}

/******************************************

 UPDATE

 ********************************************/

void vdome::update() {
    input.update();
    if (socket.enabled)
        socket.update();
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
        
            if (projectors[i].active)
                projectors[i].mask.draw();
        
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

    if (menu.active) {
        if (saveThread.saved) {
            menu.saved = true;
            saveThread.saved = false;
        }
        menu.draw();
    }
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
    socket.loadXML(xml);

    for(int i=0; i<projCount; i++) {
        projectors[i].loadXML(xml);
    }
    
    if (xml.exists("[@autosave]")) {
        string str = ofToString( xml.getAttribute("[@autosave]") );
        if (str == "on")    autosave = true;
        else                autosave = false;
    }
}

void vdome::saveXML(ofXml &xml) {
    xml.setTo("projectors");
    xml.setAttribute("count", ofToString(projCount));
    xml.setToParent();

    socket.saveXML(xml);
    input.saveXML(xml);
    render.saveXML(xml);
    window.saveXML(xml);
    dome.saveXML(xml);

    for(int i=0; i<projCount; i++) {
        projectors[i].saveXML(xml);
	}

    saveThread.save();
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
    if (autosave && menu.active)
        saveXML(xml);
}

/******************************************

 KEYBOARD

 ********************************************/

void vdome::keyPressed(int key){
    if (key == 115 && !autosave) { // s
        if (menu.active && menu.ctrl)
            saveXML(xml);
    }
    menu.keyPressed(key);
}

void vdome::keyReleased(int key){
    menu.keyReleased(key);
    if (key != OF_KEY_UP || key == OF_KEY_DOWN) {
        if (autosave && menu.active)
            saveXML(xml);
    }
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
    input.stop();
    input.close();
}

}
