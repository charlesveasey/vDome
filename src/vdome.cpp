#include "vdome.h"
namespace vd {

// global variables
float projCount = 1;
float projWidth = 1024;
float projHeight = 768;
int maxHistory = 25;
vector<ofPixels> maskHistory;

int winCount = 1;
    
/******************************************

 CONSTRUCTOR

 ********************************************/

vdome::vdome() {
    menu.input = &input;
    menu.dome = &dome;
    menu.windows = &windows;
    menu.projectors = &projectors;
    socket.input = &input;
    autosave = false;
}

/******************************************

 SETUP

 ********************************************/

void vdome::setup(){
    ofSetEscapeQuitsApp(false);

    glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();
    glfwWindows = &glfw->windows;
    
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
    if (wIndex == 0) {
        input.update();
        
        if (socket.enabled)
            socket.update();
    }
    
    if (menu.active) {
        if (saveThread.saved) {
            menu.saved = true;
            saveThread.saved = false;
        }
    }
}

/******************************************

 DRAW

 ********************************************/
    
void vdome::draw(){
    
    ofSetHexColor(0xFFFFFF);

    int wi = glfw->getWindowIndex();

    
    for (int i=windows[wi].firstProjector; i<=windows[wi].lastProjector; i++) {
    
        projectors[i].begin();
            input.bind();
                dome.draw();
            input.unbind();
        projectors[i].end();

        ofDisableNormalizedTexCoords();
        projectors[i].renderFbo.begin();
        ofClear(0);
        projectors[i].bind();
       
            projectors[i].draw();

        projectors[i].unbind();
        projectors[i].renderFbo.end();
        ofEnableNormalizedTexCoords();

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

        if (menu.active) {
            menu.draw( projectors[i].index );
        }
        
    }

}

/******************************************

 SETTINGS

 *******************************************/

void vdome::loadXML(ofXml &xml) {

    input.loadXML(xml);
    render.loadXML(xml);
    dome.loadXML(xml);
    socket.loadXML(xml);

    winCount = 0;
    projCount = 0;

    if (xml.exists("window")) {
        winCount = xml.getNumChildren("window");
        for (int i=0; i<winCount; i++) {
            Window w;
            xml.setTo("window["+ ofToString(i) + "]");
            
            w.firstProjector = projCount;
            projCount += xml.getNumChildren();
            w.lastProjector = projCount-1;
            if (i > 0)
                glfw->createWindow();
            w.glfwWindow = glfwWindows->at(i);
            
            for (int j=0; j<projCount-w.firstProjector; j++) {
                Projector p;
                p.setPlanePosition(j*projWidth, 0);
                projectors.push_back(p);
            }
            
            windows.push_back(w);
            xml.setToParent();
        }
    }
    int c=0;
    for (int i=0; i<winCount; i++) {
        xml.setTo("window["+ ofToString(i) + "]");
        windows[i].loadXML(xml);
        
        for (int j=0; j<=windows[i].lastProjector-windows[i].firstProjector; j++) {
            xml.setTo("projector["+ ofToString(j) + "]");
            projectors[c].init(c);
            projectors[c].loadXML(xml);
            xml.setToParent();
            c++;
        }
        
        xml.setToParent();
    }
    
    if (xml.exists("[@autosave]")) {
        string str = ofToString( xml.getAttribute("[@autosave]") );
        if (str == "on")    autosave = true;
        else                autosave = false;
    }
}

void vdome::saveXML(ofXml &xml) {
    
    socket.saveXML(xml);
    input.saveXML(xml);
    render.saveXML(xml);
    dome.saveXML(xml);
    
    int c = 0;
    for (int i=0; i<winCount; i++) {
        
        xml.setTo("window["+ ofToString(i) + "]");
        windows[i].saveXML(xml);

        for (int j=0; j<=windows[i].lastProjector-windows[i].firstProjector; j++) {
            xml.setTo("projector["+ ofToString(j) + "]");
            projectors[c].saveXML(xml);
            xml.setToParent();
            c++;
        }
        
        xml.setToParent();

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
