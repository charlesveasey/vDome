#include "input.h"
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Input::Input(){
    maxSource = 2;
    resolution = 2048;
    frameRate = 60;
	usePbo = true;
    isVideo = false;
    mediaTypeMap = new MediaTypeMap("media/mime.types");
    file = "";

    #ifdef TARGET_OSX
        maxSource = 3;
        vRenderer = AVF;
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
    
    texture.clear();

    if (texture.getWidth() != resolution || texture.getHeight() != resolution) {
        texture.allocate(resolution, resolution, GL_RGB);
		if (usePbo)
			pbo.allocate(texture,2);
	}

    stop();
    close();

    // create input
    switch(source){

        case GRID: 
			isVideo = false;
			image.loadImage("media/warp/grid-2k.png");
            texture = image.getTextureReference();
            break;

        case MEDIA: 
            if (isVideo) {
                
                #ifdef TARGET_OSX
					parseVideoCodec(file);
                
					if (vRenderer == AVF){
						avf.loadMovie(file);
						avf.setLoopState(OF_LOOP_NORMAL);
					}
					else if (vRenderer == QT){
						qt.loadMovie(file, OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
						qt.play();
					}
					else if (vRenderer == HAP){
						hap.loadMovie(file, OF_QTKIT_DECODE_TEXTURE_ONLY);
						hap.play();
					}
					else if (vRenderer == X){
						video.loadMovie(file);
						video.play();
						texture = video.getTextureReference();
					}
				#endif

				#ifdef TARGET_WIN32
					if (vRenderer == WMF) {
						wmf.loadMovie(file);
						wmf.play();
						wmf.setLoop(true);
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
            }
            else {
                image.loadImage(file);
                texture = image.getTextureReference();
            }
            break;
            
        case CAPTURE: 
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(frameRate);
            capture.initGrabber(resolution, resolution);
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

 BIND

 ********************************************/

void Input::bind(){
    
	if (usePbo && updateTexture) {
		pbo.updateTexture();
		updateTexture = false;
	}


	if (source == GRID) {
		texture.bind();
	}
	else if (source == MEDIA) {
        #ifdef TARGET_OSX
            if (isVideo) {
                if (vRenderer == AVF)
                    avf.getTextureReference().bind();
                else if (vRenderer == QT) {
                    if (qt.getTexture() != NULL)
                        qt.getTexture()->bind();
                }
                else if (vRenderer == HAP)
                    hap.getTexture()->bind();
                else if (vRenderer == X)
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
 
	if (source == GRID) {
		texture.unbind();
	}

	else if (source == MEDIA) {
        #ifdef TARGET_OSX
            if (isVideo) {
                if (vRenderer == AVF)
                    avf.getTextureReference().unbind();
                else if (vRenderer == QT) {
                    if (qt.getTexture() != NULL)
                        qt.getTexture()->unbind();
                }
                else if (vRenderer == HAP)
                    hap.getTexture()->unbind();
                else if (vRenderer == X)
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
                else if (vRenderer == QT)
                    qt.update();
                else if (vRenderer == HAP)
                    hap.update();
                else if (vRenderer == X)
                    video.update();
			#endif

            #ifdef TARGET_LINUX
                video.update();
            #endif

			#ifdef TARGET_WIN32
				if (vRenderer == WMF)
					wmf.update();
				else if (vRenderer == DS || vRenderer == GST)
					video.update();	
				}
			#endif

		if (usePbo && video.isFrameNew() ) {
				pbo.loadData(video.getPixelsRef());
				updateTexture = true;
        }
    }
    
	else if (source == CAPTURE)
        capture.update();
}

/******************************************

 KEYBOARD

 ********************************************/

void Input::keyPressed(int key) {
    switch (key) {
        case OF_KEY_RIGHT:
			if (source+1 > maxSource)
				source = maxSource;
			else
				source++;
			if (source == MEDIA)
				parseFileType(file);
			setup();
            break;
        case OF_KEY_LEFT:
			if (source-1 < 0)
				source = 0;
			else
				source--;
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

 SETTINGS

 ********************************************/

void Input::loadXML(ofXml &xml) {

    if (xml.exists("input[@resolution]"))
        resolution = ofToInt( xml.getAttribute("input[@resolution]") );

    if (xml.exists("input[@source]")) {
        string m = xml.getAttribute("input[@source]");
		if (m == "grid")		   source = GRID;
        else if (m == "media")     source = MEDIA;
        else if (m == "capture")   source = CAPTURE;
        else if (m == "syphon")    source = SYPHON;
    }

    if (xml.exists("input[@file]")) {
        parseFileType(ofToString( xml.getAttribute("input[@file]") ));
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

    xml.setAttribute("source", str );
    xml.setAttribute("file", file );
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
