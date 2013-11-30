#include "ofMain.h"
#include "ofxNetwork.h"

class Tcp {
	
public:
    
    void init();
    void setup();
    void draw();
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    ofxTCPServer server;
    vector <string> storeText;
    
    int port;
    bool enabled;
    
    int x;
    int y;
};


