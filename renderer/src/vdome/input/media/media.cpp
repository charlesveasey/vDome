#include "media.h"
using namespace vd;
/******************************************
 MEDIA
 A wrapper for image and video media types
 *******************************************/

Media::Media() {
    width = 2048;
    height = 2048;
    bEnded = false;
    vol = 1;
}

/******************************************
 OPEN
 *******************************************/
void Media::open(string filepath){
    fpath = filepath;
    stop();
    close();
    bEnded = false;

    string mString = parseFile(filepath);
    if (mString == "image"){
        mType = IMAGE;
        image.open(filepath);
    }
    else if (mString == "video"){
        mType = VIDEO;
        video.setLoop(bLoop);
        video.open(filepath);
        video.setVolume(vol);
    }
}

/******************************************
 OPEN
 *******************************************/
void Media::update(){
	switch (mType) {
		case IMAGE: break;
		case VIDEO:
			video.update();
			if (video.getIsMovieDone()) {
				bool end = true;
				videoEnd(end);
			}
		break;
    }
}

void Media::play(){
    switch (mType) {
        case IMAGE: break;
        case VIDEO: video.play(); break;
    }
}

void Media::stop(){
    switch (mType) {
        case IMAGE: break;
        case VIDEO: video.stop(); break;
    }
}

void Media::close(){
    switch (mType) {
        case IMAGE: image.close(); break;
        case VIDEO: video.close(); break;
    }
}

void Media::seek(float f){
    switch (mType) {
        case IMAGE: break;
        case VIDEO: video.seek(f); break;
    }
}

void Media::setLoop(bool lp){
    bLoop = lp;
    video.setLoop(lp);
}

bool Media::getLoop(){
    return bLoop;
}

bool Media::isPlaying(){
    return video.isPlaying();
}

float Media::getPosition(){
	if (mType == VIDEO) 
		return video.getPosition();
    return 0;
}

float Media::getDuration(){
    if (mType == VIDEO)
        return video.getDuration();
	return 0;
}

void Media::setResolution(int w, int h){
    width = w;
    height = h;
}

void Media::videoEnd(bool &end){
    bEnded = true;
    ofNotifyEvent(endEvent,bEnded,this);
}

string Media::getFilepath(){
    return fpath;
}

void Media::setVolume(float v){
    vol = v;
    video.setVolume(v);
}

string Media::parseFile(string filepath){
    ofFile oFile;
    oFile.open(filepath);
    string mString = "";
    string extension = ofToUpper(oFile.getExtension());
    if (isImageFile(extension))
        mString = "image";
    else
        mString = "video";
    oFile.close();
    return mString;
}

bool Media::isLoaded(){
	switch (mType) {
		case IMAGE: return image.isLoaded(); break;
		case VIDEO: return video.isLoaded(); break;
    }
}

float Media::getRealWidth(){
	switch (mType) {
		case IMAGE: return image.getWidth(); break;
        case VIDEO: return video.getWidth(); break;
    }
}

float Media::getRealHeight(){
	switch (mType) {
		case IMAGE: return image.getHeight(); break;
		case VIDEO: return video.getHeight(); break;
    }
}

ofPixels & Media::getPixels(){
    switch (mType) {
        case IMAGE: return image.getPixels(); break;
        case VIDEO: return video.getPixels(); break;
    }
}

void Media::bind() {
	switch (mType) {
		case IMAGE: image.bind(); break;
		case VIDEO: video.bind(); break;
	}
}

void Media::unbind() {
	switch (mType) {
		case IMAGE: image.unbind(); break;
		case VIDEO: video.unbind(); break;
	}
}

bool Media::isImageFile(string ext){
    for (auto i : ImageTypes){
        if (i == ext)
            return true;
    }
    return false;
}