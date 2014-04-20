#include "menu.h"
namespace vd {

extern float projCount;
extern float projWidth;
extern float projHeight;

/******************************************

 CONSTRUCTOR

 ********************************************/

Menu::Menu(){

    menuMain = new MenuItem;
    menuMain->menuId = MAIN;
    menuMain->parent = &menuMain;
    menuMain->currentItem = 0;
    menuMain->items.push_back("Input              ->");
    menuMain->items.push_back("Warp               ->");
    menuMain->items.push_back("Blend              ->");
    menuMain->items.push_back("Color              ->");
    menuMain->items.push_back("Setup              ->");

    menuInput = new MenuItem;
    menuInput->menuId = INPUT;
    menuInput->parent = &menuMain;
    menuInput->currentItem = 0;
    menuInput->items.push_back("Source");
    menuInput->items.push_back("Format");
    menuInput->items.push_back("Scale");
    //menuInput->items.push_back("Offset X");
    //menuInput->items.push_back("Offset Y");

    menuWarp = new MenuItem;
    menuWarp->menuId = WARP;
    menuWarp->parent = &menuMain;
    menuWarp->currentItem = 0;
    menuWarp->items.push_back("Cornerpin");
    menuWarp->items.push_back("Grid");

    menuBlend = new MenuItem;
    menuBlend->menuId = BLEND;
    menuBlend->parent = &menuMain;
    menuBlend->currentItem = 0;
    menuBlend->items.push_back("Brightness");
    menuBlend->items.push_back("Contrast");
    menuBlend->items.push_back("Brush              ->");

    menuBrush = new MenuItem;
    menuBrush->menuId = BRUSH;
    menuBrush->parent = &menuBlend;
    menuBrush->currentItem = 0;
    menuBrush->items.push_back("Brush Scale");
    menuBrush->items.push_back("Brush Opacity");

    menuColor = new MenuItem;
    menuColor->menuId = COLOR;
    menuColor->parent = &menuMain;
    menuColor->currentItem = 0;
    menuColor->items.push_back("HSL                ->");
    menuColor->items.push_back("Gamma              ->");

    menuHSL = new MenuItem;
    menuHSL->menuId = HSL;
    menuHSL->parent = &menuColor;
    menuHSL->currentItem = 0;
    menuHSL->items.push_back("Hue");
    menuHSL->items.push_back("Saturation");
    menuHSL->items.push_back("Lightness");

    menuGamma = new MenuItem;
    menuGamma->menuId = GAMMA;
    menuGamma->parent = &menuColor;
    menuGamma->currentItem = 0;
    menuGamma->items.push_back("Gamma");
    menuGamma->items.push_back("Gamma Red");
    menuGamma->items.push_back("Gamma Green");
    menuGamma->items.push_back("Gamma Blue");

    menuSetup = new MenuItem;
    menuSetup->menuId = SETUP;
    menuSetup->parent = &menuMain;
    menuSetup->currentItem = 0;
    menuSetup->items.push_back("Radius             ->");
    menuSetup->items.push_back("Position           ->");
    menuSetup->items.push_back("Orientation        ->");
    menuSetup->items.push_back("Lens               ->");
    menuSetup->items.push_back("Scale              ->");
    menuSetup->items.push_back("Shear              ->");

    menuRadius = new MenuItem;
    menuRadius->menuId = RADIUS;
    menuRadius->parent = &menuSetup;
    menuRadius->currentItem = 0;
    menuRadius->items.push_back("Dome Radius");

    menuPosition = new MenuItem;
    menuPosition->menuId = POSITION;
    menuPosition->parent = &menuSetup;
    menuPosition->currentItem = 0;
    menuPosition->items.push_back("Azimuth");
    menuPosition->items.push_back("Elevation");
    menuPosition->items.push_back("Distance");

    menuOrientation = new MenuItem;
    menuOrientation->menuId = ORIENTATION;
    menuOrientation->parent = &menuSetup;
    menuOrientation->currentItem = 0;
    menuOrientation->items.push_back("Tilt");
    menuOrientation->items.push_back("Roll");
    menuOrientation->items.push_back("Pan");

    menuLens = new MenuItem;
    menuLens->menuId = LENS;
    menuLens->parent = &menuSetup;
    menuLens->currentItem = 0;
    menuLens->items.push_back("Field of View");

    menuScale = new MenuItem;
    menuScale->menuId = SCALE;
    menuScale->parent = &menuSetup;
    menuScale->currentItem = 0;
    menuScale->items.push_back("Scale");
    menuScale->items.push_back("Scale X");
    menuScale->items.push_back("Scale Y");

    menuShear = new MenuItem;
    menuShear->menuId = SHEAR;
    menuShear->parent = &menuSetup;
    menuShear->currentItem = 0;
    menuShear->items.push_back("Shear YZ");
    menuShear->items.push_back("Shear ZX");
    menuShear->items.push_back("Scale XZ");
    menuShear->items.push_back("Shear ZY");
    menuShear->items.push_back("Shear YX");
    menuShear->items.push_back("Scale XY");

    currentMenu = &menuMain;

    frameCnt= 0;
    saved = false;
    active = false;

    // add values
    value = 1;
    orgValue = 1;
    shiftValue = .1;
    altValue = .01;

    // config draw
	shift = false;
	alt = false;
	ctrl = false;

    // layout
    pw = 200;
    ph = 135;
    padx = 15;
    pady = 15;

}

/******************************************

 Draw

 ********************************************/

void Menu::drawHighlight(){
    ofSetHexColor(0xF9ED6B);
}

void Menu::drawBackground(){
    // background color
    if (saved) {
        frameCnt++;
        if (frameCnt == 60) {
            saved = false;
            frameCnt = 0;
        }
        ofSetHexColor(0xAA0000);
    }
    else {
        ofSetHexColor(0x000000);
    }
    // background shape
    ofFill();
    ofRect(px, py, 1, pw, ph);
}

void Menu::drawMain(int i){
    string str;
    string val;

    for(int j=0; j<(*currentMenu)->items.size(); j++) {
        str = (*currentMenu)->items[j];

        if (j == (*currentMenu)->currentItem)
            drawHighlight();

        if ((*currentMenu)->menuId != MAIN &&
            (*currentMenu)->menuId != WARP &&
            (*currentMenu)->menuId != SETUP) {

            switch ((*currentMenu)->menuId) {

                case INPUT:
                    switch (j) {
                        case SOURCE:
                            if (input->source == 1)		 val = "Capture";
                            else if (input->source == 2)   val = "Video";
                            else if (input->source == 3)   val = "Hap";
                            else if (input->source == 4)   val = "Syphon";
                            else						 val = "Image";
                            break;
                        case FORMAT:
                            val = "Domemaster";
                            break;
                        case INPUT_SCALE:
                            val = ofToString(roundTo(dome->textureScale, .01));
                            break;
                    }
                    break;

                case BLEND:
                    switch (j) {
                        case BRIGHTNESS:
                            val = ofToString(roundTo(projectors->at(i).brightness, .001));
                            break;
                        case CONTRAST:
                            val = ofToString(roundTo(projectors->at(i).contrast, .001));
                            break;
                    }
                    break;

                case BRUSH:
                    switch (j) {
                        case BRUSH_SCALE:
                            val = ofToString(roundTo(projectors->at(i).mask.brushScale, .001));
                            break;
                        case BRUSH_OPACITY:
                            val = ofToString(roundTo(projectors->at(i).mask.brushOpacity, .001));
                            break;
                    }
                    break;

                case HSL:
                    switch (j) {
                        case HUE:
                            val = ofToString(roundTo(projectors->at(i).hue, .001));
                            break;
                        case SATURATION:
                            val = ofToString(roundTo(projectors->at(i).saturation, .001));
                            break;
                        case LIGHTNESS:
                            val = ofToString(roundTo(projectors->at(i).lightness, .001));
                            break;
                    }
                    break;

                case GAMMA:
                    switch (j) {
                        case GAMMA_RGB:
                            val = ofToString(roundTo(projectors->at(i).gamma, .001));
                            break;
                        case GAMMA_R:
                            val = ofToString(roundTo(projectors->at(i).gammaR, .001));
                            break;
                        case GAMMA_G:
                            val = ofToString(roundTo(projectors->at(i).gammaG, .001));
                            break;
                        case GAMMA_B:
                            val = ofToString(roundTo(projectors->at(i).gammaB, .001));
                            break;
                    }
                    break;

                case RADIUS:
                    switch (j) {
                        case 0:
                            val = ofToString(roundTo(dome->radius, .01));
                            break;
                    }

                    break;

                case POSITION:
                    switch (j) {
                        case AZIMUTH:
                            val = ofToString(roundTo(projectors->at(i).getCameraPosition().x, .01));
                            break;
                        case ELEVATION:
                            val = ofToString(roundTo(projectors->at(i).getCameraPosition().y, .01));
                            break;
                        case DISTANCE:
                            val = ofToString(roundTo(projectors->at(i).getCameraPosition().z, .01));
                            break;
                    }

                    break;

                case ORIENTATION:
                    switch (j) {
                        case TILT:
                            val = ofToString(roundTo(projectors->at(i).getCameraOrientation().y, .01));
                            break;
                        case ROLL:
                            val = ofToString(roundTo(projectors->at(i).getCameraOrientation().x, .01));
                            break;
                        case PAN:
                            val = ofToString(roundTo(projectors->at(i).getCameraOrientation().z, .01));
                            break;
                    }

                    break;

                case LENS:
                    switch (j) {
                        case FOV:
                            val = ofToString(roundTo(projectors->at(i).getCameraFov(), .01));
                            break;
                    }

                    break;

                case SCALE:
                    switch (j) {
                        case SCALE_XY:
                            val = "";
                            break;
                        case SCALE_X:
                            val = ofToString(roundTo(projectors->at(i).getCameraScale().x, .01));
                            break;
                        case SCALE_Y:
                            val = ofToString(roundTo(projectors->at(i).getCameraScale().y, .01));
                            break;
                    }

                    break;


                case SHEAR:
                    switch (j) {
                        case SHEAR_YZ:
                            val = ofToString(roundTo(projectors->at(i).getCameraShear()[3], .001));
                            break;
                        case SHEAR_ZX:
                            val = ofToString(roundTo(projectors->at(i).getCameraShear()[4], .001));
                            break;
                        case SHEAR_XZ:
                            val = ofToString(roundTo(projectors->at(i).getCameraShear()[1], .001));
                            break;
                        case SHEAR_ZY:
                            val = ofToString(roundTo(projectors->at(i).getCameraShear()[5], .001));
                            break;
                        case SHEAR_YX:
                            val = ofToString(roundTo(projectors->at(i).getCameraShear()[2], .001));
                            break;
                        case SHEAR_XY:
                            val = ofToString(roundTo(projectors->at(i).getCameraShear()[0], .001));
                            break;
                    }

                    break;
            }

            // value string
            if (str.find("->") != 19) { // nested menu exception
                while (str.size() + val.size() < 21) {
                    str += " ";
                }
                str += val;
            }
        }

        ofDrawBitmapString(str, px+padx, py+pady * (3.25+j) );

        if (j == (*currentMenu)->currentItem)
            ofSetHexColor(0xFFFFFF);
    }

}


void Menu::draw(){
    if (!active) return;

    for(int i=0; i<projCount; i++) {
        // set position
        px = projectors->at(i).getPlanePosition().x + projWidth/2 - pw/2;
        py = projectors->at(i).getPlanePosition().y + projHeight/2  - ph/2;

        drawBackground();
        ofSetHexColor(0xFFFFFF);
        drawProjector(i);
        drawFPS(i);
        drawMain(i);
        drawWarp(i);
        drawActive(i);
        drawSaved();
    }
}

void Menu::drawSaved(){
    if (saved) {
        ofSetHexColor(0xFFFFFF);
        ofDrawBitmapString("SAVED", px+padx+125, py+pady*1.75);

    }
}

void Menu::drawActive(int i){
    if (!saved) {
        if (projectors->at(i).keyboard || projectors->at(i).mouse) {
            ofSetHexColor(0xFFF000);
            ofDrawBitmapString("Active", px+padx+120, py+pady*1.75);
            ofSetHexColor(0xFFFFFF);
        }
    }
}

void Menu::drawWarp(int i){
    // draw cornerpin or plane
    if (projectors->at(i).editMode == projectors->at(i).CORNERPIN
        || projectors->at(i).editMode == projectors->at(i).GRID) {

        if (projectors->at(i).editMode == projectors->at(i).CORNERPIN) {
            projectors->at(i).drawKeystone();
        }
        else if (projectors->at(i).editMode == projectors->at(i).GRID) {
            projectors->at(i).drawPlaneConfig();
        }
    }
}

void Menu::drawProjector(int i){
    ofDrawBitmapString("Projector #" + ofToString(i+1), px+padx, py+pady * 1.75);
}

void Menu::drawFPS(int i){
    if (!saved) {
        if (!projectors->at(i).active)
            ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), px+padx + 125, py+pady * 1.75);
    }
}

void Menu::toggle() {
    active = !active;
    if (active)
        ofShowCursor();
    else
        ofHideCursor();
}

/******************************************

 NAVIGATION

 ********************************************/

void Menu::select() {

    int item = (*currentMenu)->currentItem;
    switch ((*currentMenu)->menuId) {
        case MAIN:
            switch (item) {
                case 0: currentMenu = &menuInput; break;
                case 1: currentMenu = &menuWarp; break;
                case 2: currentMenu = &menuBlend; break;
                case 3: currentMenu = &menuColor; break;
                case 4: currentMenu = &menuSetup; break;
                default: break;
            }
            break;
        case SETUP:
            switch (item) {
                case 0: currentMenu = &menuRadius; break;
                case 1: currentMenu = &menuPosition; break;
                case 2: currentMenu = &menuOrientation; break;
                case 3: currentMenu = &menuLens; break;
                case 4: currentMenu = &menuScale; break;
                case 5: currentMenu = &menuShear; break;
                default: break;
            }
            break;
        case BLEND:
            switch (item) {
                case 2: currentMenu = &menuBrush; break;
                default: break;
            }
            break;
        case COLOR:
            switch (item) {
                case 0: currentMenu = &menuHSL; break;
                case 1: currentMenu = &menuGamma; break;
                default: break;
            }
            break;
        default: break;
    }
    setEditMode();
}

void Menu::back() {
    currentMenu = (*currentMenu)->parent;
    setEditMode();
}

/******************************************

 MOUSE

 ********************************************/

void Menu::mousePressed(ofMouseEventArgs& mouseArgs) {
    if (active) {
        for (int i=0; i<projCount; i++) {
            projectors->at(i).mousePressed(mouseArgs);
        }
     }
}

void Menu::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (active) {
        for (int i=0; i<projCount; i++) {
            projectors->at(i).mouseDragged(mouseArgs);
        }
    }
}

void Menu::mouseReleased(ofMouseEventArgs& mouseArgs) {
    if (active) {
        for (int i=0; i<projCount; i++) {
            projectors->at(i).mouseReleased(mouseArgs);
        }
    }
}

/******************************************

 KEYBOARD

 ********************************************/

void Menu::keyPressed(int key) {

    switch(key){

        // SHORTCUTS

        // m = show menu
        case 109:
            active = !active;
            if (active)
                ofShowCursor();
            else
                ofHideCursor();
            break;


        // MODIFIERS

        case OF_KEY_ALT:
            value = altValue;
            alt = true;
            break;

        case OF_KEY_CONTROL:
            ctrl = true;
            break;

        case OF_KEY_SHIFT:
            value = shiftValue;
            shift = true;
            break;

        case OF_KEY_SUPER:
            ctrl = true;
            break;

    }


    for (int k=0; k<projCount; k++) {
        if (projectors->at(k).active)
            projectors->at(k).keyPressed(key);
    }


    // MENU
    ///////////////////////////
    if (!active) { return; }
    ///////////////////////////



    // NAVIGATION

    switch (key){
        case OF_KEY_UP:
            if ((*currentMenu)->currentItem > 0)
                (*currentMenu)->currentItem--;
            break;

        case OF_KEY_DOWN:
            if ((*currentMenu)->currentItem < (*currentMenu)->items.size()-1)
                (*currentMenu)->currentItem++;
            break;

        case OF_KEY_RETURN:
            select();
            break;

        case OF_KEY_BACKSPACE:
            back();
            break;
    }


    // SET VALUE
    for (int i=0; i<projCount; i++) {
        projectors->at(i).setValue(value);
    }
    dome->value = value;

    if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {

        switch((*currentMenu)->menuId) {
            case INPUT:
                switch ((*currentMenu)->currentItem) {
                    case SOURCE:
                        input->keyPressed(key);
                        break;
                }
                break;
            case RADIUS: // d = dome mesh
                dome->keyPressed(key);
                break;

        }


    }

    // SET EDIT MODE
    else if (key == OF_KEY_UP || key == OF_KEY_DOWN){
        setEditMode();
    }

    // SELECT PROJECTORS

    // ~ = de/select all projectors
    else if (key == 96) {
        all = projectors->at(0).keyboard;
        for (int i=1; i<projCount; i++) {
            if (all != projectors->at(i).keyboard) {
                all = true;
                break;
            }
            else {
                all = !all;
            }
        }
        for (int i=0; i<projCount; i++) {
            projectors->at(i).keyboard = all;
            projectors->at(i).mouse = all;
            projectors->at(i).active = all;
        }
        setEditMode();
    }


    // 1 - 10 projectors
   if (key >= 48 && key <= 57)  {

        // map key to projector
        if (key == 48) pActive = 10;
        else pActive = key-49;

        if (pActive < projCount) {

           // shift groups, otherwise reset
           if (!shift) {
               for (int i=0; i<projCount; i++) {
                   if (i != pActive) {
                       projectors->at(i).keyboard = false;
                       projectors->at(i).mouse = false;
                       projectors->at(i).active = false;
                   }
               }
           }

           // activate keyboard / mouse input
           projectors->at(pActive).keyboard = !(projectors->at(pActive).keyboard);
           projectors->at(pActive).mouse = !(projectors->at(pActive).mouse);
           projectors->at(pActive).active = !(projectors->at(pActive).active);

           // move mouse to new selection
           int xmouse = projectors->at(pActive).getPlanePosition().x+projectors->at(pActive).getPlaneDimensions().x/2;
           int ymouse = projectors->at(pActive).getPlanePosition().y+projectors->at(pActive).getPlaneDimensions().y/2;

           if (projectors->at(pActive).active) {
                #ifdef TARGET_OSX
                    glutWarpPointer(xmouse, -ymouse);
                #endif
                #ifdef TARGET_WIN32
                    SetCursorPos(xmouse, ymouse);
                #endif
           }
           setEditMode();
       }
    }
}

void Menu::setEditMode() {

    for (int k=0; k<projCount; k++) {
        projectors->at(k).editMode = projectors->at(k).NONE;
        projectors->at(k).setKeystoneActive(false);
        projectors->at(k).setGridActive(false);
    }

    int j = (*currentMenu)->currentItem;

    switch ((*currentMenu)->menuId) {

        case BLEND:
            switch (j) {
                case BRIGHTNESS:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).BRIGHTNESS;
                    }
                    break;
                case CONTRAST:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).CONTRAST;
                    }
                    break;
            }
            break;

        case BRUSH:
            switch (j) {
                case BRUSH_SCALE:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).BRUSH_SCALE;
                    }
                    break;
                case BRUSH_OPACITY:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).BRUSH_OPACITY;
                    }
                    break;
            }
            break;

        case WARP:
            switch (j) {
                case CORNERPIN:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).CORNERPIN;
                            projectors->at(k).setKeystoneActive(true);
                        }
                    }
                    break;
                case GRID:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).GRID;
                            projectors->at(k).setGridActive(true);
                        }
                    }
                    break;
            }
            break;

        case HSL:
            switch (j) {
                case HUE:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).HUE;
                        }
                    }
                    break;
                case SATURATION:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).SATURATION;
                        }
                    }
                    break;
                case LIGHTNESS:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).LIGHTNESS;
                        }
                    }
                    break;
            }
            break;

        case GAMMA:
            switch (j) {
                case GAMMA_RGB:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).GAMMA;
                        }
                    }
                    break;
                case GAMMA_R:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).GAMMA_R;
                        }
                    }
                    break;
                case GAMMA_G:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).GAMMA_G;
                        }
                    }
                    break;
                case GAMMA_B:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).GAMMA_B;
                        }
                    }
                    break;
            }
            break;

        case RADIUS:
            switch (j) {
                case DOME_RADIUS:
                    dome->editMode = dome->RADIUS;
                    break;
            }
            break;

        case POSITION:
            switch (j) {
                case AZIMUTH:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).AZIMUTH;
                    }
                    break;
                case ELEVATION:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).ELEVATION;
                    }
                    break;
                case DISTANCE:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).DISTANCE;
                    }
                    break;
            }
            break;

        case ORIENTATION:
            switch (j) {
                case ROLL:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                                projectors->at(k).editMode = projectors->at(k).ROLL;
                    }
                    break;
                case TILT:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).TILT;
                    }
                    break;
                case PAN:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).PAN;
                    }
                    break;
            }
            break;

        case LENS:
            switch (j) {
                case FOV:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).FOV;
                    }
                    break;
            }

            break;

        case SCALE:
            switch (j) {
                case SCALE_XY:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SCALE;
                    }
                    break;
                case SCALE_X:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SCALE_X;
                    }
                    break;
                case SCALE_Y:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SCALE_Y;
                    }
                    break;
            }

            break;

        case SHEAR:
            switch (j) {
                case SHEAR_YZ:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SHEAR_YZ;
                    }
                    break;
                case SHEAR_ZX:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SHEAR_ZX;
                    }
                    break;
                case SHEAR_XZ:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SHEAR_XZ;
                    }
                    break;
                case SHEAR_ZY:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SHEAR_ZY;
                    }
                    break;
                case SHEAR_YX:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SHEAR_YX;
                    }
                    break;
                case SHEAR_XY:
                    for (int k=0; k<projCount; k++){
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).SHEAR_XY;
                    }
                    break;
            }
            break;
        default:
            for (int k=0; k<projCount; k++)
                projectors->at(k).editMode = projectors->at(k).NONE;
            break;
    }
}


void Menu::keyReleased(int key) {

    for (int i=0; i<projCount; i++) {
        projectors->at(i).keyReleased(key);
        projectors->at(i).mask.keyReleased(key);
    }

    switch(key){

        case OF_KEY_ALT:
            value = orgValue;
            alt = false;
            break;

        case OF_KEY_CONTROL:
            ctrl = false;
            break;

        case OF_KEY_SHIFT:
            value = orgValue;
            shift = false;
            break;

        case OF_KEY_SUPER:
            ctrl = false;
            break;
    }

    for (int i=0; i<projCount; i++) {
        projectors->at(i).setValue(value);
    }

}

/******************************************

 MATH

 ********************************************/

float round(float d) {
    return floorf(d + 0.5);
}

float Menu::roundTo(float val, float n){
	return round(val * 1/n) * n;
}

}
