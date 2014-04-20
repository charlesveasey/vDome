#include "ofMain.h"
#include "ofxNetwork.h"
namespace vd {

class Tcp {

public:

    Tcp();
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

}
