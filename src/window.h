#include "ofMain.h"

class Window {
	
public:
    
    void init();
    void setup();
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    int x;
    int y;
    int width;
    int height;
	bool fullscreen;
    
};

