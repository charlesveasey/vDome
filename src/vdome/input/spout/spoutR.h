#pragma once
#include "ofMain.h"
#include "Spout.h"

using namespace Spout;

class SpoutR {

	public:
		void setup();
		void draw();
		void bind();
		void unbind();
		
		void mousePressed(int x, int y, int button);
		void exit(); 

		bool bInitialized;				// Initialization result
		bool bDoneOnce;					// Do-once intialization flag
		bool bTextureShare;				// Texture share compatibility

		ofTexture texture;				// Texture used for texture share transfers
		unsigned int g_Width, g_Height;	// Global width and height
		char SenderName[256];			// Shared memory name

};
