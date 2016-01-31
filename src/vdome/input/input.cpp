#include "input.h"
#include "commands.h"
namespace vd {
    
/******************************************
 INPUT
 ********************************************/
    
extern int maxHistory;
extern CommandHistory history;

Input::Input(){
    durationSent = false;
    resolution = 2048;
    framerate = 30;
    setLoop(false);
    setResolution(resolution);
    ofAddListener(media.endEvent, this, &Input::mediaEnd);
    format = 0;
}

/******************************************
 SETUP
 ********************************************/

void Input::setup(){
    close();

    durationSent = false;
    endSent = false;

    switch(source){
        case GRID:      media.open("settings/media/warp/grid-2k.png");                  break;
        case MEDIA:     media.open(filepath);											break;
        case CAPTURE:   capture.open();													break;
		case BLACK:     color.setup(); color.fillBlack();								break;
        case WHITE:     color.setup(); color.fillWhite();								break;
        case GREY:      color.setup(); color.fillGrey();								break;
		case COLOR:     color.setup(); color.fill(cColor.r, cColor.g, cColor.b);		break;
        default:        color.setup(); color.fillBlack();                               break;
    }

    // letterbox non-square aspect ratios
    ratio = -99999;
    if (source == MEDIA){
        ratio = media.getRealHeight()/media.getRealWidth();
    }
    else if (source == CAPTURE){
        ratio = capture.getRealHeight()/capture.getRealWidth();
    }
}

/******************************************
 UPDATE
 ********************************************/

void Input::update(){
    if (source == MEDIA)
        media.update();
    else if (source == CAPTURE)
        capture.update();
}


/******************************************
 GET INPUT SOURCE AS STRING
 ********************************************/

string Input::getSource() {
	string s = "";
    if      (source == MEDIA)       s = "media";
    else if (source == CAPTURE)     s = "capture";
    else if (source == SYPHON)      s = "syphon";
	else if (source == SPOUT)       s = "spout";
    else if (source == GRID)        s = "grid";
    else if (source == BLACK)       s = "black";
    else if (source == WHITE)       s = "white";
    else if (source == GREY)        s = "grey";
	else if (source == COLOR)       s = "color";
	return s;
}
   

/******************************************
 CONVERT INPUT SOURCE FROM STRING TO INT
 ********************************************/
    
int Input::convertSourceString(string s) {
    int sint;
    if      (s == "media")      sint = MEDIA;
    else if (s == "capture")    sint = CAPTURE;
    else if (s == "syphon")     sint = SYPHON;
    else if (s == "spout")      sint = SPOUT;
    else if (s == "grid")       sint = GRID;
    else if (s == "black")      sint = BLACK;
    else if (s == "white")      sint = WHITE;
    else if (s == "grey")       sint = GREY;
    else if (s == "color")      sint = COLOR;
    return sint;
}

/******************************************
 SET SOURCE AS INT: MUST CALL SETUP AFTER
 ********************************************/
    
void Input::setSourceInt(int i) {
	source = i;
}
    
/******************************************
 GET INPUT SOURCE AS INT
 ********************************************/
    
int Input::getSourceInt() {
	return source;
}

/******************************************
 CONVERT FORMAT STRING TO INT
 ********************************************/
    
int Input::convertFormatString(string s){
    int sint;
    if      (s == "domemaster")     sint = DOMEMASTER;
    else if (s == "hd")             sint = HD;
    return sint;
}

/******************************************
 SET INPUT FORMAT FROM INTERNAL SOURCE
 ********************************************/

void Input::setFormat() {
    if (source == GRID || source == BLACK || source == WHITE || source == GREY){
        format = DOMEMASTER;
    }
}

/******************************************
 SET INPUT FORMAT AS INT
 ********************************************/
    
void Input::setFormatInt(int i) {
    format = i;
    // must call: setFormat();
}

/******************************************
 GET INPUT FORMAT AS INT
 ********************************************/
    
int Input::getFormatInt() {
    return format;
}

/******************************************
 PLAY (IF SOURCE IS MEDIA)
 ********************************************/

void Input::play() {
    if (source == MEDIA) media.play();
}

/******************************************
 RETURN TRUE IF MEDIA IS PLAYING
 ********************************************/
    
bool Input::isPlaying() {
    if (source == MEDIA && media.isPlaying()) {
        return true;
    }
    else {
        return false;
    }
}

/******************************************
 STOP MEDIA
 ********************************************/
    
void Input::stop() {
	if (source == MEDIA)			media.stop();
}

/******************************************
 CLOSE MEDIA
 ********************************************/
    
void Input::close() {
    stop();
    if      (source == MEDIA)       media.close();
    else if (source == CAPTURE)     capture.close();
    else if (source == GRID)        media.close();
    else if (source == BLACK || source == WHITE || source == GREY || source == COLOR)     color.close();
}

/******************************************
 SEEK MEDIA
 ********************************************/
    
void Input::seek(float f) {
    if (source == MEDIA) media.seek(f);
}

/******************************************
 GET MEDIA LOOP STATE: TRUE IF LOOPING
 ********************************************/
    
bool Input::getLoop() {
    return media.getLoop();
}

/******************************************
 SET MEDIA LOOP STATE: TRUE TO LOOP
 ********************************************/
    
void Input::setLoop(bool b) {
    media.setLoop(b);
}

/******************************************
 GET MEDIA PLAYBACK POSITION
 ********************************************/
    
float Input::getPosition() {
    return media.getPosition();
}

/******************************************
 GET MEDIA DURATION
 ********************************************/
    
float Input::getDuration() {
    return media.getDuration();
}

   
void Input::setResolution(int r){
    resolution = r;
    capture.setResolution(r);
    color.setResolution(r);
}

int Input::getResolution(){
    return resolution;
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
 SET MEDIA FILEPATH, DOES NOT OPEN
 ********************************************/

void Input::setFile(string file){
    filepath = file;
    source = MEDIA;
}

/******************************************
 SET MEDIA FILEPATH AND OPEN
 ********************************************/
    
void Input::openFile(string file){
    setFile(file);
    setup();
}

/******************************************
 SET INPUT FRAMERATE
 ********************************************/
    
void Input::setFramerate(int frate){
    framerate = frate;
    capture.setFramerate(frate);
}

/******************************************
 LOAD XML SETTINGS
 ********************************************/

void Input::loadXML(ofXml &xml) {
    string v;
    
    if (xml.exists("[@resolution]")){
        setResolution(ofToInt( xml.getAttribute("[@resolution]")));
    }
    
    if (xml.exists("input[@framerate]")){
        setFramerate(ofToInt( xml.getAttribute("[@framerate]")));
    }
    
    if (xml.exists("input[@file]")){
        setFile(ofToString(xml.getAttribute("input[@file]")));
    }
    
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
}

/******************************************
 SAVE XML SETTINGS
 ********************************************/
    
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

    xml.setAttribute("source", str);
    xml.setAttribute("file", filepath);
    xml.setAttribute("framerate", ofToString(framerate));

    if (getLoop()) xml.setAttribute("loop", "on");
    else           xml.setAttribute("loop", "off");

    xml.setToParent();
}
 
/******************************************
 GET PIXELS FROM INPUT SOURCE
 ********************************************/
    
ofPixels& Input::getPixels(){
    if      (source == MEDIA)       return media.getPixels();
    else if (source == CAPTURE)     return capture.getPixels();
    else if (source == GRID)        return media.getPixels();
    else if (source == BLACK)       return color.getPixels();
    else if (source == WHITE)       return color.getPixels();
    else if (source == GREY)        return color.getPixels();
    else if (source == COLOR)       return color.getPixels();
}
    
/******************************************
 BIND INPUT TEXTURE
 ********************************************/
    
void Input::bind(){
    if      (source == MEDIA)       return media.bind();
    else if (source == CAPTURE)     return capture.bind();
    else if (source == GRID)        return media.bind();
    else if (source == BLACK)       return color.bind();
    else if (source == WHITE)       return color.bind();
    else if (source == GREY)        return color.bind();
    else if (source == COLOR)       return color.bind();
}

/******************************************
 UNBIND INPUT TEXTURE
 ********************************************/
    
void Input::unbind() {
	if (source == MEDIA)			return media.unbind();
	else if (source == CAPTURE)     return capture.unbind();
	else if (source == GRID)        return media.unbind();
	else if (source == BLACK)       return color.unbind();
	else if (source == WHITE)       return color.unbind();
	else if (source == GREY)        return color.unbind();
	else if (source == COLOR)       return color.unbind();
}

    
}//////////////
