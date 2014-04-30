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
	usePbo = true;
    isVideo = false;
	nFrame = false;
    mediaTypeMap = new MediaTypeMap("media/mime.types");
    file = "";

    #ifdef TARGET_WIN32
        ofxWMFVideoPlayer wmf;
    #endif
    
    #ifdef TARGET_OSX
        maxSource += 1;
        vRenderer = QT;
    #endif

	#ifdef TARGET_WIN32
		vRenderer = GST;
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

	#ifdef TARGET_LINUX
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
            
        case BLACK:
			fillTexture(ofColor(0));
            break;
 
        case WHITE:
			fillTexture(ofColor(255));
            break;
            
        case GREY:
			fillTexture(ofColor(128));
            break;
            
        case GRID: 
			isVideo = false;
			image.loadImage("media/warp/grid-2k.png");
            texture = image.getTextureReference();
            break;

        case MEDIA: 
            if (isVideo) {
                
                #ifdef TARGET_OSX
					//parseVideoCodec(file);
                
					if (vRenderer == AVF){
						avf.loadMovie(file);
                        avf.play();
                        if (loop) avf.setLoopState(OF_LOOP_NORMAL);
                        else avf.setLoopState(OF_LOOP_NONE);
					}
					else if (vRenderer == QT2){
						qt.loadMovie(file, OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
						qt.play();
                        if (loop) qt.setLoopState(OF_LOOP_NORMAL);
                        else qt.setLoopState(OF_LOOP_NONE);
					}
					else if (vRenderer == HAP){
						hap.loadMovie(file, OF_QTKIT_DECODE_TEXTURE_ONLY);
						hap.play();
                        if (loop) hap.setLoopState(OF_LOOP_NORMAL);
                        else hap.setLoopState(OF_LOOP_NONE);
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
					}
				#endif

                #ifdef TARGET_LINUX
					video.setPixelFormat(OF_PIXELS_RGB);
					video.loadMovie(file);
					video.play();
					texture = video.getTextureReference();
                #endif
                
                if (loop) video.setLoopState(OF_LOOP_NORMAL);
                else video.setLoopState(OF_LOOP_NONE);
            }
            else {
                image.loadImage(file);
                texture = image.getTextureReference();
            }
            break;
            
        case CAPTURE:
            fillTexture(ofColor(0));
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(frameRate);
            if (capture.initGrabber(resolution, resolution)) {
                texture = capture.getTextureReference();
            }
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

 STOP

 ********************************************/

void Input::stop() {
    video.stop();
    if(capture.isInitialized())
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
}

/******************************************

 CLOSE

 ********************************************/

void Input::close() {
    image.clear();
    video.close();
    if(capture.isInitialized())
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
    
/******************************************
 
 PLAY/PAUSE
 
 ********************************************/
    
void Input::togglePause() {
    if (!video.isPlaying()) {
        if (vRenderer == AVF){
            avf.play();
        }
        else if (vRenderer == QT2){
            qt.play();
        }
        else if (vRenderer == HAP){
            hap.play();
        }
        else if (vRenderer == QT){
            video.play();
        }
    }
    else{
        video.stop();
    }
}

/******************************************

 BIND

 ********************************************/

void Input::bind(){
    
	if (source == GRID || source == BLACK ||
        source == WHITE || source == GREY) {
		texture.bind();
	}
	else if (source == MEDIA) {
        #ifdef TARGET_OSX
            if (isVideo) {
                if (vRenderer == AVF)
                    avf.getTextureReference().bind();
                else if (vRenderer == QT2) {
                    qt.getTexture()->bind();
                }
                else if (vRenderer == HAP)
                    hap.getTexture()->bind();
                else if (vRenderer == QT)
                    texture.bind();
            }
            else {
                texture.bind();
            }
		#endif

		#ifdef TARGET_WIN32
			if (isVideo) {
				if (vRenderer == WMF)
					wmf.bind();
				else if (vRenderer == DS || vRenderer == GST)
					texture.bind();
			}
			else {
				texture.bind();
			}
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
                else if (vRenderer == QT)
                    video.update();
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
			#endif
		}
    }
    
	else if (source == CAPTURE)
        capture.update();
}

void Input::newFrame(ofPixels & pixels) {
	nFrame = true;
	//cout << " new frame" << endl;
}


/******************************************

 KEYBOARD

 ********************************************/

void Input::keyPressed(int key) {
    switch (key) {
        case OF_KEY_RIGHT:
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
        case OF_KEY_LEFT:
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
    }
}
    
/******************************************
 
 FILE
 
 ********************************************/
    
void Input::dragEvent(ofDragInfo dragInfo){
	source = MEDIA;
    parseFileType(dragInfo.files[0]);    
    setup();
}

void Input::openFileDialog(){
	source = MEDIA;
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a media file");
	if (openFileResult.bSuccess){
		parseFileType(openFileResult.filePath);    
		setup();
			
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
        if (v == "true") loop = true;
        else             loop = false;
    }
    
    setup();
}

void Input::saveXML(ofXml &xml) {
    xml.setTo("input");
    xml.setAttribute("resolution", ofToString(resolution) );

    string str;

	if (source == GRID)			  str = "grid";
    else if (source == MEDIA)     str = "media";
    else if (source == CAPTURE)   str = "capture";
    else if (source == SYPHON)    str = "syphon";
    else if (source == BLACK)    str = "black";
    else if (source == WHITE)    str = "white";
    else if (source == GREY)    str = "grey";
    
    xml.setAttribute("source", str );
    xml.setAttribute("file", file );
    if (loop) xml.setAttribute("loop", "true");
    else      xml.setAttribute("loop", "false");
    xml.setToParent();
}

/******************************************
 
 FILE METADATA
 
 ********************************************/
    
void Input::parseFileType(string filepath){
    oFile.open(filepath);
    pFile = oFile.getPocoFile();
	file = oFile.getAbsolutePath();
    filename = oFile.getFileName();
    mediaType = mediaTypeMap->getMediaTypeForPath(pFile.path()).toString();
    string sub = ofSplitString(mediaType, "/")[0];
    
    if (sub == "video")
        isVideo = true;
    else
        isVideo = false;
    
    oFile.close();
}
    
#ifdef TARGET_OSX
void Input::parseVideoCodec(string filepath){
    AVMediaInfo info = AVProbe::probe(filepath);
    string codecCode = "";
    
    cout << "filename:" << " " << info.path.getFileName() << endl;
    cout << "Metadata:" << endl;
    
    Poco::Net::NameValueCollection::ConstIterator iter = info.metadata.begin();
    while(iter != info.metadata.end())
    {
        cout << iter->first << "=" << iter->second << endl;
        ++iter;
    }
    
    for(size_t i = 0; i < info.streams.size(); i++)
    {
        cout << endl;
        cout << "stream (" << ofToString(i+1) << "/" << info.streams.size() << ")" << endl;
        cout << "=========================================================================" << endl;
        
        AVStreamInfo stream = info.streams[i];
        
        if(stream.codecType == AVMEDIA_TYPE_VIDEO)
        {
            cout << setw(20) << "type:" << " " << "VIDEO STREAM" << endl;
            cout << setw(20) << "width:" << " " << stream.videoWidth << endl;
            cout << setw(20) << "height:" << " " << stream.videoHeight << endl;
            cout << setw(20) << "decoded format:" << " " << stream.videoDecodedFormat << endl;
            
        }
        else if(stream.codecType == AVMEDIA_TYPE_AUDIO)
        {
            cout << setw(20) << "type:" << " " << "AUDIO STREAM" << endl;
            cout << setw(20) << "num channels:" << " " << stream.audioNumChannels << endl;
            cout << setw(20) << "bits / sample:" << " " << stream.audioBitsPerSample << endl;
            cout << setw(20) << "sample rate:" << " " << stream.audioSampleRate << endl;
            
        }
        else
        {
        }
        
        cout << setw(20) << "codec:" << " " << stream.codecName << " [" << stream.codecLongName << "]" << endl;
        
        codecCode = ofToString(stream.codecTag);
        
        cout << setw(20) << "codec tag:" << " " << codecCode << endl;
        cout << setw(20) << "stream codec tag:" << " " << stream.streamCodecTag << endl;
        cout << setw(20) << "profile:" << " " << stream.codecProfile << endl;
        cout << endl;
        
        cout << setw(20) << "avg. bitrate:" << " " << stream.averageBitRate << endl;
        cout << setw(20) << "avg. framerate:" << " " << (double)stream.averageFrameRate.num / stream.averageFrameRate.den << endl;
        
        cout << setw(20) << "Metadata:" << endl;
        
        Poco::Net::NameValueCollection::ConstIterator iter = stream.metadata.begin();
        while(iter != stream.metadata.end())
        {
            cout << setw(30) << iter->first << "=" << iter->second << endl;
            ++iter;
        }
        
        if (codecCode == "avc1")
            vRenderer = AVF;
        else if (codecCode == "Hap1")
            vRenderer = HAP;
        else
            vRenderer = QT; //X
    }
    
}
 #endif
}
