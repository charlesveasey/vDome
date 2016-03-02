#pragma once
#include "window.h"
#include "socket.h"
#include "saveThread.h"
#include "systemUtil.h"
namespace vd {

class vdome {

public:
    vdome();
    void    init();
    void    setup(int &n);
    void    update(int &n);

private:
    void    loadXML();
    void    saveXML();
    void    createWindow(ofXml &xml);
    void    setupInput();
    void    updateInputFormat();
    void    updateInputTransform();
    void    exit();
    
    void    onColorEvent(ofVec3f &color);
    void    onSourceEvent(int &s);
    void    onFormatEvent(int &s);
    void    onSourceColorEvent(int &s);
    
    void    keyPressed(int &key);
    void    keyReleased(int &key);

#ifdef TARGET_OSX
	void    updateSyphonInputTransform();
#endif

    Input       input;

    ofXml       xml;
    string      xmlPath;
    SaveThread  saveThread;
    
    bool        vsync;
    int         framerate;
    int         cKey;
    int         mouseMovePending;

    vector<shared_ptr<Window>>              windows;
    vector<shared_ptr<ofAppBaseWindow>>     baseWindows;
    

	vector<Projector*>              projectors;

    void        socketUpdate();
    

};

}/////////