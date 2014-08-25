#include "media.h"
using namespace vd;

Media::Media() {
    mediaTypeMap = new MediaTypeMap("settings/media/mime.types");
    forceVideoRenderer = "";
    width = 2048;
    height = 2048;
    bEnded = false;
}

void Media::open(vector<string> filepaths){
    fList.clear();
    fIndex = 0;
    for (int i=0; i<filepaths.size(); i++) {
        fList.push_back(filepaths[i]);
    }
    if (fList.size())
        open(fList[0]);
}

void Media::open(string filepath){
    fpath = filepath;
    stop();
    close();
    bEnded = false;

    string mString = parseFile(filepath);
    if (mString == "image"){
        mType = IMAGE;
        
        if (fList.size() > 1) {
            image.setUseTimer(bSlideshow);
            ofAddListener(image.timerEvent, this, &Media::slideEnd);
        }
        else {
            image.setUseTimer(false);
        }
        image.open(filepath, width, height);

    }
    else if (mString == "video"){
        mType = VIDEO;
        video.forceVideoRenderer = forceVideoRenderer;
        ofAddListener(video.endEvent, this, &Media::videoEnd);
        
        if (fList.size() > 1)
            video.setLoop(false);
        else
            video.setLoop(bLoop);
        
        video.open(filepath);
    }
    else if (mString == "playlist"){
        parsePlaylist(filepath);
    }
}

void Media::update(){
    switch (mType) {
        case IMAGE: image.update(); break;
        case VIDEO: video.update(); break;
    }
}

void Media::bind(){
    switch (mType) {
        case IMAGE: image.bind(); break;
        case VIDEO: video.bind(); break;
    }
}

void Media::unbind(){
    switch (mType) {
        case IMAGE: image.unbind(); break;
        case VIDEO: video.unbind(); break;
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
        case IMAGE: image.stop();
                    ofAddListener(image.timerEvent, this, &Media::slideEnd);
            break;
        case VIDEO: video.stop(); break;
    }
}

void Media::close(){
    switch (mType) {
        case IMAGE: image.close();
                    ofRemoveListener(image.timerEvent, this, &Media::slideEnd);
            break;
        case VIDEO: video.close(); break;
    }
}

void Media::seek(float f){
    switch (mType) {
        case IMAGE: break;
        case VIDEO: video.seek(f); break;
    }
}

void Media::previous(){
    if (fList.size() <= 1)
        return;
    fIndex--;
    if (fIndex < 0){
        if (bLoop){
            fIndex = fList.size()-1;
        }
        else {
            stop();
            return;
        }
    }
    open(fList[fIndex]);
}

void Media::next(){
    if (fList.size() <= 1)
        return;
    fIndex++;
    if (fIndex >= fList.size()){
        if (bLoop){
            fIndex = 0;
        }
        else {
            stop();
            return;
        }
    }
    open(fList[fIndex]);
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
    float val = 0;
    if (mType == VIDEO)
        val = video.getPosition();
    return val;
}

float Media::getDuration(){
    float val = 0;
    if (mType == VIDEO)
        val = video.getDuration();
    return val;
}

void Media::setResolution(int w, int h){
    width = w;
    height = h;
}

void Media::setSlideshow(bool enable, int duration){
    bSlideshow = enable;
    image.setUseTimer(enable);
    image.setTimerDuration(duration);
}

void Media::slideEnd(bool &end){
    bEnded = true;
    ofNotifyEvent(endEvent,bEnded,this);
    next();
}

void Media::videoEnd(bool &end){
    bEnded = true;
    ofNotifyEvent(endEvent,bEnded,this);
    next();
}

string Media::getFilepath(){
    return fpath;
}

void Media::setVolume(float v){
    video.setVolume(v);
}

string Media::parseFile(string filepath){
    ofFile oFile;
    oFile.open(filepath);
    string mString = "";
    if (ofToLower(oFile.getExtension()) == "m3u"){
        mString = "playlist";
    }
    else{
        Poco::File pfile = oFile.getPocoFile();
        mString = mediaTypeMap->getMediaTypeForPath(pfile.path()).toString();
        mString = mString.substr(0,5);
        ofLogVerbose("Media") << "mime-type " << mString;
    }
    oFile.close();
    return mString;
}

void Media::parsePlaylist(string filepath){
    m3u.load(filepath);
    vector<M3UItem> items = m3u.getItems();
    vector<string> paths;
    for (int i=0; i<items.size(); i++) {
        paths.push_back(items[i].file);
    }
    if (paths.size())
        open(paths);
}

bool Media::isLoaded(){
	switch (mType) {
		case IMAGE: return image.isLoaded(); break;
		case VIDEO: return video.isLoaded(); break;
    }
}