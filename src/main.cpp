#include "ofMain.h"
#include "vdome.h"
#include "ofGlProgrammableRenderer.h"
//========================================================================
int main( ){
	
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	
	//ofSetupOpenGL(2048,768, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(6144,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new vdome());

}
