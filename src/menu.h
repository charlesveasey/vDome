#pragma once

#include "ofMain.h"
#include "projector.h"
#include "dome.h"
#include "input.h"

#ifdef TARGET_OSX
#include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
#endif
#ifdef TARGET_WIN32
#include <windows.h>
#endif


class Menu {
public:
    Menu();

    // state
    int frameCnt;
    bool saved;    
    bool active;
    int pActive;
    
    struct MenuItem {
        int currentItem;
        vector<string> items;
        int menuId;
        MenuItem **parent;
    };
    
    // menus
    MenuItem *menuMain;
    MenuItem *menuInput;
    MenuItem *menuWarp;
    MenuItem *menuBlend;
    MenuItem *menuBrush;
    MenuItem *menuColor;
    MenuItem *menuSetup;
    MenuItem *menuRadius;
    MenuItem *menuPosition;
    MenuItem *menuOrientation;
    MenuItem *menuLens;
    MenuItem *menuScale;
    MenuItem *menuShear;
    MenuItem **currentMenu;

    enum menus {MAIN, INPUT, WARP, BLEND, COLOR, SETUP,
         RADIUS, POSITION, ORIENTATION, LENS, SCALE, SHEAR, BRUSH};

    // menu items
    enum inputItems {SOURCE, FORMAT, INPUT_SCALE};
    enum warpItems  {CORNERPIN, GRID};
    enum blendItems {BRIGHTNESS, CONTRAST, B_BRUSH};
    enum brushItems {BRUSH_SCALE, BRUSH_OPACITY};
    enum colorItems {SATURATION};
    enum radiusItems{DOME_RADIUS};
    enum posItems   {AZIMUTH, ELEVATION, DISTANCE};
    enum orienItems {TILT, ROLL, PAN};
    enum lensItems  {FOV};
    enum scaleItems {SCALE_XY, SCALE_X, SCALE_Y};
    enum shearItems {SHEAR_YZ, SHEAR_ZX, SHEAR_XZ,
                     SHEAR_ZY, SHEAR_YX, SHEAR_XY};
    
    void findEdit(int mode);
    
    // layout
    int px;
    int py;
    int pw;
    int ph;
    int padx;
    int pady;

    // keyboard
    bool shift;
    bool alt;
    bool ctrl;
    bool all;
    
    // value
    float value;
    float orgValue;
    float shiftValue;
    float altValue;
    
    // pointers
    vector<Projector> *projectors;
    Dome *dome;
    Input *input;
        
    // draw methods
 	void draw();
    void drawMain(int i);
    void drawInput();
    
    void drawProjector(int i);
    void drawFPS(int i);
    void drawHighlight();
    void drawBackground();
    void drawSaved();
    void drawActive(int i);
    void drawWarp(int i);
    
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
    void toggle();    
    float roundTo(float val, float n);
};
