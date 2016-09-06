#pragma once

#include "ofMain.h"
#include "ofxCurvesTool.h"
namespace vd {

class Curves {
public:
	Curves();
	void    init(int i);
	void    setup();
	void    update();
	void    draw(int x, int y);
	void    keyPressed(int key);

	void    incrementPoint(float inc);
	void    setLabelPosition();
	void    setLabelPosition(int x, int y);

	void    save(int projectorStartingIndex);
	void    load(int projectorStartingIndex);
	
	bool    show;
	bool    enabled;
	
	int     getCurrentHover();
	void    setCurrentHover(int i);

	void    setPoint(int i, ofPoint p);

	void    nextPoint();
	void    prevPoint();

	void    setActive(bool);

	int     getColorMode();
	void    setColorMode(int i);

	void	setCurrentPointY(int i, int y);

	ofTexture& colorlutTextureRef();
	
	enum colors {GREY,RED,GREEN,BLUE}; 
	ofImage colorlut;

private:
	bool    active;
	int     curveCnt;
	int     lutRes;
	int     index;
	int     colorMode;

	vector<ofxCurvesTool*>  curvesTools;
	ofXml                   *xml;
};

}