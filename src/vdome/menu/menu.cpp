#include "menu.h"
#include "commands.h"
namespace vd {

//--------------------------------------------------------------
ofEvent<ofVec3f> Menu::colorEvent = ofEvent<ofVec3f>();
ofEvent<int>     Menu::sourceColorEvent = ofEvent<int>();
    
//--------------------------------------------------------------
extern int maxHistory;
extern CommandHistory history;

//--------------------------------------------------------------
Menu::Menu(){
	menuMain = new MenuItem;
    menuMain->menuId = MAIN;
    menuMain->parent = &menuMain;
    menuMain->currentItem = 0;
	
    // main menu
	menuMain->items.push_back(new Item("Enable"));
	menuMain->items.push_back(new Item("View               ->", true));
	menuMain->items.push_back(new Item("Warp               ->", true));
    menuMain->items.push_back(new Item("Blend              ->", true));
    menuMain->items.push_back(new Item("Color              ->", true));

	// view menu
	menuView = new MenuItem;
	menuView->menuId = VIEW;
	menuView->parent = &menuMain;
	menuView->currentItem = 0;
	menuView->items.push_back(new Item("Field of View"));

    // warp menu
    menuWarp = new MenuItem;
    menuWarp->menuId = WARP;
    menuWarp->parent = &menuMain;
    menuWarp->currentItem = 0;
    menuWarp->items.push_back(new Item("Grid Warp"));

    // blend menu
    menuBlend = new MenuItem;
    menuBlend->menuId = BLEND;
    menuBlend->parent = &menuMain;
    menuBlend->currentItem = 0;
	menuBlend->items.push_back(new Item("Brightness"));
	menuBlend->items.push_back(new Item("Contrast"));

    // color menu
    menuColor = new MenuItem;
    menuColor->menuId = COLOR;
    menuColor->parent = &menuMain;
    menuColor->currentItem = 0;

    menuColor->items.push_back(new Item("Saturation"));
    menuColor->items.push_back(new Item("Curves             ->", true));

    // color curves menu
	menuCurves = new MenuItem;
    menuCurves->menuId = CURVES;
    menuCurves->parent = &menuColor;
    menuCurves->currentItem = 0;
	menuCurves->items.push_back(new Item("Grey               ->", true));
    menuCurves->items.push_back(new Item("Red                ->", true));
    menuCurves->items.push_back(new Item("Green              ->", true));
    menuCurves->items.push_back(new Item("Blue               ->", true));

    // color curves grey
	menuCurvesGrey = new MenuItem;
    menuCurvesGrey->menuId = CURVES_GREY;
	menuCurvesGrey->parent = &menuCurves;
	menuCurvesGrey->currentItem = 0;
    menuCurvesGrey->items.push_back(new Item(""));
    
    // color curves red
	menuCurvesRed = new MenuItem;
    menuCurvesRed->menuId = CURVES_RED;
	menuCurvesRed->parent = &menuCurves;
	menuCurvesRed->currentItem = 0;
    menuCurvesRed->items.push_back(new Item(""));
    
    // color curves green
	menuCurvesGreen = new MenuItem;
    menuCurvesGreen->menuId = CURVES_GREEN;
	menuCurvesGreen->parent = &menuCurves;
	menuCurvesGreen->currentItem = 0;
    menuCurvesGreen->items.push_back(new Item(""));
   
    // color curves blue
	menuCurvesBlue = new MenuItem;
    menuCurvesBlue->menuId = CURVES_BLUE;
	menuCurvesBlue->parent = &menuCurves;
	menuCurvesBlue->currentItem = 0;
    menuCurvesBlue->items.push_back(new Item(""));

	// reset current menu
    currentMenu = &menuMain;

	// reset states
    frameCnt= 0;
    saved = false;
    active = false;
	curvePointIndex = 0;

    // reset value and modifiers
    value = 1;
    orgValue = 1;
    ctrlValue = .1;
    altValue = .01;

	// set control key
    #ifdef TARGET_OSX
        cKey = OF_KEY_COMMAND;
    #else
        cKey = OF_KEY_CONTROL;
    #endif

    // set layout
    pw = 200;
    ph = 130;
    padx = 15;
    pady = 15;
}

//--------------------------------------------------------------
void Menu::drawHighlight(){
    ofSetHexColor(0xFAFA00);
}

//--------------------------------------------------------------
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
        ofSetColor(0, 0, 0, 225);
    }
    // background shape
    ofFill();
    ofDrawRectangle(px, py, 1, pw, ph);
}

//--------------------------------------------------------------
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
            (*currentMenu)->menuId != WARP) {

            switch ((*currentMenu)->menuId) {
			
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
                case COLOR:
                    switch (j) {
                        case SATURATION:
                            val = ofToString(roundTo(projectors->at(i).saturation, .001));
                            break;
                    }
                    break;
				case VIEW:
					switch (j) {
					case FOV:
						val = ofToString(roundTo(projectors->at(i).getCameraFov(), .1));
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

//--------------------------------------------------------------
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
    
    if ((*currentMenu)->menuId == VIEW) {
        for (int k = 0; k<projCount; k++) {
            if (projectors->at(k).active) {
                projectors->at(k).editMode = projectors->at(k).FOV;
                projectors->at(k).camera.enableMouseInput();
            }
        }
    }
	if ((*currentMenu)->menuId == WARP) {
		for (int k = 0; k<projCount; k++) {
			if (projectors->at(k).active) {
				projectors->at(k).editMode = projectors->at(k).GRID;
				projectors->at(k).setWarpActive(true);
			}

		}
	}
}
//--------------------------------------------------------------
void Menu::draw(int i, int labelIndex){
    if (projectors->at(i).curves.enabled){ // curve menu
        projectors->at(i).curves.setLabelPosition(16, 45);
        pw = 256;

        px = projectors->at(i).getPlanePosition().x + projectors->at(i).getWidth()/2 - pw/2;
        py = projectors->at(i).getPlanePosition().y + projectors->at(i).getHeight()/2  - ph/2;
        
        drawCurves(i);

        ofSetColor(255);
        drawProjector(labelIndex);
        drawFPS(i);
        drawActive(i);
        drawSaved();
    }

    else{ // main menu
        projectors->at(i).curves.setLabelPosition();

        pw = 200;

        px = projectors->at(i).getPlanePosition().x + projectors->at(i).getWidth()/2 - pw/2;
        py = projectors->at(i).getPlanePosition().y + projectors->at(i).getHeight()/2  - ph/2;

        drawBackground();
        ofSetColor(255);
        drawProjector(labelIndex);
        drawFPS(i);
        drawMain(i);
        drawActive(i);
        drawSaved();
    }

    // switch back to input
    if ( (*currentMenu)->menuId == CURVES) {
        if (inputSource == SOURCE_COLOR){
            inputSource = storedSource;
            ofNotifyEvent(sourceColorEvent,inputSource,this);
        }
    }

}

//--------------------------------------------------------------
void Menu::drawSaved(){
    if (saved) {
        ofSetHexColor(0xFFFFFF);
        ofDrawBitmapString("SAVED", px+padx+125, py+pady*1.75);
    }
}

//--------------------------------------------------------------
void Menu::drawActive(int i){
    if (!saved) {
        if (projectors->at(i).active) {
            ofSetHexColor(0xFFF000);
            ofDrawBitmapString("Active", px+padx+120, py+pady*1.75);
            ofSetHexColor(0xFFFFFF);
        }
    }
}

//--------------------------------------------------------------
void Menu::drawCurves(int i){
    projectors->at(i).curves.draw(px, py);
}

//--------------------------------------------------------------
void Menu::drawProjector(int i){
    ofDrawBitmapString("Projector #" + ofToString(i+1), px+padx, py+pady * 1.75);
}

//--------------------------------------------------------------
void Menu::drawFPS(int i){
    if (!saved) {
        if (!projectors->at(i).active) {
            ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), px+padx + 125, py+pady * 1.75);
		}
    }
}

//--------------------------------------------------------------
void Menu::toggle() {
    active = !active;
}

//--------------------------------------------------------------
void Menu::select() {
    int item = (*currentMenu)->currentItem;
    switch ((*currentMenu)->menuId) {
        case MAIN:
            switch (item) {
                case 1: currentMenu = &menuView; break;
                case 2: currentMenu = &menuWarp; break;
                case 3: currentMenu = &menuBlend; break;
                case 4: currentMenu = &menuColor; break;
                default: break;
            }
            break;
        case COLOR:
            switch (item) {
				case 1: currentMenu = &menuCurves; break;
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

//--------------------------------------------------------------
void Menu::changeColorCurveMode(int i){
    switch (i) {
		case CURVES_GREY:
            for (int k=0; k<projCount; k++) {
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.GREY );
            }
            break;
		case CURVES_RED:            
            for (int k=0; k<projCount; k++) {
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.RED );
            }
            break;
		case CURVES_GREEN: 
			for (int k=0; k<projCount; k++) {
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.GREEN );
            }
            break;
        case CURVES_BLUE:
			for (int k=0; k<projCount; k++) {
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.BLUE );
            }
            break;
        default: return; break;
    }
	storedSource = inputSource;
	inputSource = SOURCE_COLOR;
	currentColor = updateColorFromCurve(0, false);

	for (int k = 0; k < projCount; k++) {
		projectors->at(k).curves.enabled = true;
		projectors->at(k).curves.setCurrentHover(0);
	}
    
    ofNotifyEvent(colorEvent,currentColor,this);
}
    
//--------------------------------------------------------------
void Menu::back() {
    currentMenu = (*currentMenu)->parent;
    setEditMode();
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
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
    
//--------------------------------------------------------------
void Menu::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (active) {
        for (int i=0; i<projCount; i++) {
            projectors->at(i).mouseDragged(mouseArgs);
        }
    }
}
  
//--------------------------------------------------------------
void Menu::mouseReleased(ofMouseEventArgs& mouseArgs) {
    if (active) {
		vector<Command*> cmds;
		for (int k=0; k<projCount; k++) {
			if (projectors->at(k).editMode == projectors->at(k).GRID) {	
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

		if (cmds.size()){
			history.execute(new SetProjectors(cmds));
		}

	}
}

//--------------------------------------------------------------
void Menu::mouseMoved(ofMouseEventArgs& mouseArgs) {
	if (active) {
		for (int i = 0; i<projCount; i++) {
			projectors->at(i).mouseMoved(mouseArgs);
		}
	}
}

//--------------------------------------------------------------
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
           int xmouse = projectors->at(pActive).getPlanePosition().x+projectors->at(pActive).getWidth()/2;
           int ymouse = projectors->at(pActive).getPlanePosition().y+projectors->at(pActive).getHeight()/2;

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

//--------------------------------------------------------------
void Menu::setEditMode() {

	if ((*currentMenu)->menuId == CURVES_GREY  
		|| (*currentMenu)->menuId == CURVES_RED 
		|| (*currentMenu)->menuId == CURVES_GREEN 
		|| (*currentMenu)->menuId == CURVES_BLUE) {
			// do nothing
			return;
	}

	// reset
    for (int k=0; k<projCount; k++) {
        projectors->at(k).editMode = projectors->at(k).NONE;
		projectors->at(k).curves.enabled = false;
		projectors->at(k).setWarpActive(false);
        projectors->at(k).camera.disableMouseInput();
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
		
		case VIEW:
			switch (j) {
			case FOV:
				for (int k = 0; k<projCount; k++) {
                    if (projectors->at(k).active){
						projectors->at(k).editMode = projectors->at(k).FOV;
                        projectors->at(k).camera.enableMouseInput();
                    }
                }
				break;
			}
			break;

        case WARP:
            switch (j) {
                case GRID:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active){
                            projectors->at(k).editMode = projectors->at(k).GRID;
                            projectors->at(k).setWarpActive(true);
                        }
                    }
                    break;
            }
            break;
		
		case BLEND:
			switch (j) {
			case BRIGHTNESS:
				for (int k = 0; k<projCount; k++) {
					if (projectors->at(k).active)
						projectors->at(k).editMode = projectors->at(k).BRIGHTNESS;
				}
				break;
			case CONTRAST:
				for (int k = 0; k<projCount; k++) {
					if (projectors->at(k).active)
						projectors->at(k).editMode = projectors->at(k).CONTRAST;
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
            }
            break;

		case CURVES:
				 for (int k=0; k<projCount; k++) {
                      projectors->at(k).editMode = projectors->at(k).CURVES;
				}
            break;

        default:
            for (int k=0; k<projCount; k++)
                projectors->at(k).editMode = projectors->at(k).NONE;
            break;
    }
}

//--------------------------------------------------------------
void Menu::keyReleased(int key) {
    for (int i=0; i<projCount; i++) {
        projectors->at(i).keyReleased(key);
    }
    for (int i=0; i<projCount; i++) {
        projectors->at(i).setValue(value);
    }
}

//--------------------------------------------------------------
float Menu::round(float d) {
    return floorf(d + 0.5);
}

//--------------------------------------------------------------
float Menu::roundTo(float val, float n){
	return round(val * 1/n) * n;
}

    
}//////////
