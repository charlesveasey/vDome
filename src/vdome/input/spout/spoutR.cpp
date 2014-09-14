#include "spoutR.h"

void SpoutR::setup(){

	bInitialized			= false;				// Spout receiver initialization
	bDoneOnce				= false;				// do-once initialization flag
	g_Width					= 2048;					// set global width and height to something
	g_Height				= 2048;					// they need to be reset when the receiver connects to a sender
	SenderName[0]			= NULL;					// the name will be filled when the receiver connects to a sender
	
	texture.allocate(g_Width, g_Height, GL_RGBA);	// Allocate a texture for shared texture transfers

}

void SpoutR::bind() {

	char str[256];
	ofSetColor(255);
	unsigned int width, height;
	char tempname[256]; // temp name
	
	// A render window must be available for Spout initialization
	// and might not be available in "update", so do it now 
	// when there is definitely a render window.
	if(!bDoneOnce) { 

		// INITIALIZE A RECEIVER
		// Optionally pass a sender name to try to find and connect to 
		// strcpy_s(tempname, 256, "Required sender name"); 
		width  = g_Width; // pass the initial width and height (they will be adjusted if necessary)
		height = g_Height;

		// Initialization will fail if there are no senders running
		// but can be called repeatedly until a sender is started
		bInitialized = InitReceiver(tempname, width, height, bTextureShare);

		if(bInitialized) {

			// Check to see whether it has initialized texture share or memoryshare
			if(bTextureShare) { 
				// Texture share is OK so we can look at sender names
				// Check if the name returned is different.
				if(strcmp(SenderName, tempname) != 0) {
					// If the sender name is different, the requested 
					// sender was not found so the active sender was used.
					// Act on this if necessary.
					strcpy_s(SenderName, 256, tempname);
				}
			}
			// else the receiver has initialized in memoryshare mode

			// Is the size of the detected sender different from the current texture size ?
			// This is detected for both texture share and memoryshare
			if(width != g_Width || height != g_Height) {
				// Reset the local receiving texture size
				g_Width = width;
				g_Height = height;
				// Adjust the receiving texture
				texture.allocate(g_Width, g_Height, GL_RGBA);
				// reset render window
				//ofSetWindowShape(g_Width, g_Height);
			}
			bDoneOnce = true; // Set the do-once flag
		} // Receiver did not initialize - no sender presenet - keep trying
		else {
			sprintf_s(str, "No sender detected");
			ofDrawBitmapString(str, 20, 20);
		}
		return; // quit for the next round
	}

	// Initialized so OK to draw
	if(bInitialized) {
		
		// Save current global width and height - they will be changed
		// by receivetexture if the sender changes dimensions
		width  = g_Width;
		height = g_Height;
		
		// Try to receive the texture at the current size 
		//
		// NOTE : if the host calls ReceiveTexture with an FBO bound
		// that binding must be restored after the call because Spout makes use of its
		// own FBO for intermediate rendering
		if(!ReceiveTexture(SenderName, texture.getTextureData().textureID, texture.getTextureData().textureTarget, width, height)) {
			
			//
			// Receiver failure :
			//	1)	width and height are zero for read failure.
			//	2)	width and height are changed for sender change
			//		The local texture then has to be resized.
			//
			if(width == 0 || height == 0) {
				// width and height are returned zero if there has been 
				// a texture read failure which might happen if the sender
				// is closed. Spout will keep trying and if the same sender opens again
				// will use it. Otherwise the user can select another sender.
				return;
			}

			if(width != g_Width || height != g_Height ) {
				// The sender dimensions have changed
				// Update the global width and height
				g_Width  = width;
				g_Height = height;
				// Update the local texture to receive the new dimensions
				texture.allocate(g_Width, g_Height, GL_RGBA);
				return; // quit for next round
			}

		}
		else {

			// draw the texture and fill the screen
			texture.bind();
			
			// Show what it is receiving
			if(bTextureShare) {
				sprintf_s(str, "Receiving from : [%s]", SenderName);
				ofDrawBitmapString(str, 20, 20);
				sprintf_s(str, "RH click select sender", SenderName);
				ofDrawBitmapString(str, 15, ofGetHeight()-20);
			}
			else {
				sprintf_s(str, "Memoryshare receiver");
				ofDrawBitmapString(str, 20, 20);
			}
		}
	}

}

void SpoutR::unbind() {
	texture.unbind();
}

void SpoutR::draw() {

	char str[256];
	ofSetColor(255);
	unsigned int width, height;
	char tempname[256]; // temp name
	
	// A render window must be available for Spout initialization
	// and might not be available in "update", so do it now 
	// when there is definitely a render window.
	if(!bDoneOnce) { 

		// INITIALIZE A RECEIVER
		// Optionally pass a sender name to try to find and connect to 
		// strcpy_s(tempname, 256, "Required sender name"); 
		width  = g_Width; // pass the initial width and height (they will be adjusted if necessary)
		height = g_Height;

		// Initialization will fail if there are no senders running
		// but can be called repeatedly until a sender is started
		bInitialized = InitReceiver(tempname, width, height, bTextureShare);

		if(bInitialized) {

			// Check to see whether it has initialized texture share or memoryshare
			if(bTextureShare) { 
				// Texture share is OK so we can look at sender names
				// Check if the name returned is different.
				if(strcmp(SenderName, tempname) != 0) {
					// If the sender name is different, the requested 
					// sender was not found so the active sender was used.
					// Act on this if necessary.
					strcpy_s(SenderName, 256, tempname);
				}
			}
			// else the receiver has initialized in memoryshare mode

			// Is the size of the detected sender different from the current texture size ?
			// This is detected for both texture share and memoryshare
			if(width != g_Width || height != g_Height) {
				// Reset the local receiving texture size
				g_Width = width;
				g_Height = height;
				// Adjust the receiving texture
				texture.allocate(g_Width, g_Height, GL_RGBA);
				// reset render window
				ofSetWindowShape(g_Width, g_Height);
			}
			bDoneOnce = true; // Set the do-once flag
		} // Receiver did not initialize - no sender presenet - keep trying
		else {
			sprintf_s(str, "No sender detected");
			ofDrawBitmapString(str, 20, 20);
		}
		return; // quit for the next round
	}

	// Initialized so OK to draw
	if(bInitialized) {
		
		// Save current global width and height - they will be changed
		// by receivetexture if the sender changes dimensions
		width  = g_Width;
		height = g_Height;
		
		// Try to receive the texture at the current size 
		//
		// NOTE : if the host calls ReceiveTexture with an FBO bound
		// that binding must be restored after the call because Spout makes use of its
		// own FBO for intermediate rendering
		if(!ReceiveTexture(SenderName, texture.getTextureData().textureID, texture.getTextureData().textureTarget, width, height)) {
			
			//
			// Receiver failure :
			//	1)	width and height are zero for read failure.
			//	2)	width and height are changed for sender change
			//		The local texture then has to be resized.
			//
			if(width == 0 || height == 0) {
				// width and height are returned zero if there has been 
				// a texture read failure which might happen if the sender
				// is closed. Spout will keep trying and if the same sender opens again
				// will use it. Otherwise the user can select another sender.
				return;
			}

			if(width != g_Width || height != g_Height ) {
				// The sender dimensions have changed
				// Update the global width and height
				g_Width  = width;
				g_Height = height;
				// Update the local texture to receive the new dimensions
				texture.allocate(g_Width, g_Height, GL_RGBA);
				return; // quit for next round
			}

		}
		else {

			// draw the texture and fill the screen
			texture.draw(0, 0, ofGetWidth(), ofGetHeight()); 
			
			// Show what it is receiving
			if(bTextureShare) {
				sprintf_s(str, "Receiving from : [%s]", SenderName);
				ofDrawBitmapString(str, 20, 20);
				sprintf_s(str, "RH click select sender", SenderName);
				ofDrawBitmapString(str, 15, ofGetHeight()-20);
			}
			else {
				sprintf_s(str, "Memoryshare receiver");
				ofDrawBitmapString(str, 20, 20);
			}
		}
	}
	
	// Show fps
	sprintf_s(str, "fps: %3.3d", (int)ofGetFrameRate());
	ofSetColor(255); 
	ofDrawBitmapString(str, ofGetWidth()-120, 20);

}

void SpoutR::exit() { 

	ReleaseReceiver(); // can be used for repeated Init and Release of a receiver within the program
	CloseSpout(); // Program termination so clean up everything

}

void SpoutR::mousePressed(int x, int y, int button){

	if(button == 2) { // rh button
		SelectSenderDialog(); // The Spout.dll sender selection dialog OK for OF
		// SelectSenderPanel(); // Alternative using SpoutPanel.exe (must be in executable path)
	}

}




