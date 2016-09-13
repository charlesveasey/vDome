#pragma once
#include "ofMain.h"
#include "projector.h"
#include "systemUtil.h"

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
    
    void update();

    // Draw
 	void draw(int i, int labelIndex);
    void drawMain(int i);
    void drawProjector(int i);
    void drawFPS(int i);
    void drawHighlight();
    void drawBackground();
    void drawSaved();
    void drawActive(int i);
    void drawCurves(int i);

    // Navigation
    void select();
    void back();
    void setEditMode();
	void toggle();

    // Mouse
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
	void mouseMoved(ofMouseEventArgs& mouseArgs);
	void mouseScrolled(ofMouseEventArgs& mouseArgs);

    // Keyboard
    void keyPressed(int key);
    void keyReleased(int key);
    
    // Utils
	float roundTo(float val, float n);
	float round(float d);

	// Menu item
	struct MenuItem {
        int currentItem;
        vector<Item*> items;
        int menuId;
        MenuItem **parent;
    };
    
    // Menu data structures
    MenuItem *menuMain;
	MenuItem *menuView;
    MenuItem *menuWarp;
    MenuItem *menuBlend;
    MenuItem *menuColor;
	MenuItem *menuCurves;
	MenuItem *menuCurvesGrey;
	MenuItem *menuCurvesRed;
	MenuItem *menuCurvesGreen;
	MenuItem *menuCurvesBlue;
    MenuItem **currentMenu;

	// Menu types
    enum menus		{ MAIN, WARP, BLEND, COLOR, VIEW,
					  CURVES, CURVES_GREY, CURVES_RED, CURVES_GREEN, CURVES_BLUE};
    
	// Menu item types
	enum mainItems	{ ENABLE };
    enum warpItems  { GRID };
    enum colorItems { SATURATION, COLOR_CURVES };
	enum blendItems { BRIGHTNESS, CONTRAST };
	enum viewItems  { FOV, SHEARX, SHEARY };

	// Layout
    int px;
    int py;
    int pw;
    int ph;
    int padx;
    int pady;
    
    // Stores whether all all keys are selected
    bool all;
    // Control key selected
    int  cKey;

    // Value  
    float value;
    float orgValue;
    float ctrlValue;
    float altValue;

    // Projector list
    vector<Projector>  *projectors;
    
    // Projector count
    int					projCount;
    
    // First projector index within the window
    int					projectorStartingIndex;
    
	// Input sources
    int		inputSource;
    enum	inputSources {SOURCE_MEDIA, SOURCE_CAPTURE, SOURCE_SYPHON, SOURCE_SPOUT, SOURCE_GRID, SOURCE_BLACK, SOURCE_WHITE, SOURCE_GREY, SOURCE_COLOR};
    
    // Menu States:    
    // Frame count for saved state
    int		frameCnt;
    // Saved state
	bool	saved;
    // Active state
	bool	active;
    // Projector active state
    int		pActive;
    // Stored source, used for input overrides
	int		storedSource;
    // Current color, used for color overrides
	ofVec3f currentColor;
	// Color events
	static ofEvent<int>		sourceColorEvent;
	static ofEvent<ofVec3f> colorEvent;

private:
    ofVec3f updateColorFromCurve(int pointIndex, bool forceGrey);
    void	changeColorCurveMode(int i);
    int		curvePointIndex;
    bool	newCurvePointIndex;
};

}
