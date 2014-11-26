#include "menu.h"
namespace vd {

extern float projCount;

/******************************************

 CONSTRUCTOR

 ********************************************/

Menu::Menu(){

	menuMain = new MenuItem;
    menuMain->menuId = MAIN;
    menuMain->parent = &menuMain;
    menuMain->currentItem = 0;
	
	menuMain->items.push_back(new Item("Enable"));
    menuMain->items.push_back(new Item("Input              ->", true));
    menuMain->items.push_back(new Item("Warp               ->", true));
    menuMain->items.push_back(new Item("Blend              ->", true));
    menuMain->items.push_back(new Item("Color              ->", true));
    menuMain->items.push_back(new Item("Setup              ->", true));

    menuInput = new MenuItem;
    menuInput->menuId = INPUT;
    menuInput->parent = &menuMain;
    menuInput->currentItem = 0;
    menuInput->items.push_back(new Item("Source"));
    menuInput->items.push_back(new Item("Format"));
    menuInput->items.push_back(new Item("Loop"));
    menuInput->items.push_back(new Item("Transform          ->", true));

    menuInputTransform = new MenuItem;
    menuInputTransform->menuId = INPUT_TRANSFORM;
    menuInputTransform->parent = &menuInput;
    menuInputTransform->currentItem = 0;
    menuInputTransform->items.push_back(new Item("Flip"));
    menuInputTransform->items.push_back(new Item("Rotate"));
    menuInputTransform->items.push_back(new Item("Tilt"));
    menuInputTransform->items.push_back(new Item("Scale"));

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
    menuBlend->items.push_back(new Item("Brightness"));
    menuBlend->items.push_back(new Item("Contrast"));
    menuBlend->items.push_back(new Item("Levels             ->", true));
    menuBlend->items.push_back(new Item("Brush              ->", true));

    menuLevels = new MenuItem;
    menuLevels->menuId = LEVELS;
    menuLevels->parent = &menuBlend;
    menuLevels->currentItem = 0;
    menuLevels->items.push_back(new Item("Black"));
    menuLevels->items.push_back(new Item("White"));

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
    menuColor->items.push_back(new Item("HSL                ->", true));
    menuColor->items.push_back(new Item("Gamma              ->", true));
	menuColor->items.push_back(new Item("Curves             ->", true));

    menuHSL = new MenuItem;
    menuHSL->menuId = HSL;
    menuHSL->parent = &menuColor;
    menuHSL->currentItem = 0;
    menuHSL->items.push_back(new Item("Hue"));
    menuHSL->items.push_back(new Item("Saturation"));
    menuHSL->items.push_back(new Item("Lightness"));

    menuGamma = new MenuItem;
    menuGamma->menuId = GAMMA;
    menuGamma->parent = &menuColor;
    menuGamma->currentItem = 0;
    menuGamma->items.push_back(new Item("Gamma"));
    menuGamma->items.push_back(new Item("Red"));
    menuGamma->items.push_back(new Item("Green"));
    menuGamma->items.push_back(new Item("Blue"));

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
    menuSetup->items.push_back(new Item("Shear              ->", true));
    menuSetup->items.push_back(new Item("Scale              ->", true));

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

    menuShear = new MenuItem;
    menuShear->menuId = SHEAR;
    menuShear->parent = &menuSetup;
    menuShear->currentItem = 0;
    menuShear->items.push_back(new Item("Shear YZ"));
    menuShear->items.push_back(new Item("Shear ZX"));
    menuShear->items.push_back(new Item("Scale XZ"));
    menuShear->items.push_back(new Item("Shear ZY"));
    menuShear->items.push_back(new Item("Shear YX"));
    menuShear->items.push_back(new Item("Scale XY"));

    menuScale = new MenuItem;
    menuScale->menuId = SCALE;
    menuScale->parent = &menuSetup;
    menuScale->currentItem = 0;
    menuScale->items.push_back(new Item("Scale X"));
    menuScale->items.push_back(new Item("Scale Y"));

    currentMenu = &menuMain;




    frameCnt= 0;
    saved = false;
    autosave = false;
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


}


void Menu::onCurHoverChange(ofVec3f & xyi){
	for (int j=0; j<projCount; j++) {
		if ( projectors->at(j).curves.getCurrentHover() != xyi.z ){
			projectors->at(j).curves.setCurrentHover(xyi.z);
		}
    }

	int cpnt = xyi.x;
	int clmp = ofClamp(xyi.x, 0, 255);

	if (projectors->size()){
		if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.GREY){
				input->setColor(clmp, clmp, clmp);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.RED){
				input->setColor(clmp, 0, 0);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.GREEN){
				input->setColor(0, clmp, 0);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.BLUE){
				input->setColor(0, 0, clmp);
		}
	}

}


void Menu::onCurHoverUpdate(ofVec4f & xyip){

	int cx = ofClamp(xyip[0], 0, 255);
	int cy = ofClamp(xyip[1], 0, 255);	
	int ci = xyip[2];
	int pi = xyip[3];

	for (int j=0; j<projCount; j++) {
		if (j != ci){
			if ( projectors->at(j).curves.getCurrentHover() != ci ){
				projectors->at(j).curves.setCurrentHover(ci);
			}
			
			if ( projectors->at(j).active ){
				projectors->at(j).curves.setPoint(ci,ofPoint(cx,cy));
			}
		}
    }


	if (projectors->size()){
		if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.GREY){
				input->setColor(cx, cx, cx);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.RED){
				input->setColor(cx, 0, 0);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.GREEN){
				input->setColor(0, cx, 0);
		}
		else if (projectors->at(0).curves.getColorMode() == projectors->at(0).curves.BLUE){
				input->setColor(0, 0, cx);
		}
	}

}



void Menu::setup(){
	for (int i=0; i<projCount; i++) {
		ofAddListener(projectors->at(i).curves.curHoverChange,this, &Menu::onCurHoverChange);
		ofAddListener(projectors->at(i).curves.curHoverUpdate,this, &Menu::onCurHoverUpdate);
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
    if (saved && !autosave) {
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

                case INPUT:
                    switch (j) {
                        case SOURCE:
                            if (input->source == 0)		   val = "Media";
							else if (input->source == 1)   val = "Capture";
                            else if (input->source == 2)   val = "Syphon";
							else if (input->source == 3)   val = "Spout";
                            else if (input->source == 4)   val = "Grid";
                            else if (input->source == 5)   val = "Black";
                            else if (input->source == 6)   val = "White";
                            else if (input->source == 7)   val = "Grey";
                            break;
                        case FORMAT:
                            if (input->format == 0)         val = "Domemaster";
                            else if (input->format == 1)    val = "HD";
                            break;
                        case LOOP:
                            if (input->getLoop()) val = "On";
                            else val = "Off";
                            break;
                    }
                    break;

                case INPUT_TRANSFORM:
                    switch (j) {
                        case INPUT_FLIP:
                            if (model->textureFlip) val = "On";
                            else val = "Off";
                            break;
                        case INPUT_ROTATE:
                            val = ofToString(roundTo(model->textureRotate, .001));
                            break;
                        case INPUT_TILT:
                            val = ofToString(roundTo(model->textureTilt, .001));
                            break;
                        case INPUT_SCALE:
                            val = ofToString(roundTo(model->textureScale, .001));
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

                case LEVELS:
                    switch (j) {
                        case BLACK:
                            val = ofToString(projectors->at(i).blackLevel);
                            break;
                        case WHITE:
                            val = ofToString(projectors->at(i).whiteLevel);
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

                case SCALE:
                    switch (j) {
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




void Menu::draw(int i){
   
		if (projectors->at(i).curves.enabled){
			projectors->at(i).curves.setLabelPosition(16, 45);
			pw = 256;
			ph = 256;

			px = projectors->at(i).getPlanePosition().x + projectors->at(i).width/2 - pw/2;
			py = projectors->at(i).getPlanePosition().y + projectors->at(i).height/2  - ph/2;

			drawBackground();
			drawCurves(i);
			ofSetColor(255);
			drawProjector(i);
			drawFPS(i);
			drawActive(i);
			drawSaved();
		}

		else {

			projectors->at(i).curves.setLabelPosition();

			pw = 200;
			ph = 145;

			px = projectors->at(i).getPlanePosition().x + projectors->at(i).width/2 - pw/2;
			py = projectors->at(i).getPlanePosition().y + projectors->at(i).height/2  - ph/2;

			drawBackground();
			ofSetColor(255);
			drawProjector(i);
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
		else if (input->source == input->COLOR){
			input->source = storedSource;
			input->setup();
		}

}






void Menu::drawSaved(){
    if (saved && !autosave) {
        ofSetHexColor(0xFFFFFF);
        ofDrawBitmapString("SAVED", px+padx+125, py+pady*1.75);
    }
}




void Menu::drawActive(int i){
    if (!saved || autosave) {
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




void Menu::drawCurves(int i){
	projectors->at(i).drawCurves(px, py);
}




void Menu::drawProjector(int i){
    ofDrawBitmapString("Projector #" + ofToString(i+1), px+padx, py+pady * 1.75);
}




void Menu::drawFPS(int i){
    if (!saved || autosave) {
        if (!projectors->at(i).active) {
            ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), px+padx + 125, py+pady * 1.75);
		}
    }
}




void Menu::toggle() {
    active = !active;
    if (active) {
        for (int i=0; i<windows->size(); i++) {
            glfw->setWindow(windows->at(i).glfwWindow);
            ofShowCursor();
        }
    }
    else {
        for (int i=0; i<windows->size(); i++) {
            glfw->setWindow(windows->at(i).glfwWindow);
            ofHideCursor();
        }
    }
}



















/******************************************

 NAVIGATION

 ********************************************/

void Menu::select() {

    int item = (*currentMenu)->currentItem;
    switch ((*currentMenu)->menuId) {
        case MAIN:
            switch (item) {
                case 1: currentMenu = &menuInput; break;
                case 2: currentMenu = &menuWarp; break;
                case 3: currentMenu = &menuBlend; break;
                case 4: currentMenu = &menuColor; break;
                case 5: currentMenu = &menuSetup; break;
                default: break;
            }
            break;
        case INPUT:
            switch (item) {
                case 3: currentMenu = &menuInputTransform; break;
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
                case 2: currentMenu = &menuLevels; break;
                case 3: currentMenu = &menuBrush; break;
                default: break;
            }
            break;
        case COLOR:
            switch (item) {
                case 0: currentMenu = &menuHSL; break;
                case 1: currentMenu = &menuGamma; break;
				case 2: currentMenu = &menuCurves; break;
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


    switch ((*currentMenu)->menuId) {
		case CURVES_GREY:
			storedSource = input->source;
			input->source = input->COLOR;
			input->setup();
			input->setColor(0, 0, 0);
			
            for (int k=0; k<projCount; k++) {
                projectors->at(k).curves.enabled = true;
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.GREY );
				projectors->at(k).curves.setCurrentHover(0);
            }

            break;
		case CURVES_RED:
			storedSource = input->source;
			input->source = input->COLOR;
			input->setup();
			input->setColor(0, 0, 0);

            for (int k=0; k<projCount; k++) {
                projectors->at(k).curves.enabled = true;
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.RED );
				projectors->at(k).curves.setCurrentHover(0);
            }

            break;
		case CURVES_GREEN:
			storedSource = input->source;
			input->source = input->COLOR;
			input->setup();
 			input->setColor(0, 0, 0);
          
			for (int k=0; k<projCount; k++) {
                projectors->at(k).curves.enabled = true;
				projectors->at(k).curves.setColorMode( projectors->at(k).curves.GREEN );
				projectors->at(k).curves.setCurrentHover(0);
            }

            break;
		case CURVES_BLUE:
			storedSource = input->source;
			input->source = input->COLOR;
			input->setup();
 			input->setColor(0, 0, 0);
           
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
            toggle();
            break;
        // space = play/pause video
        case 32:
            if (input->isVideo) {
				if (input->isPlaying())
					input->stop();
				else
					input->play();
            }
            break;

		// ctrl + o = open media file dialog
        case 111:
            if (ofGetKeyPressed(cKey))
				input->openFileDialog();
            break;
    }


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
    model->value = value;


    for (int k=0; k<projCount; k++) {
		projectors->at(k).keyPressed(key);
    }

    // MENU
    ///////////////////////////
    if (!active) { return; }
    ///////////////////////////


	// NAVIGATION
    switch (key){
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

    if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {
		
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

		else{
			switch((*currentMenu)->menuId) {
				case INPUT:
					switch ((*currentMenu)->currentItem) {
						default:
							input->keyPressed(key);
							break;
					}
					break;
				case INPUT_TRANSFORM:
					switch ((*currentMenu)->currentItem) {
						default:
							model->keyPressed(key);
							break;
					}
					break;
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

    else if (key == 33){
        key = 49;
    }
    else if (key == 64){
        key = 50;
    }
    else if (key == 35){
        key = 51;
    }
    else if (key == 36){
        key = 52;
    }
    else if (key == 37){
        key = 53;
    }
    else if (key == 94){
        key = 54;
    }
    else if (key == 38){
        key = 55;
    }
    else if (key == 42){
        key = 56;
    }
    else if (key == 40){
        key = 57;
    }
    else if (key == 41){
        key = 48;
    }

    if (key >= 48 && key <= 57)  {

        // map key to projector
        if (key == 48) pActive = 10;
        else pActive = key-49;

        if (pActive < projCount) {

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

           if (projectors->at(pActive).active) {
                glfwSetCursorPos(glfw->getWindow(), xmouse, ymouse);
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

    model->editMode = model->NONE;
    input->editMode = input->NONE;

    int j = (*currentMenu)->currentItem;

    switch ((*currentMenu)->menuId) {

        case MAIN:
            switch (j) {
                case ENABLE:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).ENABLE;
                    }
                    model->editMode = model->NONE;
                    break;
            }
            break;


        case INPUT:
            switch (j) {
                case SOURCE:
                    input->editMode = input->SOURCE;
                    break;
                case FORMAT:
                    input->editMode = input->FORMAT;
                    break;
                case LOOP:
                    input->editMode = input->LOOP;
                    break;
            }
            break;

        case INPUT_TRANSFORM:
            switch (j) {
                case INPUT_FLIP:
                    model->editMode = model->T_FLIP;
                    break;
                case INPUT_ROTATE:
                    model->editMode = model->T_ROTATE;
                    break;
                case INPUT_TILT:
                    model->editMode = model->T_TILT;
                    break;
                case INPUT_SCALE:
                    model->editMode = model->T_SCALE;
                    break;
            }
            break;

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

        case LEVELS:
            switch (j) {
                case BLACK:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).BLACK;
                    }
                    break;
                case WHITE:
                    for (int k=0; k<projCount; k++) {
                        if (projectors->at(k).active)
                            projectors->at(k).editMode = projectors->at(k).WHITE;
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

        case SCALE:
            switch (j) {
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
