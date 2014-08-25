#include "input.h"
namespace vd {

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Input::Input(){
    durationSent = false;
    maxSource = 6;
    resolution = 2048;
    framerate = 30;
    slide = 10;
    setSlide(slide);
    setLoop(true);
    setResolution(resolution);
    ofAddListener(media.endEvent, this, &Input::mediaEnd);
}

/******************************************
 
 SETUP
 
 ********************************************/

void Input::setup(){
    stop();
    close();
    
    durationSent = false;
    endSent = false;
    
	if (source == CAPTURE || source == SYPHON){
		model->textureFlipInternal = true;
		model->setup();
	}
	else {
		model->textureFlipInternal = false;
		model->setup();
	}

    switch(source){
        case GRID:      media.open("settings/media/warp/grid-2k.png");  break;
        case MEDIA:     media.open(files);                              break;
        case CAPTURE:   capture.open();                                 break;
        case BLACK:     color.fillBlack();                              break;
        case WHITE:     color.fillWhite();                              break;
        case GREY:      color.fillGrey();                               break;
        
        case SYPHON:
            #ifdef TARGET_OSX
            syphon.setup();
            #endif
            break;
            
        default:        color.fillBlack();                              break;
    }
    
}

/******************************************
 
 BIND
 
 ********************************************/

void Input::bind(){
    if      (source == MEDIA)    media.bind();
    else if (source == CAPTURE)  capture.bind();
    else if (source == GRID)     media.bind();
    else if (source == BLACK || source == WHITE || source == GREY)     color.bind();
    
    #ifdef TARGET_OSX
    if (source == SYPHON)    syphon.bind();
    #endif
}

void Input::unbind(){
    if      (source == MEDIA)    media.unbind();
    else if (source == CAPTURE)  capture.unbind();
    else if (source == GRID)     media.unbind();
    else if (source == BLACK || source == WHITE || source == GREY)     color.unbind();

    #ifdef TARGET_OSX
    if (source == SYPHON)    syphon.unbind();
    #endif
}

/******************************************
 
 UPDATE
 
 ********************************************/

void Input::update(){
    if (source == MEDIA) {
        media.update();
        if (!durationSent) {
            if (media.getDuration() > 0 && media.isLoaded()) {
                socket->sendDuration();
                durationSent = true;
            }
        }
    }
    else if (source == CAPTURE)  capture.update();
}
    
    
/******************************************
 
 CONTROLS
 
 ********************************************/

void Input::setSource(string s) {
    if      (s == "media")      source = MEDIA;
    else if (s == "capture")    source = CAPTURE;
    else if (s == "syphon")     source = SYPHON;
    else if (s == "grid")       source = GRID;
    else if (s == "black")      source = BLACK;
    else if (s == "white")      source = WHITE;
    else if (s == "grey")       source = GREY;
    setup();
}

void Input::play() {
    if (source == MEDIA) media.play();
}

bool Input::isPlaying() {
    if (source == MEDIA) return true;
    else                 return false;
}

void Input::stop() {
    if (source == MEDIA) media.stop();
}

void Input::close() {
    stop();
    if      (source == MEDIA)       media.close();
    else if (source == CAPTURE)     capture.close();
    else if (source == GRID)        media.close();
    else if (source == BLACK || source == WHITE || source == GREY)     color.close();

}

void Input::previous() {
    if (source == MEDIA) media.previous();
}

void Input::next() {
    if (source == MEDIA) media.next();
}

void Input::seek(float f) {
    if (source == MEDIA) media.seek(f);
}

bool Input::getLoop() {
    return media.getLoop();
}

void Input::setLoop(bool b) {
    media.setLoop(b);
}

float Input::getPosition() {
    return media.getPosition();
}

float Input::getDuration() {
    return media.getDuration();
}

void Input::setResolution(int r){
    resolution = r;
    color.setResolution(r);
    capture.setResolution(r);
}
    
void Input::setSlide(int s){
    slide = s;
    media.setSlideshow(true, s);
}
 
string Input::getFilepath(){
    return media.getFilepath();
}
    
void Input::setVolume(float v){
    media.setVolume(v);
}
    
void Input::mediaEnd(bool &end){
    if (!endSent) {
        socket->sendEnd();
        endSent = true;
    }
}

/******************************************
 
 KEYBOARD
 
 ********************************************/

void Input::keyPressed(int key) {
    int inc=0;
    switch (key) {
        case OF_KEY_RIGHT:
            inc++;
            if (editMode == LOOP)       setLoop(true);      break;
            break;
            
        case OF_KEY_LEFT:
            inc--;
            if (editMode == LOOP)       setLoop(false);      break;
            break;
    }
    if (editMode == SOURCE && inc != 0){
        lastSource = source;
        source += inc;
        if (source+inc > maxSource)     source = maxSource;
        else if (source+inc < 0)          source = 0;
        #ifdef TARGET_WIN32
        if (source == SYPHON)           source += inc;
        #endif
        #ifdef TARGET_LINUX
        if (source == SYPHON)           source += inc;
        #endif
        if (lastSource != source)       setup();
    }
}

/******************************************
 
 FILE OPEN
 
 ********************************************/
  
void Input::setFile(string filepath){
    files.clear();
    files.push_back(filepath);
    source = MEDIA;
}
    
void Input::openFile(string filepath){
    setFile(filepath);
    setup();
}
    
void Input::dragEvent(ofDragInfo dragInfo){
    source = MEDIA;
    files = dragInfo.files;
    setup();
}

void Input::openFileDialog(){
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a media file");
    if (openFileResult.bSuccess)
        setFile(openFileResult.filePath);
}

/******************************************
 
 SETTINGS
 
 ********************************************/

void Input::loadXML(ofXml &xml) {
    string v;
    
    if (xml.exists("[@resolution]"))
        setResolution(ofToInt( xml.getAttribute("[@resolution]")));
 
	if (xml.exists("input[@file]"))
        setFile(ofToString(xml.getAttribute("input[@file]")));

    if (xml.exists("input[@source]")) {
        v = xml.getAttribute("input[@source]");
        if (v == "grid")		   source = GRID;
        else if (v == "media")     source = MEDIA;
        else if (v == "capture")   source = CAPTURE;
        else if (v == "syphon")    source = SYPHON;
        else if (v == "black")     source = BLACK;
        else if (v == "white")     source = WHITE;
        else if (v == "grey")      source = GREY;
    }
    
    if (xml.exists("input[@loop]")) {
        v = xml.getAttribute("input[@loop]");
        if (v == "on") setLoop(true);
        else           setLoop(false);
    }
    
    if (xml.exists("input[@slide]"))
        setSlide(ofToFloat( xml.getAttribute("input[@slide]")));
    
    if (xml.exists("input[@renderer]"))
        media.forceVideoRenderer = ofToString( xml.getAttribute("input[@renderer]") );
    
    setup();
}

void Input::saveXML(ofXml &xml) {
    xml.setAttribute("resolution", ofToString(resolution));
    
    xml.setTo("input");
    string str;
    
    if (source == GRID)			  str = "grid";
    else if (source == MEDIA)     str = "media";
    else if (source == CAPTURE)   str = "capture";
    else if (source == SYPHON)    str = "syphon";
    else if (source == BLACK)     str = "black";
    else if (source == WHITE)     str = "white";
    else if (source == GREY)      str = "grey";
    
    xml.setAttribute("source", str );
    if (files.size() > 0)
        xml.setAttribute("file", files[0] );
    
    if (getLoop()) xml.setAttribute("loop", "on");
    else      xml.setAttribute("loop", "off");
    
    xml.setAttribute("slide", ofToString(slide));
    xml.setToParent();
}
}
