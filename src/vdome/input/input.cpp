#pragma once
#include "input.h"
#include "commands.h"

namespace vd {

extern float projCount;
extern int maxHistory;
extern CommandHistory history;
extern vector<ofPixels> maskHistory;

/******************************************

 CONSTRUCTOR

 ********************************************/

Input::Input(){
    durationSent = false;
    maxSource = 7;
    resolution = 2048;
    framerate = 30;
    slide = 10;
    setSlide(slide);
    setLoop(true);
    setResolution(resolution);
    ofAddListener(media.endEvent, this, &Input::mediaEnd);
    format = 0;
    maxFormat = 1;
    lastSource = -1;
    lastFormat = -1;
}

/******************************************

 SETUP

 ********************************************/

void Input::setup(){
    stop();
    close();

    durationSent = false;
    endSent = false;

    switch(source){
        case GRID:      media.open("settings/media/warp/grid-2k.png");					break;
        case MEDIA:     media.open(files);												break;
        case CAPTURE:   capture.open();													break;
		case BLACK:     color.setup(); color.fillBlack();								break;
        case WHITE:     color.setup(); color.fillWhite();								break;
        case GREY:      color.setup(); color.fillGrey();								break;
		case COLOR:     color.setup(); color.fill(cColor.r, cColor.g, cColor.b);		break;

        case SYPHON:
            #ifdef TARGET_OSX
            syphon.setup();
            #endif
            break;

        case SPOUT:
            #ifdef TARGET_WIN32
            spout.setup();
            #endif
            break;

        default:        color.setup(); color.fillBlack();                              break;
    }
    
    // reset aspect ratio
    model->textureScaleInternalW = 1;
    model->textureScaleInternalH = 1;
    
    
    // letterbox non-square aspect ratios
    ratio = -99999;
    if (source == MEDIA){
        ratio = media.getRealHeight()/media.getRealWidth();
    }
    else if (source == CAPTURE){
        ratio = capture.getRealHeight()/capture.getRealWidth();
    }
    else if (source == SYPHON){
        #ifdef TARGET_OSX
        if (syphon.isSetup())
            ratio = syphon.getHeight()/syphon.getWidth();
        #endif
    }
    
    if (ratio != -99999){
        if (ratio < 1){
            model->textureScaleInternalW = 1;
            model->textureScaleInternalH = ratio;
        }
        else if (ratio > 1){
            model->textureScaleInternalW = 1/ratio;
            model->textureScaleInternalH = 1;
        }
    }
    
    
    if (source == CAPTURE || source == SYPHON){
		model->textureFlipInternal = true;
		model->setup();
	}
	else {
        if (source == GRID || source == BLACK || source == WHITE || source == GREY || source == COLOR){
            model->textureFlipInternal = false;
            setFormat();
        }
        else {
            model->textureFlipInternal = false;
            model->setup();
        }
	}
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
    else if (source == CAPTURE)		capture.update();

    #ifdef TARGET_OSX
    else if (source == SYPHON && syphon.isSetup()){
        float nRatio = syphon.getHeight()/syphon.getWidth();
        if (nRatio != ratio){
            ratio = nRatio;
            if (ratio < 1){
                model->textureScaleInternalW = 1;
                model->textureScaleInternalH = ratio;
            }
            else if (ratio > 1){
                model->textureScaleInternalW = 1/ratio;
                model->textureScaleInternalH = 1;
            }
            else {
                model->textureScaleInternalW = 1;
                model->textureScaleInternalH = 1;
            }
            model->setup();
        }
        
    }
    #endif
    
    #ifdef TARGET_WIN32
	else if (source == SPOUT)		spout.update();
    #endif
}

/******************************************

 BIND

 ********************************************/

void Input::bind(){
    if      (source == MEDIA)    media.bind();
    else if (source == CAPTURE)  capture.bind();
    else if (source == GRID)     media.bind();
    else if (source == BLACK || source == WHITE || source == GREY || source == COLOR)     color.bind();

    #ifdef TARGET_OSX
    if (source == SYPHON)    syphon.bind();
    #endif

	#ifdef TARGET_WIN32
	if (source == SPOUT)    spout.bind();
	#endif
}

void Input::unbind(){
    if      (source == MEDIA)    media.unbind();
    else if (source == CAPTURE)  capture.unbind();
    else if (source == GRID)     media.unbind();
    else if (source == BLACK || source == WHITE || source == GREY || source == COLOR)     color.unbind();

    #ifdef TARGET_OSX
    if (source == SYPHON)    syphon.unbind();
    #endif

	#ifdef TARGET_WIN32
	if (source == SPOUT)    spout.unbind();
	#endif
}

/******************************************

 CONTROLS

 ********************************************/

void Input::setSource(string s) {
    if      (s == "media")      source = MEDIA;
    else if (s == "capture")    source = CAPTURE;
    else if (s == "syphon")     source = SYPHON;
	else if (s == "spout")      source = SPOUT;
    else if (s == "grid")       source = GRID;
    else if (s == "black")      source = BLACK;
    else if (s == "white")      source = WHITE;
    else if (s == "grey")       source = GREY;
	else if (s == "color")      source = COLOR;
    setup();
}
string Input::getSource() {
	string s = "";
    if      (source = MEDIA)       s = "media";
    else if (source = CAPTURE)     s = "capture";
    else if (source = SYPHON)      s = "syphon";
	else if (source = SPOUT)       s = "spout";
    else if (source = GRID)        s = "grid";
    else if (source = BLACK)       s = "black";
    else if (source = WHITE)       s = "white";
    else if (source = GREY)        s = "grey";
	else if (source = COLOR)       s = "color";
	return s;
}
   
void Input::setSourceInt(int i) {
	source = i;
    setup();
}
int Input::getSourceInt() {
	return source;
}

void Input::setFormat(string s) {
    if      (s == "domemaster")     format = DOMEMASTER;
    else if (s == "hd")             format = HD;
    setFormat();
}   
void Input::setFormat() {
    if (source == GRID || source == BLACK || source == WHITE || source == GREY){
        format = DOMEMASTER;
    }
    if (format == DOMEMASTER){
        model->textureScaleInternal = 1;
        model->textureTiltInternal = 0;
        model->setup();
    }
    else if (format == HD){
        model->textureScaleInternal = .5625;
        model->textureTiltInternal = 50;
        model->setup();
    }
}

void Input::setFormatInt(int i) {
    format = i;
    setFormat();
}   
int Input::getFormatInt() {
    return format;
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
    else if (source == BLACK || source == WHITE || source == GREY || source == COLOR)     color.close();

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

void Input::setColor(int r, int g, int b){
	cColor = ofColor(r,g,b);
	color.fill(cColor.r, cColor.g, cColor.b);
}

/******************************************

 KEYBOARD

 ********************************************/

void Input::keyPressed(int key) {
    int inc=0;
    switch (key) {
        case OF_KEY_RIGHT:
            inc++;
            if (editMode == LOOP)       history.execute( new SetLoop(*this, true));      break;
            break;

        case OF_KEY_LEFT:
            inc--;
            if (editMode == LOOP)       history.execute( new SetLoop(*this, false));      break;
            break;
    }
    if (editMode == SOURCE && inc != 0){
        lastSource = source;
		int newSource = source + inc; 

        if (newSource > maxSource)     newSource = maxSource;
        else if (newSource < 0)        newSource = 0;
        
		#ifdef TARGET_OSX
        if (newSource == SPOUT)            newSource += inc;
        #endif
        #ifdef TARGET_WIN32
        if (newSource == SYPHON)           newSource += inc;
        #endif
        #ifdef TARGET_LINUX
        
		while (newSource == SYPHON || newSource == SPOUT) {
            newSource += inc;
        }
        #endif

		if (newSource != lastSource) 
			history.execute( new SetSource(*this, newSource) );
    }
    else if (editMode == FORMAT && inc != 0){
        lastFormat = format;
		int newFormat = format + inc;

        if (newFormat > maxFormat)			newFormat = maxFormat;
        else if (newFormat < 0)				newFormat = 0;
        
        if (newFormat != lastFormat)
		history.execute( new SetFormat(*this, newFormat) );

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
		else if (v == "spout")	   source = SPOUT;
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
	else if (source == SPOUT)     str = "spout";
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
