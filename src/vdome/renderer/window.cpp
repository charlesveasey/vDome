#include "window.h"
namespace vd {

//--------------------------------------------------------------
ofEvent<int>  Window::keyPressEvent = ofEvent<int>();
ofEvent<int>  Window::keyReleaseEvent = ofEvent<int>();

//--------------------------------------------------------------
Window::Window(){
    fullscreen = false;
}

//--------------------------------------------------------------
void Window::setup(){
    ofHideCursor();
    ofBackground(0,0,0);
    ofEnableAntiAliasing();
	ofEnableNormalizedTexCoords();
    ofSetWindowShape(width, height);
    ofSetWindowPosition(x, y);
    ofSetFullscreen(fullscreen);
	ofEnableDepthTest();
}
    
//--------------------------------------------------------------
void Window::update(){
    
#ifdef TARGET_WIN32
	if (input->source == SPOUT)
		spout.update();
#endif
    
    if (menu.active) {
        menu.update();
	}
}
    
//--------------------------------------------------------------
void Window::draw(){
    ofSetColor(255);
        
    for (int i=0; i<projectors.size(); i++) {
        
        // model view
        if (projectors[i].enable) {            
            ofPushMatrix();
            ofTranslate(projectors[i].getPlanePosition().x, 0);
           
            // input to model
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
            
            ofPopMatrix();
        }
        
        // onscreen menu
        if (menu.active) {
            menu.draw(projectors[i].index, projectors[i].index + projectorStartingIndex);
        }
        
    }
}

//--------------------------------------------------------------
void Window::setVSync(bool val) {
    ofSetVerticalSync(val);
}

//--------------------------------------------------------------
void Window::setFrameRate(int val) {
    ofSetFrameRate(val);
}
    
//--------------------------------------------------------------
void Window::loadXML(ofXml &xml) {
  
	// model settings
	model.loadXML(xml);
      
    // window settings
	xml.setTo("window[" + ofToString(index) + "]");

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
    setup();

    //get projector count from  xml
    int projectorCount = xml.getNumChildren("projector");

    // projector position
    ofPoint pos(0,0);
    
    // loop through projectors
    for (int j=0; j<projectorCount; j++) {

        // create projectors
        Projector p;
        projectors.push_back(p);

        // get projector xml settings
        xml.setTo("projector["+ ofToString(j) + "]");

        projectors[j].init(j, projectorStartingIndex);
		projectors[j].loadXML(xml);

        // tile virtual projectors to the right
        if (j != 0) {
            pos.x += projectors[j-1].getWidth();
            pos.y += 0;
        }        
        projectors[j].setPlanePosition(pos.x, pos.y);
        
        // finish setting up projector
        projectors[j].setup();
        projectors[j].loadXML2(xml);
        xml.setToParent();
    }

	// set up menu
    menu.projCount = projectors.size();
    menu.projectors = &projectors;
    
    xml.setToParent();
}

//--------------------------------------------------------------
void Window::saveXML(ofXml &xml) {
	// save window settings
    xml.setTo("window["+ ofToString(index) + "]");
    
    xml.setAttribute("position", ( ofToString(x) + "," + ofToString(y) ) );
    xml.setAttribute("resolution", ( ofToString(width) + "," + ofToString(height) ) );

    if (fullscreen) xml.setAttribute("fullscreen", "on" );
    else            xml.setAttribute("fullscreen", "off" );

	// save projector settings
    for (int i=0; i<projectors.size(); i++) {
        xml.setTo("projector["+ ofToString(i) + "]");
        projectors[i].saveXML(xml);
        xml.setToParent();
    }
    xml.setToParent();
    
    // save model settings
    model.saveXML(xml);
}

//--------------------------------------------------------------
void Window::mousePressed(ofMouseEventArgs& mouseArgs) {
    menu.mousePressed(mouseArgs);
}

//--------------------------------------------------------------
void Window::mouseDragged(ofMouseEventArgs& mouseArgs) {
    menu.mouseDragged(mouseArgs);
}

//--------------------------------------------------------------
void Window::mouseReleased(ofMouseEventArgs& mouseArgs) {
    menu.mouseReleased(mouseArgs);
}

//--------------------------------------------------------------
void Window::mouseMoved(ofMouseEventArgs& mouseArgs) {
	menu.mouseMoved(mouseArgs);
}

//--------------------------------------------------------------
void Window::mouseScrolled(ofMouseEventArgs& mouseArgs) {
	menu.mouseScrolled(mouseArgs);
}

//--------------------------------------------------------------
void Window::keyPressed(int key){
    ofNotifyEvent(keyPressEvent,key,this);}

//--------------------------------------------------------------
void Window::keyReleased(int key){
    ofNotifyEvent(keyReleaseEvent,key,this);
}


}//////////
