#include "Curves.h"
namespace vd {

extern int cCurveIndex;

Curves::Curves() {
	curveCnt = 4;
	lutRes = 256;
	show = true;
	enabled = false;
	index = 0;
	colorMode = GREY;
	xml = new ofXml;
}

void Curves::init(int i) {
	index =  i;
}

void Curves::setup() {

	for(int i = 0; i < curveCnt; i++) {
		curvesTools.push_back(new ofxCurvesTool);
		curvesTools.at(i)->setup();
		curvesTools.at(i)->mouseAddsPoint = false;
		curvesTools.at(i)->useKey(false);
		curvesTools.at(i)->keepFocus = true;
		curvesTools.at(i)->mouseMovesPoint = false;

		ofAddListener(curvesTools.at(i)->curHoverChange,this, &Curves::onCurHoverChange);
		ofAddListener(curvesTools.at(i)->curHoverUpdate,this, &Curves::onCurHoverUpdate);

		for(int j = 1; j < 8; j++) {
			curvesTools.at(i)->add(ofVec2f(ofClamp(j*lutRes/8, 0, lutRes-1),ofClamp(j*lutRes/8, 0, lutRes-1)));
		}
	}	

	colorlut.allocate(lutRes, lutRes, OF_IMAGE_COLOR);

	for(int x = 0; x < 256; x++) {
		for(int y = 0; y < 256; y++) {
			colorlut.setColor(x, y, ofColor(x,x,x));
		}
	}

	setCurrentHover(0);
	update();

	setActive(false);
}  

void Curves::update() {
	if (!curvesTools.size()) return;

	bool n = curvesTools.at(colorMode)->isLutNew();

	for(int x = 0; x < lutRes; x++) {
		for(int y = 0; y < lutRes; y++) {
			if (n){
				colorlut.setColor(x, y, ofColor(
					ofClamp(curvesTools.at(0)->getLut(x) - x + curvesTools.at(1)->getLut(x), 0, lutRes-1),
					ofClamp(curvesTools.at(0)->getLut(x) - x + curvesTools.at(2)->getLut(x), 0, lutRes-1),
					ofClamp(curvesTools.at(0)->getLut(x) - x + curvesTools.at(3)->getLut(x), 0, lutRes-1)));
			}
		}
	}

	colorlut.update();
}

void Curves::draw(int x, int y) {
	if(show || enabled) {
		curvesTools.at(colorMode)->draw(x,y);
		//colorlut.draw(lutRes, 0);
	}
}

void Curves::keyPressed(int key) {

	if(key == OF_KEY_UP) {
		incrementPoint(1);
	}
	else if(key == OF_KEY_DOWN) {
		incrementPoint(-1);
	}

}

void Curves::incrementPoint(float inc) {
	int cpnt = curvesTools.at(colorMode)->getCurrentHover();
	curvesTools.at(colorMode)->set(cpnt, 
		ofVec2f( curvesTools.at(colorMode)->getPoint(cpnt).x, 
				 curvesTools.at(colorMode)->getPoint(cpnt).y + inc));	
}

void Curves::prevPoint() {
	curvesTools.at(colorMode)->prevPoint();
}

void Curves::nextPoint() {
	curvesTools.at(colorMode)->nextPoint();
}

void Curves::load() {
	xml->clear();
	xml->load("settings/color/color-"+ofToString(index+1)+".xml");	
	
	xml->setTo("projector");

	int n = xml->getNumChildren();

	for(int i = 0; i < n; i++) {
		xml->setToChild(i);
		curvesTools.at(i)->clear();
			
		int nn = xml->getNumChildren();

		for(int j = 0; j < nn; j++) {	
			if (xml->exists("point["+ofToString(j)+"][@xy]")) {
				string str = xml->getAttribute("point["+ofToString(j)+"][@xy]");
				int x = ofToInt(ofSplitString(str, ",")[0]);
				int y = ofToInt(ofSplitString(str, ",")[1]);
				curvesTools.at(i)->add(ofVec2f(x,y));
			}	
		}	
		xml->setToParent();
	}
}

void Curves::save() {
	xml->clear();
	xml->addChild("projector");
    xml->setTo("projector");

	for(int i = 0; i < 4; i++) {
		xml->addChild("curve");
		xml->setToChild(i);

		if (i == GREY)
			xml->setAttribute("color", "grey");
		if (i == RED)
			xml->setAttribute("color", "red");
		if (i == GREEN)
			xml->setAttribute("color", "green");
		if (i == BLUE)
			xml->setAttribute("color", "blue");

		vector<ofVec2f> pnts = curvesTools.at(i)->getControlPoints();

		for(int j = 0; j < pnts.size(); j++) {
			xml->addChild("point");
			xml->setToChild(j);
			xml->setAttribute("xy", ofToString(pnts[j].x) + "," + ofToString(pnts[j].y));
			xml->setToParent();
		}
		xml->setToParent();
	}
	xml->setToParent();

	xml->save("settings/color/color-"+ofToString(index+1)+".xml");
}

int Curves::getCurrentHover() {
	return curvesTools.at(colorMode)->getCurrentHover();
}

void Curves::setCurrentHover(int i) {
	curvesTools.at(colorMode)->setCurrentHover(i);
}

int Curves::getColorMode() {
	return colorMode;
}
void Curves::setColorMode(int i) {
	colorMode = i;
}

ofTexture Curves::colorlutTextureRef() {
	return colorlut.getTextureReference();
}

void Curves::setLabelPosition() {
	curvesTools.at(colorMode)->setLabelPosition();
}
void Curves::setLabelPosition(int x, int y) {
		curvesTools.at(colorMode)->setLabelPosition(x,y);
}

void Curves::setActive(bool b) {
	active = b;
	for(int i = 0; i < curvesTools.size(); i++) {
		curvesTools.at(i)->mouseMovesPoint = active;
	}
}

void Curves::setPoint(int i, ofPoint p){
	curvesTools.at(colorMode)->set(i,p);
}

void Curves::onCurHoverChange(ofVec3f & xyi){
	ofNotifyEvent(curHoverChange, xyi);
}

void Curves::onCurHoverUpdate(ofVec3f & xyi){
	ofVec4f xyip = ofVec4f(xyi[0],xyi[1],xyi[2],index);
	ofNotifyEvent(curHoverUpdate, xyip);
}

}