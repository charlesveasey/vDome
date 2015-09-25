#include "window.h"
namespace vd {

ofEvent<int>  Window::keyPressEvent = ofEvent<int>();
ofEvent<int>  Window::keyReleaseEvent = ofEvent<int>();

/******************************************

 CONSTRUCTOR

 ********************************************/

Window::Window(){
    x = y = 0;
    fullscreen = false;
}

/******************************************

 INIT

 ********************************************/
void Window::init(){
    ofHideCursor();
    ofBackground(0,0,0);
    ofDisableAntiAliasing();
    ofSetFullscreen(fullscreen);
    
    ofSetWindowShape(width, height);
    ofSetWindowPosition(x, y);

    // projection shader
    shader.load("settings/shaders/vdome.vert", "settings/shaders/vdome.frag");
    
    //fix
    /*maskHistory.clear();
     for (int i=0; i<=(maxHistory+2); i++) {
     ofPixels buffer;
     buffer.allocate(1920, 1080, OF_IMAGE_COLOR_ALPHA); //FIXME: HARDCODED RESOLUTION?
     maskHistory.push_back(buffer);
     }*/
}
    
/******************************************
 
 SETUP
 
 ********************************************/
    
void Window::setup(){}
  
/******************************************
 
 UPDATE
 
 ********************************************/

void Window::update(){
   // if (input->source != input->SYPHON){
        //texture.loadData(input->getPixels()); //fix
        //texture = input->getTexture();

    //}
#ifdef TARGET_WIN32
	if (input->source == SPOUT)
		spout.update();
#endif
    
    if (menu.active)
        menu.update();
}

/******************************************
 
 DRAW
 
 ********************************************/

void Window::draw(){
    ofSetColor(255);
    
    for (int i=0; i<projectors.size(); i++) {
        if (projectors[i].enable) {            
            projectors[i].begin();
            
                if (input->source == input->SYPHON || input->source == input->SPOUT){
					#ifdef TARGET_OSX
						syphon.bind();
					#endif

					#ifdef TARGET_WIN32
						spout.bind();
					#endif
                }
                else{
					input->bind();
                }
            
					model.draw();
				
				if (input->source == input->SYPHON || input->source == input->SPOUT) {
					#ifdef TARGET_OSX
						syphon.unbind();
					#endif
					#ifdef TARGET_WIN32
						spout.unbind();
					#endif
                }
                else{
					input->unbind();
                }
            
            
            projectors[i].end();

            ofDisableNormalizedTexCoords();
            
            projectors[i].renderFbo.begin();
                ofClear(0);
                
                projectors[i].bind();
                        projectors[i].draw();
                projectors[i].unbind();
            
            projectors[i].renderFbo.end();
            
            ofEnableNormalizedTexCoords();
           
           
            projectors[i].renderFbo.getTexture().bind();
            
                if (projectors[i].active){
                    projectors[i].mask.draw();
                }
                
                shader.begin();
                
                    shader.setUniform1f("brightness", projectors[i].brightness);
                    shader.setUniform1f("contrast", projectors[i].contrast);
                    shader.setUniform1f("saturation", projectors[i].saturation);
            
                    shader.setUniform1i("interp", 1 );
                    shader.setUniform1f("amt", 1.0 );
                    shader.setUniform1f("mapdim", 256.0 );
                
                    shader.setUniformTexture("texsampler", projectors[i].renderFbo.getTexture(), 0);
                    shader.setUniformTexture("colorlut", projectors[i].curves.colorlutTextureRef(), 1);
                    shader.setUniformTexture("maskTex", projectors[i].mask.maskFbo.getTexture(), 2);

                    projectors[i].renderPlane.draw();
                
                shader.end();
            
            projectors[i].renderFbo.getTexture().unbind();
            
        }
        
        if (menu.active) {
            menu.draw(projectors[i].index, projectors[i].index + projectorStartingIndex);
        }
    }
}
    
/******************************************
 
 ACCESSORS
 
 ********************************************/

void Window::setVSync(bool val) {
    ofSetVerticalSync(val);
}
void Window::setFrameRate(int val) {
    ofSetFrameRate(val);
}

/******************************************

 ACCESSORS

 ********************************************/

ofPoint Window::getPosition() {
    return ofPoint(x, y);
}
void Window::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    ofSetWindowPosition(x, y);
}

ofPoint Window::getResolution() {
    return ofPoint(width, height);
}
void Window::setResolution(int w, int h) {
    width = w;
    height = h;
    ofSetWindowShape(w, h);
}

/******************************************

 SETTINGS

 ********************************************/

void Window::loadXML(ofXml &xml) {
    model.loadXML(xml);
    
    xml.setTo("window["+ ofToString(index) + "]");
    
    // get window xml settings
    string str = "";
    if (xml.exists("[@position]")) {
        str = xml.getAttribute("[@position]");
        x = ofToInt(ofSplitString(str, ",")[0]);
        y = ofToInt(ofSplitString(str, ",")[1]);
    }
    if (xml.exists("[@resolution]")) {
        str = xml.getAttribute("[@resolution]");
        width = ofToInt(ofSplitString(str, ",")[0]);
        height = ofToInt(ofSplitString(str, ",")[1]);
    }
    if (xml.exists("[@fullscreen]")) {
        str = ofToString( xml.getAttribute("[@fullscreen]") );
        if (str == "on") fullscreen = true;
        else             fullscreen = false;
    }

    // initialize window settings
    init();
    
    //get projector count from  xml
    int projectorCount = xml.getNumChildren("projector");

    // virtual projector position
    ofPoint pos(0,0);
    
    // loop through projectors
    for (int j=0; j<projectorCount; j++) {

        // create projectors
        Projector p;
        projectors.push_back(p);

        // get prjoector xml settings
        xml.setTo("projector["+ ofToString(j) + "]");
        projectors[j].loadXML(xml);

        projectors[j].init(j, projectorStartingIndex);

        // auto tile virtual projectors to the right
        if (j != 0) {
            pos.x += projectors[j-1].width;
            pos.y += 0;
        }
        
        projectors[j].setPlanePosition(pos.x, pos.y);
        
        // finish setting up projector
        projectors[j].setup();
        projectors[j].loadXML2(xml);
        xml.setToParent();
    }
    
    menu.projCount = projectors.size();
    menu.projectors = &projectors;
    menu.setup();
    
    xml.setToParent();
}

void Window::saveXML(ofXml &xml) {
    xml.setTo("window["+ ofToString(index) + "]");
    
    xml.setAttribute("position", ( ofToString(x) + "," + ofToString(y) ) );
    xml.setAttribute("resolution", ( ofToString(width) + "," + ofToString(height) ) );

    if (fullscreen) xml.setAttribute("fullscreen", "on" );
    else            xml.setAttribute("fullscreen", "off" );
    
    for (int i=0; i<projectors.size(); i++) {
        xml.setTo("projector["+ ofToString(i) + "]");
        projectors[i].saveXML(xml);
        xml.setToParent();
    }

    xml.setToParent();
   
    model.saveXML(xml);
}
    
/******************************************
 
 MOUSE
 
 ********************************************/

void Window::mousePressed(ofMouseEventArgs& mouseArgs) {
    menu.mousePressed(mouseArgs);
}

void Window::mouseDragged(ofMouseEventArgs& mouseArgs) {
    menu.mouseDragged(mouseArgs);
}

void Window::mouseReleased(ofMouseEventArgs& mouseArgs) {
    menu.mouseReleased(mouseArgs);
}

/******************************************
 
 KEYBOARD
 
 ********************************************/

void Window::keyPressed(int key){
    ofNotifyEvent(keyPressEvent,key,this);
}

void Window::keyReleased(int key){
    ofNotifyEvent(keyReleaseEvent,key,this);
}


}
