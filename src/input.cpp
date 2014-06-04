#include "input.h"
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Input::Input(){
    maxSource = 5;
    resolution = 2048;
    frameRate = 30;
    loop = false;
	usePbo = false;
    isVideo = false;
	nFrame = false;
    mediaTypeMap = new MediaTypeMap("media/mime.types");
    file = "";
    fileIndex = 0;
    imageDuration = 10;

    #ifdef TARGET_OSX
        maxSource += 1;
        vRenderer = QT;
    #endif
	#ifdef TARGET_WIN32
		ofxWMFVideoPlayer wmf;
		vRenderer = WMF;
		if (vRenderer == WMF) {
		}
		else if (vRenderer == DS) {
			ofPtr <ofBaseVideoPlayer> ptr(new ofDirectShowPlayer());
			video.setPlayer(ptr);
		}
		else if (vRenderer == GST) {
			ofPtr <ofGstVideoPlayer> ptr(new ofGstVideoPlayer());
			video.setPlayer(ptr);
			ofAddListener(ptr->getGstVideoUtils()->bufferEvent,this,&Input::newFrame);
			if (usePbo)
				video.setUseTexture(false);
		}
	#endif
}

/******************************************

 SETUP

 ********************************************/

void Input::setup(){
    
    stop();
	close();
    texture.clear();

    if (usePbo) { // not reallocating causing issues with PBO
        if (texture.getWidth() != resolution || texture.getHeight() != resolution) {
            texture.allocate(resolution, resolution, GL_RGB);
            if (usePbo)
                pbo.allocate(texture,2);
        }
    }

    // create input
    switch(source){
            
        case BLACK: fillTexture(ofColor(0));
            break;
 
        case WHITE: fillTexture(ofColor(255));
            break;
            
        case GREY: fillTexture(ofColor(128));
            break;
            
        case GRID: 
			isVideo = false;
			image.loadImage("media/warp/grid-2k.png");
            texture = image.getTextureReference();
            break;

        case MEDIA: 
            if (isVideo) {
                
                #ifdef TARGET_OSX                
					if (vRenderer == AVF){
						avf.loadMovie(file);
                        avf.play();
					}
					else if (vRenderer == QT2){
						qt.loadMovie(file, OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
						qt.play();
					}
					else if (vRenderer == HAP){
						hap.loadMovie(file, OF_QTKIT_DECODE_TEXTURE_ONLY);
						hap.play();
					}
					else if (vRenderer == QT){
						video.loadMovie(file);
						video.play();
						texture = video.getTextureReference();
					}
				#endif

				#ifdef TARGET_WIN32
					if (vRenderer == WMF) {
						wmf.loadMovie(file);
						wmf.play();
                        if (loop) wmf.setLoop(true);
                        else wmf.setLoop(false);
					}
					else if (vRenderer == DS) {
						video.loadMovie(file);
						video.play();
						texture = video.getTextureReference();
					}
					else if (vRenderer == GST) {
						video.loadMovie(file);
						video.play();
						if (!usePbo)
							texture = video.getTextureReference();
					}
					else if (vRenderer == QT){
						video.loadMovie(file);
						video.play();
						texture = video.getTextureReference();
					}
				#endif

                #ifdef TARGET_LINUX
					video.setPixelFormat(OF_PIXELS_RGB);
					video.loadMovie(file);
					video.play();
					texture = video.getTextureReference();
                #endif
                
                setLoop(loop);
            }
            else {
                image.loadImage(file);
                texture = image.getTextureReference();
                if (imageDuration > 0 && fileList.size() > 1)  startTimer();
            }
            break;
            
        case CAPTURE:
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(frameRate);
            capture.initGrabber(resolution, resolution, true);
            texture = capture.getTextureReference();
            break;
            
        case SYPHON:
			#ifdef TARGET_OSX
				syphon.setup();
			#endif
            break;
            
        default:
			image.loadImage("media/warp/grid-2k.png");
            texture = image.getTextureReference();
            break;
    }

}

/******************************************

 CONTROLS

 ********************************************/

void Input::play() {
    if (isVideo) {
		#ifdef TARGET_OSX
			if      (vRenderer == AVF)  avf.play();
			else if (vRenderer == QT2)  qt.play();
			else if (vRenderer == HAP)  hap.play();
			else if (vRenderer == QT)   video.play();
		#endif
		#ifdef TARGET_WIN32
			if		(vRenderer == WMF)	wmf.play();
			else						video.play();
		#endif
		#ifdef TARGET_LINUX	
			video.play();
		#endif
    }
    else {
        if (fileIndex >= fileList.size())
            fileIndex = 0;
        startTimer();
    }
}

bool Input::isPlaying() {
	bool isP = false;
	#ifdef TARGET_OSX
		if		(vRenderer == AVF)	isP = avf.isPlaying();
		else if (vRenderer == QT2)	isP = qt.isPlaying();
		else if (vRenderer == HAP)	isP = hap.isPlaying();
		else if (vRenderer == QT)	isP = video.isPlaying();
	#endif
	#ifdef TARGET_WIN32
		if		(vRenderer == WMF)	isP = wmf.isPlaying();
		else						isP = video.isPlaying();
	#endif
	#ifdef TARGET_LINUX				
		isP = video.isPlaying();
	#endif
	return isP;
}
    
void Input::toggle() {
	if (isVideo) {    
		if (!isPlaying())	 play();
		else				 stop();
    }
    else {
        if (timerRunning)   play();
        else                stop();
    }
}
    
void Input::stop() {
    if (capture.isInitialized())
        capture.close();

	#ifdef TARGET_OSX
		hap.stop();
        qt.stop();
        avf.stop();
	#endif
	#ifdef TARGET_WIN32
		if (wmf.isPlaying())
			wmf.stop();
    #endif
	video.stop();
}

void Input::close() {
    image.clear();
    video.close();
    if (capture.isInitialized())
        capture.close();
    #ifdef TARGET_OSX
        hap.closeMovie();
        qt.close();
        avf.closeMovie();
    #endif
	#ifdef TARGET_WIN32
		//wmf.close();
    #endif
}
    
void Input::prev() {
    getPrevFile();
}
    
void Input::next() {
    getNextFile();
}

void Input::seek(float f) {
	if (isVideo) {
		#ifdef TARGET_OSX
			if		(vRenderer == AVF)  avf.setPosition(f/1000);
			else if (vRenderer == QT2)  qt.setPosition(f/1000);
			else if (vRenderer == HAP)  hap.setPosition(f/1000);
			else if (vRenderer == QT)   video.setPosition(f/1000);
		#endif
		#ifdef TARGET_WIN32
			if		(vRenderer == WMF)	wmf.setPosition(f/1000);
			else						video.setPosition(f/1000);
		#endif
		#ifdef TARGET_LINUX	
			video.setPosition(f/1000);
		#endif
    }
}

void Input::prevFrame() {
    if (isVideo) {
		#ifdef TARGET_OSX
			if      (vRenderer == AVF)  avf.previousFrame();
			else if (vRenderer == QT2)  qt.previousFrame();
			else if (vRenderer == HAP)  hap.previousFrame();
			else if (vRenderer == QT)   video.previousFrame();
		#endif
		#ifdef TARGET_WIN32
			if		(vRenderer == WMF)	return;
			else						video.previousFrame();
		#endif
		#ifdef TARGET_LINUX	
			video.previousFrame();
		#endif
    }
}

void Input::nextFrame() {
    if (isVideo) {
		#ifdef TARGET_OSX
			if      (vRenderer == AVF)  avf.nextFrame();
			else if (vRenderer == QT2)  qt.nextFrame();
			else if (vRenderer == HAP)  hap.nextFrame();
			else if (vRenderer == QT)   video.nextFrame();
		#endif
		#ifdef TARGET_WIN32
			if		(vRenderer == WMF)	return;
			else						video.nextFrame();
		#endif
		#ifdef TARGET_LINUX
			video.nextFrame();
		#endif
    }
}
    
bool Input::getLoop() {
    return loop;
}

void Input::setLoop(bool b) {
    loop = b;
    ofLoopType v;
    
    if (loop && fileList.size() <= 1)
        v = OF_LOOP_NORMAL;
    else
        v = OF_LOOP_NONE;
    
	#ifdef TARGET_OSX
		if      (vRenderer == AVF)  avf.setLoopState(v);
		else if (vRenderer == QT2)  qt.setLoopState(v);
		else if (vRenderer == HAP)  hap.setLoopState(v);
		else if (vRenderer == QT)   video.setLoopState(v);
	#endif
	#ifdef TARGET_WIN32
		if		(vRenderer == WMF)	wmf.setLoop(v);
		else						video.setLoopState(v);
	#endif
	#ifdef TARGET_LINUX
		video.setLoopState(v);
	#endif
}

string Input::getSource() {
    return "";
}

void Input::setSource(string s) {
    if      (s == "media")      source = MEDIA;
    else if (s == "capture")    source = CAPTURE;
    else if (s == "syphon")     source = SYPHON;
    else if (s == "grid")       source = GRID;
    else if (s == "black")      source = BLACK;
    else if (s == "white")      source = WHITE;
    else if (s == "grey")       source = GREY;
}

string Input::getFile() {
    return file;
}

void Input::setFile(string s) {
    file = s;
}

float Input::getPosition() {
    float pos = 0.0;
	#ifdef TARGET_OSX
		if      (vRenderer == AVF)  pos = avf.getPosition();
		else if (vRenderer == QT2)  pos = qt.getPosition();
		else if (vRenderer == HAP)  pos = hap.getPosition();
		else if (vRenderer == QT)   pos = video.getPosition();
	#endif
	#ifdef TARGET_WIN32
		if		(vRenderer == WMF)	wmf.getPosition();
		else						video.getPosition();
	#endif
	#ifdef TARGET_LINUX
	#endif
    return pos;
}
    
/******************************************

 BIND

 ********************************************/

void Input::bind(){
    texture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

	if (source == GRID || source == BLACK ||
        source == WHITE || source == GREY) {
		texture.bind();
	}
	else if (source == MEDIA) {
        #ifdef TARGET_OSX
            if (isVideo) {
                if (vRenderer == AVF)
                    avf.getTextureReference().bind();
                else if (vRenderer == QT2)
                    qt.getTexture()->bind();
                else if (vRenderer == HAP)
                    hap.getTexture()->bind();
                else if (vRenderer == QT)
                    texture.bind();
            }
            else
                texture.bind();
		#endif

		#ifdef TARGET_WIN32
			if (isVideo) {
				if (vRenderer == WMF)
					wmf.bind();
				else if (vRenderer == DS || vRenderer == GST)
					texture.bind();
				else if (vRenderer == QT)
                    texture.bind();
			}
			else
				texture.bind();
		#endif

        #ifdef TARGET_LINUX
            texture.bind();
        #endif
	}
    
    if (source == CAPTURE)
        texture.bind();
    
    #ifdef TARGET_OSX
        if (source == SYPHON)
            syphon.bind();
    #endif
}

void Input::unbind(){
 
	if (source == GRID || source == BLACK ||
        source == WHITE || source == GREY) {
		texture.unbind();
	}

	else if (source == MEDIA) {
        #ifdef TARGET_OSX
            if (isVideo) {
                if (vRenderer == AVF)
                    avf.getTextureReference().unbind();
                else if (vRenderer == QT2) {
                    qt.getTexture()->unbind();
                }
                else if (vRenderer == HAP)
                    hap.getTexture()->unbind();
                else if (vRenderer == QT)
                    texture.unbind();
            }
            else {
                texture.unbind();
            }
		#endif

		#ifdef TARGET_WIN32
			if (isVideo) {
				if (vRenderer == WMF)
					wmf.unbind();
				else if (vRenderer == DS || vRenderer == GST)
					texture.unbind();
				else if (vRenderer == QT)
					texture.unbind();
			}
			else {
				texture.unbind();
			}
		#endif

        #ifdef TARGET_LINUX
            texture.unbind();
        #endif
	}
    
    if (source == CAPTURE)
         texture.unbind();
    
	#ifdef TARGET_OSX
		if (source == SYPHON)
			syphon.unbind();
    #endif

}

/******************************************

 UPDATE

 ********************************************/

void Input::update(){
    
    if (source == MEDIA) {
        
        if (isVideo) {

            #ifdef TARGET_OSX
                if (vRenderer == AVF){
                    avf.update();
                    if (avf.isLoaded())
                        avf.play();
                }
                else if (vRenderer == QT2)
                    qt.update();
                else if (vRenderer == HAP)
                    hap.update();
                else if (vRenderer == QT) {
                    video.update();
                    if (video.getIsMovieDone()) {
                        getNextFile();
                    }

                }
			#endif

            #ifdef TARGET_LINUX
                video.update();
            #endif

			#ifdef TARGET_WIN32
				if (vRenderer == WMF)
					wmf.update();
				else if (vRenderer == DS) {
					video.update();	
				}
				else if (vRenderer == GST) {

					if (nFrame) {
						video.update();
					}

					if (usePbo && nFrame) {
						pbo.loadData(video.getPixelsRef());
						pbo.updateTexture();
						nFrame = false;
					}
				}
				else if (vRenderer == QT) {
					video.update();	
				}
			#endif
		}
        else {
            if (timerRunning)
                evalTimer();
        }
    }
    
	else if (source == CAPTURE) {
        capture.update();
	}
}

void Input::newFrame(ofPixels & pixels) {
	nFrame = true;
}


/******************************************

 KEYBOARD

 ********************************************/

void Input::keyPressed(int key) {
    switch (key) {
        case OF_KEY_RIGHT:
            
            switch (editMode) {
                case SOURCE:
                    if (source+1 > maxSource)
                        source = maxSource;
                    else {
                        source++;
                        #ifdef TARGET_WIN32
                        if (source == SYPHON)
                            source++;
                        #endif
                        #ifdef TARGET_LINUX
                        if (source == SYPHON)
                            source++;
                        #endif
                    }
                    if (source == MEDIA)
                        parseFileType(file);
                    setup();
                    break;
                    
                case LOOP:
                    setLoop(true);
                    break;
            }
            break;
        
        case OF_KEY_LEFT:
            switch (editMode) {

                case SOURCE:
                    if (source-1 < 0)
                        source = 0;
                    else {
                        source--;
                        #ifdef TARGET_WIN32
                        if (source == SYPHON)
                            source--;
                        #endif
                        #ifdef TARGET_LINUX
                        if (source == SYPHON)
                            source--;
                        #endif
                    }
                    if (source == MEDIA)
                        parseFileType(file);
                    setup();
                    break;
                
                case LOOP:
                    setLoop(false);
                    break;
            }
            break;
    }
}
    
/******************************************
 
 FILE OPEN EVENTS
 
 ********************************************/
    
void Input::dragEvent(ofDragInfo dragInfo){
    openMediaFile(dragInfo.files);
}

void Input::openFileDialog(){
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a media file");
	if (openFileResult.bSuccess) {
        vector<string> files;
        files.push_back(openFileResult.filePath);
        openMediaFile(files);
    }
}
    
/******************************************
 
 FILL
 
 ********************************************/

void Input::fillTexture(ofColor color){
    isVideo = false;
    image.clear();
    image.allocate(resolution, resolution, OF_IMAGE_COLOR);
    for (int x = 0; x < resolution; x++) {
        for (int y = 0; y < resolution; y++) {
            image.setColor(x, y, color);
        }
    }
    image.update();
    texture = image.getTextureReference();
}

/******************************************

 SETTINGS

 ********************************************/

void Input::loadXML(ofXml &xml) {
    string v;
    
    if (xml.exists("input[@resolution]"))
        resolution = ofToInt( xml.getAttribute("input[@resolution]") );

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

    if (xml.exists("input[@file]")) {
        parseFileType(ofToString( xml.getAttribute("input[@file]") ));
    }
    
    if (xml.exists("input[@loop]")) {
        v = xml.getAttribute("input[@loop]");
        if (v == "on") loop = true;
        else             loop = false;
    }
    
    if (xml.exists("input[@imageDuration]"))
        imageDuration = ofToFloat( xml.getAttribute("input[@imageDuration]") );
    
    if (xml.exists("input[@videoRenderer]")) {
        v = ofToString( xml.getAttribute("input[@videoRenderer]") );
        if (v == "avf")         vRenderer = AVF;
        else if (v == "qt2")    vRenderer = QT2;
        else if (v == "qt")     vRenderer = QT;
        else if (v == "hap")    vRenderer = HAP;
        else if (v == "wmf")    vRenderer = WMF;
        else if (v == "ds")     vRenderer = DS;
        else if (v == "gst")    vRenderer = GST;
    }
    
    setup();
}

void Input::saveXML(ofXml &xml) {
    xml.setTo("input");
    xml.setAttribute("resolution", ofToString(resolution));

    string str;

	if (source == GRID)			  str = "grid";
    else if (source == MEDIA)     str = "media";
    else if (source == CAPTURE)   str = "capture";
    else if (source == SYPHON)    str = "syphon";
    else if (source == BLACK)     str = "black";
    else if (source == WHITE)     str = "white";
    else if (source == GREY)      str = "grey";
    
    xml.setAttribute("source", str );
    xml.setAttribute("file", file );
    if (loop) xml.setAttribute("loop", "on");
    else      xml.setAttribute("loop", "off");
    
    xml.setAttribute("imageDuration", ofToString(imageDuration));
    xml.setToParent();
}

/******************************************
 
 IMAGE TIMER
 
 ********************************************/

void Input::startTimer(){
    startTime = ofGetElapsedTimeMillis();  // get the start time
    endTime = 1000 * imageDuration;
    timerRunning = true;
}
   
void Input::evalTimer(){
    float timer = ofGetElapsedTimeMillis() - startTime;
        
    if (timer >= endTime) {
        stopTimer();
        getNextFile();
    }
}
    
void Input::stopTimer(){
    startTime = 0;
    timerRunning = false;
}

/******************************************
 
 FILE
 
 ********************************************/

void Input::openMediaFile(vector<string> files){
    stop();
    fileList.clear();
    fileIndex = 0;
	source = MEDIA;
    
    for (int i=0; i<files.size(); i++) {
        fileList.push_back(files[i]);
    }
    if (fileList.size())
        parseFileType(fileList[0]);
}

void Input::getPrevFile(){
    if (fileList.size() > 1) {
        if (fileIndex >= 1) {
            fileIndex--;
            parseFileType(fileList[fileIndex]);
        }
        if (fileIndex < 1) {
            stopTimer();
        }
    }
}
    
void Input::getNextFile(){
    if (fileList.size() > 1) {
        fileIndex++;
        if (fileIndex >= fileList.size()) {
            if (loop)   fileIndex = 0;
        }
        
        if (fileIndex < fileList.size()) {
            parseFileType(fileList[fileIndex]);
        }
        
        if (fileIndex+1 >= fileList.size()) {
            if (!loop) stopTimer();
        }
    }
}
    
void Input::parseFileType(string filepath){
    oFile.open(filepath);
    pFile = oFile.getPocoFile();
	file = oFile.getAbsolutePath();
    filename = oFile.getFileName();

    if (oFile.isDirectory()) {
        ofDirectory dir;
        dir.listDir(file);
        dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
        if (dir.size()) {
            vector <string> files;
          	for(int i = 0; i < (int)dir.size(); i++){
                files.push_back(dir.getPath(i));
            }
            openMediaFile(files);
        }
    }
    else if (oFile.isFile()) {
        mediaType = mediaTypeMap->getMediaTypeForPath(pFile.path()).toString();
        string sub = ofSplitString(mediaType, "/")[0];
        
        if (oFile.getExtension() == "m3u") {
            playlist = filepath;
            parsePlaylist(playlist);
        }
        else {
            if (sub == "video")         isVideo = true;
            else if  (sub == "image")   isVideo = false;

            if (sub == "video" || sub == "image")
                setup();
            else
                getNextFile();
        }
    }
    
    oFile.close();
}
    
void Input::parsePlaylist(string filepath){
    m3u.load(filepath);
    vector<M3UItem> items = m3u.getItems();
    
    fileList.clear();
    
    for (int i=0; i<items.size(); i++) {
        fileList.push_back(items[i].file);
    }
    
    if (fileList.size()) {
        parseFileType(fileList[0]);
    }
}
    
}
