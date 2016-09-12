#pragma once
#include "ofMain.h"

class SaveThread : public ofThread{

public:
    SaveThread(){
        saved = false;
    }

    void save(){
        saved = false;
        startThread();
    }

    void threadedFunction(){
        while( isThreadRunning() != 0 ){
            if( lock() ){
                for (int i=0; i <xml.size(); i++) {
                    xml[i]->save(files[i]);
                }
                saved = true;
                unlock();
                stopThread();
            }
        }
    }
    
    bool saved;
    vector<ofXml*>      xml;
    vector<string>      files;
};
