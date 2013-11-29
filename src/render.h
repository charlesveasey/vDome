#include "ofMain.h"

class Render {
	
public:
    
    void init();
    void setup();
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);

    int frameRate;
    int domeMaster;
    bool vSync;

};