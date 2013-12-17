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
    void onMouseDragged(int x, int y, int button);
    void onMousePressed(int x, int y, int button);
    void onMouseReleased(int x, int y, int button);
    
    //void loadXML(ofXml &xml);
    //void saveXML(ofXml &xml);

};

