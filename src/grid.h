#include "ofMain.h"

class Grid {
	
public:
    
    void init();
    void setup();
    void update();
    void draw();
    void drawConfig();
    
    int indx;
    ofVboMesh mesh;
    ofPlanePrimitive plane;
    
    void keyPressed(int key);
    void keyReleased(int key);
    void onMouseDragged(ofMouseEventArgs& mouseArgs);
    void onMousePressed(ofMouseEventArgs& mouseArgs);
    void onMouseReleased(ofMouseEventArgs& mouseArgs);
    //void loadXML(ofXml &xml);
    //void saveXML(ofXml &xml);

};

