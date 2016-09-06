#pragma once
#include "ofMain.h"
#include ".\SpoutSDK\Spout.h" // Spout SDK

class SpoutR {

	public:
		void setup();
		void update();
		void bind();
		void unbind();		
		void exit(); 

		SpoutReceiver *spoutreceiver; // A Spout receiver object
		bool bInitialized;		      // Initialization result
		ofTexture texture;			  // Texture used for texture share transfers
		char SenderName[256];	      // Sender name used by a receiver
		int g_Width, g_Height;        // Used for checking sender size change
};
