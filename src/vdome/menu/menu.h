#pragma once
#include "ofMain.h"
#include "projector.h"

namespace vd {

class Item {
public:
    string str;
    bool isParent;
    Item(string s){
        str = s;
        isParent = false;
    }
    Item(string s, bool p){
        str = s;
        isParent = p;
    }
};
 
    
class Menu {
public:
    Menu();
    
	void setup();
    void update();

    // draw methods
 	void draw(int i, int labelIndex);
    void drawMain(int i);
    
    void drawProjector(int i);
    void drawFPS(int i);
    void drawHighlight();
    void drawBackground();
    void drawSaved();
    void drawActive(int i);
    void drawWarp(int i);
    void drawCurves(int i);

    // navigation
    void select();
    void back();
    void setEditMode();
    
    // mouse methods
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    
    // keyboard methods
    void keyPressed(int key);
    void keyReleased(int key);
    
    // utils
    void  toggle();
	float roundTo(float val, float n);
	float round(float d);

	// menu item
	struct MenuItem {
        int currentItem;
        vector<Item*> items;
        int menuId;
        MenuItem **parent;
    };
    
    // menu objects
    MenuItem *menuMain;
    MenuItem *menuInput;
    MenuItem *menuInputVideo;
    MenuItem *menuInputTransform;
    MenuItem *menuWarp;
    MenuItem *menuBlend;
    MenuItem *menuBrush;
    MenuItem *menuColor;
	MenuItem *menuCurves;
	MenuItem *menuCurvesGrey;
	MenuItem *menuCurvesRed;
	MenuItem *menuCurvesGreen;
	MenuItem *menuCurvesBlue;
    MenuItem *menuSetup;
    MenuItem *menuPosition;
    MenuItem *menuOrientation;
    MenuItem *menuFov;
    MenuItem *menuLens;
    MenuItem *menuScale;
    MenuItem *menuShear;
    MenuItem **currentMenu;

	// menu types
    enum menus		{MAIN, WARP, BLEND, COLOR,
					 CURVES, CURVES_GREY, CURVES_RED, CURVES_GREEN, CURVES_BLUE, 
					 SETUP, POSITION, ORIENTATION, FIELD_OF_VIEW, LENS, BRUSH};
    
	// menu item types
	enum mainItems	{ENABLE};
    enum warpItems  {CORNERPIN, GRID};
    enum blendItems {B_BRUSH};
    enum brushItems {BRUSH_SCALE, BRUSH_OPACITY};
    enum colorItems {BRIGHTNESS, CONTRAST, SATURATION, COLOR_CURVES};
    enum posItems   {AZIMUTH, ELEVATION, DISTANCE};
    enum orienItems {TILT, ROLL, PAN};
    enum fovItems   {FOV};
    enum lensItems  {OFFSET_X, OFFSET_Y};

    // layout
    int px;
    int py;
    int pw;
    int ph;
    int padx;
    int pady;

    // keyboard
    bool all;
    int  cKey;

    // value
    float value;
    float orgValue;
    float ctrlValue;
    float altValue;

    // projector
    vector<Projector>  *projectors;				// list of projectors
    int					projCount;				// projector count
    int					projectorStartingIndex;	// first projector index within the window
    
	// input sources
    int		inputSource;
    enum	inputSources {SOURCE_MEDIA, SOURCE_CAPTURE, SOURCE_SYPHON, SOURCE_SPOUT, SOURCE_GRID, SOURCE_BLACK, SOURCE_WHITE, SOURCE_GREY, SOURCE_COLOR};
    
	// menu states
	int		frameCnt;		// frame cout for saved items
	bool	saved;			// saved state
	bool	active;			// active state
	int		pActive;		// projector active state
	int		storedSource;	// stored source, used for input overrides
	ofVec3f currentColor;	// current color, used for color overrides

	static ofEvent<int>		sourceColorEvent;	// event for 
	static ofEvent<ofVec3f> colorEvent;

private:
    ofVec3f updateColorFromCurve(int pointIndex, bool forceGrey);
    void	changeColorCurveMode(int i);
    int		curvePointIndex;
    bool	newCurvePointIndex;
};

}
