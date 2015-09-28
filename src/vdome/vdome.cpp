#include "vdome.h"
#include "command.h"
namespace vd {
        
// global variables
int maxHistory = 25;
CommandHistory history;
vector<ofPixels> maskHistory;
    
/******************************************

 CONSTRUCTOR

 ********************************************/

vdome::vdome() {
    socket.input = &input;
    input.socket = &socket;
    
#ifdef TARGET_OSX
    cKey = OF_KEY_COMMAND;
#else
    cKey = OF_KEY_CONTROL;
#endif
    
    vsync = true;
    framerate = 60;
}

/******************************************

 SETUP

 ********************************************/

void vdome::setup(){
    // remove esc quits
    ofSetEscapeQuitsApp(false);
    
    // format xml settings path
    string dataPath = "data";
    dataPath = ofFilePath::addLeadingSlash(dataPath);
    boost::filesystem::path fullPath( boost::filesystem::current_path() );
    dataPath = fullPath.string() + dataPath;
    dataPath = ofFilePath::addTrailingSlash(dataPath);
    
    xmlPath = "settings.xml";
    xmlPath = dataPath + xmlPath;
    
    // load xml settings from path
    if (xml.load(xmlPath)){
        loadXML();
    }

    setupInput();
    
    ofAddListener(ofEvents().update, this, &vdome::update, 0);
    ofAddListener(Menu::colorEvent, this, &vdome::onColorEvent);    
    ofAddListener(Window::keyPressEvent, this, &vdome::keyPressed);
    ofAddListener(Socket::sourceEvent, this, &vdome::onSourceEvent);
    ofAddListener(Socket::formatEvent, this, &vdome::onFormatEvent);

    // push xml to save thread
    saveThread.xml.push_back(&xml);
    saveThread.files.push_back(xmlPath);
    
    string warpPath = dataPath + "settings";
    warpPath = ofFilePath::addTrailingSlash(warpPath);
    warpPath += "warp";
    warpPath = ofFilePath::addTrailingSlash(warpPath);
    
    string maskPath = dataPath + "settings";
    maskPath = ofFilePath::addTrailingSlash(maskPath);
    maskPath += "masks";
    maskPath = ofFilePath::addTrailingSlash(maskPath);
    
    for (auto w : windows){
        for(int i=0; i<w->projectors.size(); i++) {
            string wp = warpPath + "warp-"+ofToString(i+w->projectorStartingIndex+1)+".xml";
            string mp = maskPath + "mask-"+ofToString(i+w->projectorStartingIndex+1)+".png";            
            saveThread.xml.push_back(w->projectors[i].plane.wXml);
            saveThread.files.push_back(wp);
            saveThread.image.push_back(w->projectors[i].mask.maskFboImage);
            saveThread.imageFiles.push_back(mp);
        }
    }
    
    // start main of loop
    ofRunMainLoop();
}
    
/******************************************
 
 KEYBOARD
 
 ********************************************/

void vdome::keyPressed(int &key){

    for (int i=0; i<windows.size(); i++){
        windows[i]->menu.keyPressed(key);
        
        if (key == 109){  // m = show menu
             windows[i]->menu.toggle();
            
            if (windows[i]->menu.active) {
                  baseWindows[i]->showCursor();
            }

			else {
                  baseWindows[i]->hideCursor();
            }
        }
    }
    
    if (ofGetKeyPressed(cKey) && (ofGetKeyPressed(115) || ofGetKeyPressed(19))) { // ctrl + s = save
        saveXML();
    }
}
    
void vdome::keyReleased(int &key){
    for (auto w : windows){
        w->menu.keyReleased(key);
    }
}
    

/******************************************
 
 EVENTS
 
 ********************************************/

void vdome::onColorEvent(ofVec3f &color) {
    input.source = input.COLOR;
    input.setColor(color[0], color[1], color[2]);
    input.setup();
}
    
/******************************************

 UPDATE

 ********************************************/

void vdome::update(ofEventArgs & args) {
    input.update();
    
    if (socket.enabled){
        socket.update();
    }
    
    if (input.source == input.MEDIA) {
        if (!input.durationSent) {
            if (input.media.getDuration() > 0 && input.media.isLoaded()) {
                socket.sendDuration();
                input.durationSent = true;
            }
        }
    }
    
#ifdef TARGET_OSX
    else if (input.source == input.SYPHON){
        updateSyphonInputTransform();
    }
#endif
    
    if (saveThread.saved) {
        for (auto w : windows){
            if (w->menu.active) {
                w->menu.saved = true;
                saveThread.saved = false;
            }
        }
    }
}

    
/******************************************

 SETTINGS

 *******************************************/

void vdome::loadXML(){
    if (xml.exists("[@framerate]")){
        framerate = ofToInt( xml.getAttribute("[@framerate]") );
    }
    if (xml.exists("[@vsync]")) {
        string str = ofToString( xml.getAttribute("[@vsync]") );
        if (str == "on")    vsync = true;
        else                vsync = false;
    }
    
    createWindow(xml);
    input.loadXML(xml);
    socket.loadXML(xml);
    
    for (int i=0; i<windows.size(); i++) {
        ofGetMainLoop()->setCurrentWindow(baseWindows[i]);
        windows[i]->setFrameRate(framerate);
        windows[i]->setVSync(vsync);
        windows[i]->loadXML(xml);
    }
    ofGetMainLoop()->setCurrentWindow(baseWindows[0]);
}

void vdome::saveXML(){        
    socket.saveXML(xml);
    input.saveXML(xml);
    
    for (int i=0; i<windows.size(); i++) {
        if (vsync)  xml.setAttribute("vsync", "on" );
        else        xml.setAttribute("vsync", "off" );
        
        xml.setAttribute("framerate", ofToString(framerate) );
        windows[i]->saveXML(xml);
    }
    
    saveThread.save();
}

/******************************************

 EXIT

 ********************************************/

void vdome::exit(){
    saveThread.waitForThread(true);
    input.stop();
    input.close();
}

/******************************************
 
 CREATE WINDOW
 
 ********************************************/
    
void vdome::createWindow(ofXml &xml){
    ofGLFWWindowSettings settings;
    int projectorIndex = 0;
    int numProjectors = 0;
	bool border = false;
	string str;
    
    if (xml.exists("window")) {
        //get window count form xml
        int windowCount = xml.getNumChildren("window");
        
        // create system windows
        for (int i=0; i<windowCount; i++) {
            xml.setTo("window["+ ofToString(i) + "]");
            
			// get border setting from xml
			if (xml.exists("[@border]")) {
				str = ofToString(xml.getAttribute("[@border]"));
				if (str == "on")	border = true;
			}

            // GLFW window settings
            settings.glVersionMajor = 3;
            settings.glVersionMinor = 2;
            settings.resizable = false;
            settings.decorated = border;
            
            if (i > 0){
                settings.shareContextWith = baseWindows[0];
            }
            
            // needs some bogus resolution or there is a rendering error with default (1024 x 768) and multiple windows
            settings.width = 1;
            settings.height = 1;
            
            // create GLFW window
            shared_ptr<ofAppBaseWindow> win = ofCreateWindow(settings);
            
            // create ofApp - Window
            shared_ptr<Window> app(new Window);
            
            // store an incremental index on the ofApp - Window
            app->index = i;
            app->input = &input; //fix
            app->projectorStartingIndex = projectorIndex;
            app->menu.projectorStartingIndex = projectorIndex;
 
			if (i == 0) {
				socket.model = &app->model;
			}

            // create a list of rendering windows
            windows.push_back(app);
            baseWindows.push_back(win);


            // register new window to ofMainLoop
            ofRunApp(win, app);
            
            // starting index of each window's projector
            numProjectors = xml.getNumChildren("projector");
            projectorIndex += numProjectors;
        }
    }
    xml.setToParent();
}

/******************************************
 
 SETUP INPUT
 
 ********************************************/

void vdome::onSourceEvent(int &s){
	input.stop();
    input.setSourceInt(s);
    setupInput();
}

void vdome::onFormatEvent(int &s){
    input.setFormatInt(s);
    updateInputFormat();
}

void vdome::onSourceColorEvent(int &s) {
    input.setSourceInt(s);
    input.setup(); //fix: do i need this?? how many times is this firing
}
    
void vdome::setupInput(){
    // setup input
    input.setup();
  
    for (auto w : windows){
        w->menu.inputSource = input.source;
    }
    
    // setup syphon
#ifdef TARGET_OSX
    if (input.source == input.SYPHON){
        for (auto w : windows){
            w->syphon.setup();
        }
    }
#endif

	// setup spout
#ifdef TARGET_WIN32
	if (input.source == input.SPOUT) {
		for (auto w : windows) {
			w->spout.setup();
		}
	}
#endif
    
    // configure input transform
    if (input.ratio == -99999){
        updateInputTransform();
    }
}
    
/******************************************
 
 UPDATE INPUT FORMAT
 
 ********************************************/
    
void vdome::updateInputFormat(){
    input.setFormat();
    
    if (input.format == input.DOMEMASTER){
        for (auto w : windows) {
            w->model.textureScaleInternal = 1;
            w->model.textureTiltInternal = 0;
            w->model.setup();
        }
    }
    else if (input.format == input.HD){
        for (auto w : windows) {
            w->model.textureScaleInternal = .5625;
            w->model.textureTiltInternal = 50;
            w->model.setup();
        }
    }
}
    
/******************************************
 
 UPDATE INPUT TRANSFORM
 
 ********************************************/
    
void vdome::updateInputTransform(){
    for (auto w : windows) {
        // reset model internal transform
        w->model.textureScaleInternalW = 1;
        w->model.textureScaleInternalH = 1;
        
    #ifdef TARGET_OSX
        if (input.source == input.SYPHON){
            if (w->syphon.isSetup()){
                input.ratio = w->syphon.getHeight()/w->syphon.getWidth();
            }
        }
    #endif
        
        if (input.ratio != -99999){
            if (input.ratio < 1){
                w->model.textureScaleInternalW = 1;
                w->model.textureScaleInternalH = input.ratio;
            }
            else if (input.ratio > 1){
                w->model.textureScaleInternalW = 1/input.ratio;
                w->model.textureScaleInternalH = 1;
            }
        }
        
        if (input.source == input.CAPTURE || input.source == input.SYPHON){
            w->model.textureFlipInternal = true;
            w->model.setup();
        }
        else {
            if (input.source == input.GRID || input.source == input.BLACK ||
                input.source == input.WHITE || input.source == input.GREY ||
                input.source == input.COLOR){
                
                w->model.textureFlipInternal = false;
                
                updateInputFormat();
            }
            else {
                w->model.textureFlipInternal = false;
                w->model.setup();
            }
        }
    }
}
    
    
    
    
/******************************************
 
 UPDATE SYPHON INPUT TRANSFORM
 
 ********************************************/
#ifdef TARGET_OSX
void vdome::updateSyphonInputTransform(){
    float nRatio;
        for (auto w : windows){
            if (w->syphon.isSetup()){
                nRatio = w->syphon.getHeight()/w->syphon.getWidth();
                if (nRatio != input.ratio){
                    input.ratio = nRatio;
                    
                    if (input.ratio < 1){
                        w->model.textureScaleInternalW = 1;
                        w->model.textureScaleInternalH = input.ratio;
                    }
                    else if (input.ratio > 1){
                        w->model.textureScaleInternalW = 1/input.ratio;
                        w->model.textureScaleInternalH = 1;
                    }
                    else {
                        w->model.textureScaleInternalW = 1;
                        w->model.textureScaleInternalH = 1;
                    }
                    
                    w->model.setup();
                }
            }
        }
    }
#endif
}
