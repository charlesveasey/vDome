#include "input.h"
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Input::Input(){
	#ifdef TARGET_WIN32
		ofPtr <ofBaseVideoPlayer> ptr(new ofDirectShowPlayer());
		video.setPlayer(ptr);
	#endif

    maxSource = 2;
    resolution = 2048;
    frameRate = 60;

    #ifdef TARGET_OSX
        maxSource = 4;
        file = "test.mov";
        vRenderer = AVF;
    #else
        file = "test.avi";
    #endif
    
    isVideo = false;
    mediaTypeMap = MediaTypeMap::getDefault();
}

/******************************************

 SETUP

 ********************************************/

void Input::setup(){
    
    texture.clear();

    if (texture.getWidth() != resolution || texture.getHeight() != resolution)
        texture.allocate(resolution, resolution, OF_IMAGE_COLOR);

    stop();

    // create input
    switch(source){
        case 1: // capture
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(frameRate);
            capture.initGrabber(resolution, resolution);
            texture = capture.getTextureReference();
            break;
        case 2: // media
            
            if (isVideo) {
                
                #ifdef TARGET_OSX
                parseVideoCodec("media/"+file);
                if (vRenderer == AVF){
                    avf.loadMovie("media/"+file);
                    avf.setLoopState(OF_LOOP_NORMAL);
                }
                else if (vRenderer == QT){
                    qt.loadMovie("media/"+file, OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
                    qt.play();
                }
                else if (vRenderer == HAP){
                    hap.loadMovie("media/"+file, OF_QTKIT_DECODE_TEXTURE_ONLY);
                    hap.play();
                }
                #else
                video.setPixelFormat(OF_PIXELS_RGB);
                video.loadMovie("media/"+file);
                video.play();
                texture = video.getTextureReference();
                #endif
            }
            else {
                image.loadImage("media/grid.jpg");
                texture = image.getTextureReference();
            }

            break;
        case 3: // hap video
			#ifdef TARGET_OSX

			#endif
            break;
        case 4: // syphon
			#ifdef TARGET_OSX
				syphon.setup();
			#endif
            break;
        default:
            image.loadImage("media/grid.jpg");
            texture = image.getTextureReference();
            break;
    }

}

/******************************************

 STOP

 ********************************************/

void Input::stop() {
    video.stop();
	#ifdef TARGET_OSX
		hap.stop();
	#endif
}

/******************************************

 CLOSE

 ********************************************/

void Input::close() {
    image.clear();
    video.stop();
    video.close();
    //capture.close();
    #ifdef TARGET_OSX
		hap.stop();
		hap.close();
    #endif
}

/******************************************

 BIND

 ********************************************/

void Input::bind(){
    
	if (source != 3 && source != 4) {
        #ifdef TARGET_OSX
            if (vRenderer == AVF)
                avf.getTextureReference().bind();
            else if (vRenderer == QT) {
                if (qt.getTexture() != NULL)
                    qt.getTexture()->bind();
            }
            else if (vRenderer == HAP)
                hap.getTexture()->bind();
        #else
           texture.bind();
        #endif
	}
    
    #ifdef TARGET_OSX
        if (source == 4)
            syphon.bind();
    #endif
}

void Input::unbind(){
 
	if (source != 3 && source != 4) {
        #ifdef TARGET_OSX
            if (vRenderer == AVF)
                avf.getTextureReference().unbind();
            else if (vRenderer == QT) {
                if (qt.getTexture() != NULL)
                    qt.getTexture()->unbind();
            }
            else if (vRenderer == HAP)
                hap.getTexture()->unbind();
        #else
            texture.unbind();
        #endif
	}
    
	#ifdef TARGET_OSX
		if (source == 4)
			syphon.unbind();
    #endif

}

/******************************************

 UPDATE

 ********************************************/

void Input::update(){
    
    if (source == 2) {
        
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
        
        #else
            video.update();
        #endif
    }
    
	else if (source == 1)
        capture.update();
	
    #ifdef TARGET_OSX
		if (source == 3)
		    hap.update();
    #endif
}

/******************************************

 KEYBOARD

 ********************************************/

void Input::keyPressed(int key) {
    switch (key) {
        case OF_KEY_RIGHT:  // up = switch on mode
			if (source+1 > maxSource)
				source = maxSource;
			else
				source++;
			setup();
            break;
        case OF_KEY_LEFT:  // up = switch on mode
			if (source-1 < 0)
				source = 0;
			else
				source--;
            setup();
			break;
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
        if (m == "image")           source = 0;
        else if (m == "video")      source = 2;
        else if (m == "capture")    source = 1;
        else if (m == "hap")        source = 3;
        else if (m == "syphon")     source = 4;
    }

    if (xml.exists("input[@file]")) {
        file = ofToString( xml.getAttribute("input[@file]") );
        oFile.open(file);
        pFile = oFile.getPocoFile();
        mediaType = mediaTypeMap->getMediaTypeForPath(pFile.path()).toString();
        string sub = ofSplitString(mediaType, "/")[0];
        
        if (sub == "video")
            isVideo = true;
        else
            isVideo = false;
    }
    
    setup();
}

void Input::saveXML(ofXml &xml) {
    xml.setTo("input");
    xml.setAttribute("resolution", ofToString(resolution) );

    string str;

    if (source == 0)        str = "image";
    else if (source == 2)   str = "video";
    else if (source == 1)   str = "capture";
    else if (source == 3)   str = "hap";
    else if (source == 4)   str = "capture";

    xml.setAttribute("source", str );
    xml.setAttribute("file", file );
    xml.setToParent();
}

#ifdef TARGET_OSX
void Input::parseVideoCodec(string filepath){
    AVMediaInfo info = AVProbe::probe(filepath);
    string codecCode;
    
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
            vRenderer = QT;
    }
    
}
 #endif
}
