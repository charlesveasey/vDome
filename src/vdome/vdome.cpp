#include "vdome.h"
#include "command.h"
namespace vd {

//--------------------------------------------------------------
int maxHistory = 25;
CommandHistory history;
Socket         socket;

//--------------------------------------------------------------
vdome::vdome() {
    //socket.input = &input;
    //input.socket = &socket;
    vsync = true;
    framerate = 60;
#ifdef TARGET_OSX
    cKey = OF_KEY_COMMAND;
#else
    cKey = OF_KEY_CONTROL;
#endif
}
    
//--------------------------------------------------------------
void vdome::setup(){
    // remove esc quits
    ofSetEscapeQuitsApp(false);
    
    // format xml settings path
    xmlPath = "settings.xml";
    
    // load xml settings from path
    if (xml.load(ofToDataPath(xmlPath)))
        loadXML();
    
    // set up input
    setupInput();
    
    // add event listeners
    //ofAddListener(ofEvents().update, this, &vdome::update, 0);
    ofAddListener(Menu::colorEvent, this, &vdome::onColorEvent);
    ofAddListener(Menu::sourceColorEvent, this, &vdome::onSourceColorEvent);
    ofAddListener(Window::keyPressEvent, this, &vdome::keyPressed);
    ofAddListener(Window::keyReleaseEvent, this, &vdome::keyReleased);
    ofAddListener(Window::updateEvent, this, &vdome::update);
    ////////////
    //ofAddListener(Socket::sourceEvent, this, &vdome::onSourceEvent);
    //ofAddListener(Socket::formatEvent, this, &vdome::onFormatEvent);
    
    // push xml to save thread
    saveThread.xml.push_back(&xml);
    saveThread.files.push_back(xmlPath);
    
    // set warp xml file path
    string warpPath = ofToDataPath("settings");
    warpPath = ofFilePath::addTrailingSlash(warpPath);
    warpPath += "warp";
    warpPath = ofFilePath::addTrailingSlash(ofToDataPath(warpPath));
    
    //fix: save thread
    for (auto w : windows){
        for(int i=0; i<w->projectors.size(); i++) {
            //saveThread.xml.push_back(w->projectors[i].plane.wXml);
            //fix
            //saveThread.files.push_back(wp);
        }
    }
    
    // start main of loop
    ofRunMainLoop();
}
    
//--------------------------------------------------------------
void vdome::update(int &n) {
    
    // input update
    input.update();
    
    // socket update
    if (socket.enabled)
        socketUpdate();
    
    /*if (input.source == input.MEDIA) {
        if (!input.durationSent) {
            if (input.media.getDuration() > 0 && input.media.isLoaded()) {
                socket.sendDuration();
                input.durationSent = true;
            }
        }
    }*/
    
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
    
//--------------------------------------------------------------
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
    
//--------------------------------------------------------------
void vdome::keyReleased(int &key){
    for (auto w : windows){
        w->menu.keyReleased(key);
    }
}
    
//--------------------------------------------------------------
void vdome::onColorEvent(ofVec3f &color) {
    input.source = input.COLOR;
    input.setColor(color[0], color[1], color[2]);
    input.setup();
}
    
//--------------------------------------------------------------
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

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void vdome::createWindow(ofXml &xml){
    ofGLFWWindowSettings settings;
    int projectorIndex = 0;
    int numProjectors = 0;
	bool border = false;
	string str;
    
    if (xml.exists("window")) {
        //get window count from xml
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
            
            // need some bogus resolution or there is a rendering error with default (1024 x 768) and multiple windows
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

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void vdome::onSourceEvent(int &s){
    input.stop();
    input.setSourceInt(s);
    setupInput();
}
    
//--------------------------------------------------------------
void vdome::onSourceColorEvent(int &s){
    input.setSourceInt(s);
    setupInput();
}
    
//--------------------------------------------------------------
void vdome::onFormatEvent(int &s){
    input.setFormatInt(s);
    updateInputFormat();
}

//--------------------------------------------------------------
void vdome::updateInputFormat(){
    //input.setFormat();
    
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
            w->model.textureTiltInternal = -50;
            w->model.setup();
        }
    }
}

//--------------------------------------------------------------
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
    
//--------------------------------------------------------------
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
    
//--------------------------------------------------------------
void vdome::exit(){
    saveThread.waitForThread(true);
    input.stop();
    input.close();
}

void vdome::socketUpdate(){
    // receive
    while(socket.oscReceiver.hasWaitingMessages()){
        socket.rMsg.clear();
        socket.oscReceiver.getNextMessage(&socket.rMsg);
        
        if (socket.rMsg.getAddress() == "/input/"){
            if (socket.rMsg.getArgAsString(0) == "play")
                input.play();
            else if (socket.rMsg.getArgAsString(0) == "stop")
                input.stop();
        }
        else if (socket.rMsg.getAddress() == "/input/loop/") {
            if (socket.rMsg.getArgAsString(0) == "on")
                input.setLoop(true);
            else
                input.setLoop(false);
        }
        else if (socket.rMsg.getAddress() == "/input/seek/") {
            input.seek(ofToFloat(socket.rMsg.getArgAsString(0)));
        }
        else if (socket.rMsg.getAddress() == "/input/source/") {
            int s = input.convertSourceString(socket.rMsg.getArgAsString(0));
            //FIX
            //ofNotifyEvent(sourceEvent,s,this);
        }
        else if (socket.rMsg.getAddress() == "/input/file/") {
            ofFile oFile;
            oFile.open(socket.rMsg.getArgAsString(0));
            string file = oFile.getAbsolutePath();
            input.openFile(file);
        }
        else if (socket.rMsg.getAddress() == "/input/volume/") {
            input.setVolume(ofToFloat(socket.rMsg.getArgAsString(0)));
        }
        else if (socket.rMsg.getAddress() == "/input/format/") {
            int s = input.convertFormatString(socket.rMsg.getArgAsString(0));
            ///ofNotifyEvent(formatEvent,s,this);
        }
        
        // transform
        else if (socket.rMsg.getAddress() == "/input/flip/") {
            string s = socket.rMsg.getArgAsString(0);
            for (auto w : windows){
                if (socket.rMsg.getArgAsString(0) == "on")
                    w->model.setTextureFlip(true);
                else
                    w->model.setTextureFlip(false);
            }
        }
        else if (socket.rMsg.getAddress() == "/input/scale/") {
            float f = ofToFloat(socket.rMsg.getArgAsString(0));
            for (auto w : windows){
                w->model.setTextureScale(f);
            }
        }
        else if (socket.rMsg.getAddress() == "/input/rotate/") {
            float f = ofToFloat(socket.rMsg.getArgAsString(0));
            for (auto w : windows){
                w->model.setTextureRotate(f);
            }
        }
        else if (socket.rMsg.getAddress() == "/input/tilt/") {
            float f = ofToFloat(socket.rMsg.getArgAsString(0));
            for (auto w : windows){
                w->model.setTextureTilt(f);
            }
        }
        
 
        // projector
        else if (socket.rMsg.getAddress() == "/projector/polar/") {
            string f = socket.rMsg.getArgAsString(0);
            for (auto w : windows){
                for (int i=0;i<w->projectors.size(); i++){
                    float a = ofToFloat(ofSplitString(f, ",")[0]);
                    float e = ofToFloat(ofSplitString(f, ",")[1]);
                    cout << a << endl;
                    cout << e << endl;
                     w->projectors[i].setPolar(a, e, 1);
                }
            }
        }

        
    }
    
    // send position
    /*sMsg.clear();
     if (input->getSourceInt() == input->MEDIA) {
     if (lastInputPosition != input->getPosition()){
     sMsg.setAddress("/input/position");
     sMsg.addStringArg(ofToString( input->getPosition() ));
     oscSender.sendMessage(sMsg);
     }
     lastInputPosition = input->getPosition();
     }*/
}
    
}//////


