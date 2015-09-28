#include "menu.h"
#include "commands.h"
namespace vd {

ofEvent<ofVec3f> Menu::colorEvent = ofEvent<ofVec3f>();
ofEvent<int>     Menu::sourceColorEvent = ofEvent<int>();
    
extern int maxHistory;
extern CommandHistory history;
extern vector<ofPixels> maskHistory;
    
/******************************************

 CONSTRUCTOR

 ********************************************/

Menu::Menu(){
	menuMain = new MenuItem;
    menuMain->menuId = MAIN;
    menuMain->parent = &menuMain;
    menuMain->currentItem = 0;
	
	menuMain->items.push_back(new Item("Enable"));
    menuMain->items.push_back(new Item("Warp               ->", true));
    menuMain->items.push_back(new Item("Blend              ->", true));
    menuMain->items.push_back(new Item("Color              ->", true));
    menuMain->items.push_back(new Item("Setup              ->", true));

    menuWarp = new MenuItem;
    menuWarp->menuId = WARP;
    menuWarp->parent = &menuMain;
    menuWarp->currentItem = 0;
    menuWarp->items.push_back(new Item("Cornerpin"));
	menuWarp->items.push_back(new Item("Bezier Grid"));

    menuBlend = new MenuItem;
    menuBlend->menuId = BLEND;
    menuBlend->parent = &menuMain;
    menuBlend->currentItem = 0;
    menuBlend->items.push_back(new Item("Brush              ->", true));

    menuBrush = new MenuItem;
    menuBrush->menuId = BRUSH;
    menuBrush->parent = &menuBlend;
    menuBrush->currentItem = 0;
    menuBrush->items.push_back(new Item("Brush Scale"));
    menuBrush->items.push_back(new Item("Brush Opacity"));

    menuColor = new MenuItem;
    menuColor->menuId = COLOR;
    menuColor->parent = &menuMain;
    menuColor->currentItem = 0;
    menuColor->items.push_back(new Item("Brightness"));
    menuColor->items.push_back(new Item("Contrast"));
    menuColor->items.push_back(new Item("Saturation"));
    menuColor->items.push_back(new Item("Curves             ->", true));

	menuCurves = new MenuItem;
    menuCurves->menuId = CURVES;
    menuCurves->parent = &menuColor;
    menuCurves->currentItem = 0;
	menuCurves->items.push_back(new Item("Grey               ->", true));
    menuCurves->items.push_back(new Item("Red                ->", true));
    menuCurves->items.push_back(new Item("Green              ->", true));
    menuCurves->items.push_back(new Item("Blue               ->", true));

	menuCurvesGrey = new MenuItem;
    menuCurvesGrey->menuId = CURVES_GREY;
	menuCurvesGrey->parent = &menuCurves;
	menuCurvesGrey->currentItem = 0;
    menuCurvesGrey->items.push_back(new Item(""));

	menuCurvesRed = new MenuItem;
    menuCurvesRed->menuId = CURVES_RED;
	menuCurvesRed->parent = &menuCurves;
	menuCurvesRed->currentItem = 0;
    menuCurvesRed->items.push_back(new Item(""));

	menuCurvesGreen = new MenuItem;
    menuCurvesGreen->menuId = CURVES_GREEN;
	menuCurvesGreen->parent = &menuCurves;
	menuCurvesGreen->currentItem = 0;
    menuCurvesGreen->items.push_back(new Item(""));

	menuCurvesBlue = new MenuItem;
    menuCurvesBlue->menuId = CURVES_BLUE;
	menuCurvesBlue->parent = &menuCurves;
	menuCurvesBlue->currentItem = 0;
    menuCurvesBlue->items.push_back(new Item(""));

    menuSetup = new MenuItem;
    menuSetup->menuId = SETUP;
    menuSetup->parent = &menuMain;
    menuSetup->currentItem = 0;
    menuSetup->items.push_back(new Item("Position           ->", true));
    menuSetup->items.push_back(new Item("Orientation        ->", true));
    menuSetup->items.push_back(new Item("Field of View      ->", true));
    menuSetup->items.push_back(new Item("Lens Offset        ->", true));

    menuPosition = new MenuItem;
    menuPosition->menuId = POSITION;
    menuPosition->parent = &menuSetup;
    menuPosition->currentItem = 0;
    menuPosition->items.push_back(new Item("Azimuth"));
    menuPosition->items.push_back(new Item("Elevation"));
    menuPosition->items.push_back(new Item("Distance"));

    menuOrientation = new MenuItem;
    menuOrientation->menuId = ORIENTATION;
    menuOrientation->parent = &menuSetup;
    menuOrientation->currentItem = 0;
    menuOrientation->items.push_back(new Item("Tilt"));
    menuOrientation->items.push_back(new Item("Roll"));
    menuOrientation->items.push_back(new Item("Pan"));

    menuFov = new MenuItem;
    menuFov->menuId = FIELD_OF_VIEW;
    menuFov->parent = &menuSetup;
    menuFov->currentItem = 0;
    menuFov->items.push_back(new Item("Field of View"));

    menuLens = new MenuItem;
    menuLens->menuId = LENS;
    menuLens->parent = &menuSetup;
    menuLens->currentItem = 0;
    menuLens->items.push_back(new Item("Offset X"));
    menuLens->items.push_back(new Item("Offset Y"));

    currentMenu = &menuMain;

    frameCnt= 0;
    saved = false;
    active = false;

    // add values
    value = 1;
    orgValue = 1;
    ctrlValue = .1;
    altValue = .01;

    #ifdef TARGET_OSX
        cKey = OF_KEY_COMMAND;
    #else
        cKey = OF_KEY_CONTROL;
    #endif

    // layout
    pw = 200;
    ph = 145;
    padx = 15;
    pady = 15;

    curvePointIndex = 0;
}

/******************************************

 Setup

 ********************************************/

void Menu::setup(){
    maskHistory.clear();
	for (int i=0; i<=(maxHistory+2); i++) {
        ofPixels buffer;
        maskHistory.push_back(buffer);
	}
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
    ofDrawRectangle(px, py, 1, pw, ph);
}

void Menu::drawMain(int i){
    string str;
    string val;

    for(int j=0; j<(*currentMenu)->items.size(); j++) {
        str = (*currentMenu)->items[j]->str;

        if (j == (*currentMenu)->currentItem)
            drawHighlight();

		if ((*currentMenu)->menuId == MAIN){ 
            if (projectors->at(i).enable) val = "On";
            else val = "Off";
		}
        else if ((*currentMenu)->menuId != MAIN &&
            (*currentMenu)->menuId != WARP &&
            (*currentMenu)->menuId != SETUP) {

            switch ((*currentMenu)->menuId) {

                case COLOR:
                    switch (j) {
                        case BRIGHTNESS:
                            val = ofToString(roundTo(projectors->at(i).brightness, .001));
                            break;
                        case CONTRAST:
                            val = ofToString(roundTo(projectors->at(i).contrast, .001));
                            break;
                        case SATURATION:
                            val = ofToString(roundTo(projectors->at(i).saturation, .001));
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

                case FIELD_OF_VIEW:
                    switch (j) {
                        case FOV:
                            val = ofToString(roundTo(projectors->at(i).getCameraFov(), .01));
                            break;
                    }
                    break;

                case LENS:
                    switch (j) {
                        case OFFSET_X:
                            val = ofToString(roundTo(projectors->at(i).getCameraOffset().x, .001));
                            break;
                        case OFFSET_Y:
                            val = ofToString(roundTo(projectors->at(i).getCameraOffset().y, .001));
                            break;
                    }

                    break;

            }

        }

		// value string
		if (str.find("->") != 19) { // nested menu exception
			while (str.size() + val.size() < 21) {
				str += " ";
			}
			str += val;
		}

        ofDrawBitmapString(str, px+padx, py+pady * (3.25+j) );

        if (j == (*currentMenu)->currentItem)
            ofSetHexColor(0xFFFFFF);
    }
}

void Menu::update(){
    if (inputSource == SOURCE_COLOR){

        newCurvePointIndex = false;
        
        for (auto p : *projectors){
            if (p.curves.getCurrentHover() != curvePointIndex){
                curvePointIndex = p.curves.getCurrentHover();
                newCurvePointIndex = true;
                break;
            }
        }
        
        if (!newCurvePointIndex)
            return;
        
        for (auto p : *projectors){
            p.curves.setCurrentHover(curvePointIndex);
        }
        
        if (ofGetKeyPressed('g') || ofGetKeyPressed('G')){
            currentColor = updateColorFromCurve(curvePointIndex, true);
        }
        else{
            currentColor = updateColorFromCurve(curvePointIndex, false);
        }
        
       ofNotifyEvent(colorEvent,currentColor,this);
    }
}

    
    
    
void Menu::draw(int i, int labelIndex){
    if (projectors->at(i).curves.enabled){
        projectors->at(i).curves.setLabelPosition(16, 45);
        pw = 256;
        ph = 256;

        px = projectors->at(i).getPlanePosition().x + projectors->at(i).width/2 - pw/2;
        py = projectors->at(i).getPlanePosition().y + projectors->at(i).height/2  - ph/2;
        
        //drawBackground();
        drawCurves(i);

        ofSetColor(255);
        drawProjector(labelIndex);
        drawFPS(i);
        drawActive(i);
        drawSaved();
    }

    else{
        projectors->at(i).curves.setLabelPosition();

        pw = 200;
        ph = 145;

        px = projectors->at(i).getPlanePosition().x + projectors->at(i).width/2 - pw/2;
        py = projectors->at(i).getPlanePosition().y + projectors->at(i).height/2  - ph/2;

        drawBackground();
        ofSetColor(255);
        drawProjector(labelIndex);
        drawFPS(i);
        drawMain(i);
        drawWarp(i);
        drawActive(i);
        drawSaved();
    }

    if ((*currentMenu)->menuId == CURVES_GREY
        || (*currentMenu)->menuId == CURVES_RED 
        || (*currentMenu)->menuId == CURVES_GREEN 
        || (*currentMenu)->menuId == CURVES_BLUE) {
    }
    else if (inputSource == SOURCE_COLOR){
        inputSource = storedSource;
        ofNotifyEvent(sourceColorEvent,inputSource,this);
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
        if (projectors->at(i).active) {
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

void Menu::drawCurves(int i){
	projectors->at(i).drawCurves(px, py);
}

void Menu::drawProjector(int i){
    ofDrawBitmapString("Projector #" + ofToString(i+1), px+padx, py+pady * 1.75);
}

void Menu::drawFPS(int i){
    if (!saved) {
        if (!projectors->at(i).active) {
            ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), px+padx + 125, py+pady * 1.75);
		}
    }
}

void Menu::toggle() {
    active = !active;
}

/******************************************

 NAVIGATION

 ********************************************/

void Menu::select() {

    int item = (*currentMenu)->currentItem;
    switch ((*currentMenu)->menuId) {
        case MAIN:
            switch (item) {
                case 1: currentMenu = &menuWarp; break;
                case 2: currentMenu = &menuBlend; break;
                case 3: currentMenu = &menuColor; break;
                case 4: currentMenu = &menuSetup; break;
                default: break;
            }
            break;
        case SETUP:
            switch (item) {
                case 0: currentMenu = &menuPosition; break;
                case 1: currentMenu = &menuOrientation; break;
                case 2: currentMenu = &menuFov; break;
                case 3: currentMenu = &menuLens; break;
                case 4: currentMenu = &menuShear; break;
                case 5: currentMenu = &menuScale; break;
                default: break;
            }
            break;
        case BLEND:
            switch (item) {
                case 0: currentMenu = &menuBrush; break;
                default: break;
            }
            break;
        case COLOR:
            switch (item) {
				case 3: currentMenu = &menuCurves; break;
                default: break;
            }
            break;
		case CURVES:
            switch (item) {
				case 0: currentMenu = &menuCurvesGrey; break;
				case 1: currentMenu = &menuCurvesRed; break;
				case 2: currentMenu = &menuCurvesGreen; break;
				case 3: currentMenu = &menuCurvesBlue; break;
                default: break;
            }
            break;
        default: break;
    }
    
	setEditMode();
    changeColorCurveMode((*currentMenu)->menuId);
}

void Menu::changeColorCurveMode(int i){
    switch (i) {
		case CURVES_GREY:
			storedSource = inputSource;
			inputSource = SOURCE_COLOR;
            currentColor = updateColorFromCurve(0, false);
			
            for (int k=0; k<projCount; k++) {
                projectors->at(k).curves.enabled = true;
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.GREY );
				projectors->at(k).curves.setCurrentHover(0);
            }

            break;
		
        case CURVES_RED:
            storedSource = inputSource;
            inputSource = SOURCE_COLOR;
            currentColor = updateColorFromCurve(0, false);
            
            for (int k=0; k<projCount; k++) {
                projectors->at(k).curves.enabled = true;
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.RED );
				projectors->at(k).curves.setCurrentHover(0);
            }
            
            break;
		
        case CURVES_GREEN:
            storedSource = inputSource;
            inputSource = SOURCE_COLOR;
            currentColor = updateColorFromCurve(0, false);
            
			for (int k=0; k<projCount; k++) {
                projectors->at(k).curves.enabled = true;
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.GREEN );
				projectors->at(k).curves.setCurrentHover(0);
            }

            break;
		
        case CURVES_BLUE:
            storedSource = inputSource;
            inputSource = SOURCE_COLOR;
            currentColor = updateColorFromCurve(0, false);
            
			for (int k=0; k<projCount; k++) {
                projectors->at(k).curves.enabled = true;
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.BLUE );
				projectors->at(k).curves.setCurrentHover(0);
            }
            
            break;
        default: 
			break;
    }
    
}
    
void Menu::back() {
    currentMenu = (*currentMenu)->parent;
    setEditMode();
}


ofVec3f Menu::updateColorFromCurve(int pointIndex, bool forceGrey){
    curvePointIndex = pointIndex;
    int displayColor = ofClamp(256/8 * pointIndex, 0, 255);
    ofVec3f displayVector;
	if (projectors->size()){
		if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.GREY || forceGrey){
            displayVector.set(displayColor, displayColor, displayColor);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.RED){
            displayVector.set(displayColor, 0, 0);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.GREEN){
            displayVector.set(0, displayColor, 0);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.BLUE){
            displayVector.set(0, 0, displayColor);
		}
	}
    return displayVector;
}

/******************************************

 MOUSE

 ********************************************/

void Menu::mousePressed(ofMouseEventArgs& mouseArgs) {
    if (active) {
        for (int i=0; i<projCount; i++) {
            projectors->at(i).mousePressed(mouseArgs);
        }

		if (ofGetKeyPressed(OF_KEY_ALT)){
			value = altValue;
		}
		else if (ofGetKeyPressed(cKey)){
			value = ctrlValue;
		}
		else {
			value = orgValue;
		}
		for (int i=0; i<projCount; i++) {
			projectors->at(i).setValue(value);
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


		vector<Command*> cmds;
		for (int k=0; k<projCount; k++) {
			if (projectors->at(k).editMode == projectors->at(k).CORNERPIN || projectors->at(k).editMode == projectors->at(k).GRID) {	
				if (projectors->at(k).active) {
					Command* cmd = projectors->at(k).execute(0);
					if (cmd)
						cmds.push_back(cmd);
				}
			}
		}
		

		for (int i=0; i<projCount; i++) {
            projectors->at(i).mouseReleased(mouseArgs);
        }


		for (int k=0; k<projCount; k++) {
			if (projectors->at(k).editMode == projectors->at(k).BRUSH_SCALE || projectors->at(k).editMode == projectors->at(k).BRUSH_OPACITY) {	
				if (projectors->at(k).active) {
					Command* cmd = projectors->at(k).executeBrush();
					if (cmd)
						cmds.push_back(cmd);
				}
			}
		}
		


		if (cmds.size()){
			history.execute(new SetProjectors(cmds));
		}

	}
}

/******************************************

 KEYBOARD

 ********************************************/

void Menu::keyPressed(int key) {

    // SET VALUE
    if (ofGetKeyPressed(OF_KEY_ALT)){
        value = altValue;
    }
    else if (ofGetKeyPressed(cKey)){
        value = ctrlValue;
    }
    else {
        value = orgValue;
    }

    for (int i=0; i<projCount; i++) {
        projectors->at(i).setValue(value);
    }

	for (int k=0; k<projCount; k++) {
		projectors->at(k).keyPressed(key);
	}
    
    // MENU NOT ACTIVE
    ///////////////////////////
    if (!active) { return; }
    ///////////////////////////

    
	if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT || key == 114){

			vector<Command*> cmds;

			if (key == 114){

				for (int k=0; k<projCount; k++) {
					if (projectors->at(k).active) {
						Command* cmd = projectors->at(k).reset();
						if (cmd)
							cmds.push_back(cmd);
					}
				}			
			
			}
			else {

				int val;
				if (key == OF_KEY_LEFT)
					val = -1;
				else if (key == OF_KEY_RIGHT)
					val = 1;
			
				for (int k=0; k<projCount; k++) {
					if (projectors->at(k).active) {
						Command* cmd = projectors->at(k).execute(val);
						if (cmd)
							cmds.push_back(cmd);
					}
				}


			}
        if (cmds.size()){
				history.execute(new SetProjectors(cmds));
        }
    }


    switch (key){

        case 122: // (z)
            history.undo();
            break;
        case 121: // (y)
            history.redo();
            break;

        case OF_KEY_UP:
			if ((*currentMenu)->menuId == CURVES_GREY  
				|| (*currentMenu)->menuId == CURVES_RED 
				|| (*currentMenu)->menuId == CURVES_GREEN 
				|| (*currentMenu)->menuId == CURVES_BLUE) {
					// do nothing
		    }
			else{
				if ((*currentMenu)->currentItem > 0){
					(*currentMenu)->currentItem--;
				}
				setEditMode();
			}
            break;

        case OF_KEY_DOWN:
			if ((*currentMenu)->menuId == CURVES_GREY  
				|| (*currentMenu)->menuId == CURVES_RED 
				|| (*currentMenu)->menuId == CURVES_GREEN 
				|| (*currentMenu)->menuId == CURVES_BLUE) {
					// do nothing
		    }
			else{
				if ((*currentMenu)->currentItem < (*currentMenu)->items.size()-1){
					(*currentMenu)->currentItem++;
				}
				setEditMode();
			}

            break;

        case OF_KEY_RIGHT:
			if ((*currentMenu)->items.at( (*currentMenu)->currentItem )->isParent ) {
                select();
                return;
            }
            break;

        case OF_KEY_RETURN:
            select();
            break;

        case OF_KEY_BACKSPACE:
            back();
            break;
    }

    if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT || key == 114) { // r = 114
		
		if ((*currentMenu)->menuId == CURVES_GREY  
			|| (*currentMenu)->menuId == CURVES_RED 
			|| (*currentMenu)->menuId == CURVES_GREEN 
			|| (*currentMenu)->menuId == CURVES_BLUE) {

			if (key == OF_KEY_LEFT){
				if (projectors->size()){
					projectors->at(0).curves.prevPoint(); // other points update on event
				}
			}
			else if (key == OF_KEY_RIGHT){
				if (projectors->size()){
					projectors->at(0).curves.nextPoint(); // other points update on event
				}
			}

		}

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
    else if (key == 33) key = 49;
    else if (key == 64)	key = 50;
    else if (key == 35)	key = 51;
    else if (key == 36) key = 52;
    else if (key == 37) key = 53;
    else if (key == 94) key = 54;
    else if (key == 38) key = 55;
    else if (key == 42) key = 56;
    else if (key == 40) key = 57;
    else if (key == 41) key = 48;

    if (key >= 48 && key <= 57) {

        // map key to projector
        if (key == 48)
            pActive = 10;
        else
            pActive = key-49;
        
        pActive -= projectorStartingIndex;
        
        if (pActive < projCount && pActive >= 0) {

           // shift groups, otherwise reset
           if (!ofGetKeyPressed(OF_KEY_SHIFT)) {
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

            //fix: need to set mouse active using native window
           if (projectors->at(pActive).active) {
               
               shared_ptr<ofAppGLFWWindow> glfw =
                    static_pointer_cast<ofAppGLFWWindow>(ofGetMainLoop()->getCurrentWindow());
               
               glfwSetCursorPos(glfw->getGLFWWindow(), xmouse, ymouse);
               
           }
            
           setEditMode();
       }
    }

	for (int i=0; i<projCount; i++) {
		projectors->at(i).curves.setActive( projectors->at(i).active );
    }
}

void Menu::setEditMode() {

	if ((*currentMenu)->menuId == CURVES_GREY  
		|| (*currentMenu)->menuId == CURVES_RED 
		|| (*currentMenu)->menuId == CURVES_GREEN 
		|| (*currentMenu)->menuId == CURVES_BLUE) {
			// do nothing
			return;
	}

    for (int k=0; k<projCount; k++) {
        projectors->at(k).editMode = projectors->at(k).NONE;
        projectors->at(k).setKeystoneActive(false);
        projectors->at(k).setGridActive(false);
		projectors->at(k).curves.enabled = false;
    }

    int j = (*currentMenu)->currentItem;

    switch ((*currentMenu)->menuId) {

        case MAIN:
            switch (j) {
                case ENABLE:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).ENABLE;
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

        case COLOR:
            switch (j) {
                case SATURATION:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).SATURATION;
                        }
                    }
                    break;
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

		case CURVES:
				 for (int k=0; k<projCount; k++) {
                      projectors->at(k).editMode = projectors->at(k).CURVES;
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

        case FIELD_OF_VIEW:
            switch (j) {
                case FOV:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).FOV;
                    }
                    break;
            }
            break;

        case LENS:
            switch (j) {
                case OFFSET_X:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).OFFSET_X;
                    }
                    break;
                case OFFSET_Y:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).OFFSET_Y;
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
    for (int i=0; i<projCount; i++) {
        projectors->at(i).setValue(value);
    }
}





/******************************************

 MATH

 ********************************************/

float Menu::round(float d) {
    return floorf(d + 0.5);
}

float Menu::roundTo(float val, float n){
	return round(val * 1/n) * n;
}

}
