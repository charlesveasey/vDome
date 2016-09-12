#include "spoutR.h"

/******************************************
 
 SETUP
 
 ********************************************/

void SpoutR::setup(){
	spoutreceiver = new SpoutReceiver; // Create a Spout receiver object
	bInitialized = false;             // Spout receiver initialization
	SenderName[0] = 0;                 // the name will be filled when the receiver connects to a sender

									   // Allocate a texture for shared texture transfers
									   // An openFrameWorks texture is used so that it can be drawn.
	g_Width = ofGetWidth();
	g_Height = ofGetHeight();

	if (texture.getWidth() != g_Width && texture.getHeight() != g_Height)
		texture.allocate(g_Width, g_Height, GL_RGBA);	// Allocate a texture for shared texture transfers
}

/******************************************
 
 UPDATE
 
 ********************************************/

void SpoutR::update() {
	char str[256];
	ofSetColor(255);
	unsigned int width, height;
	char tempname[256]; // temp name

						// A render window must be available for Spout initialization
						// and might not be available in "update", so do it now
						// when there is definitely a render window.
						//
						// INITIALIZE A RECEIVER
						//
						// The receiver will attempt to connect to the name it is sent.
						// Alternatively set the optional bUseActive flag to attempt to connect to the active sender.
						// If the sender name is not initialized it will attempt to find the active sender
						// If the receiver does not find any senders the initialization will fail
						// and "CreateReceiver" can be called repeatedly until a sender is found.
						// "CreateReceiver" will update the passed name, and dimensions.
	if (!bInitialized) {
		// Create the receiver and specify true to attempt to connect to the active sender
		if (spoutreceiver->CreateReceiver(SenderName, width, height, true)) {

			// Is the size of the detected sender different ?
			if (width != g_Width || height != g_Height) {
				// The sender dimensions have changed so update the global width and height
				g_Width = width;
				g_Height = height;
				// Update the local texture to receive the new dimensions
				texture.allocate(g_Width, g_Height, GL_RGBA);
			}
			bInitialized = true;
			return; // quit for next round
		} // receiver was initialized
		else {
			sprintf(str, "No sender detected");
			ofDrawBitmapString(str, 20, 20);
		}
	} // end initialization

	  // The receiver has initialized so OK to draw
	if (bInitialized) {

		// Save current global width and height - they will be changed
		// by ReceiveTexture if the sender changes dimensions
		width = g_Width;
		height = g_Height;

		// Try to receive into the local the texture at the current size
		// NOTE: If a host calls ReceiveTexture with a framebuffer object bound,
		// include the FBO id in the ReceiveTexture call so that the binding is restored
		// afterwards because Spout makes use of its own FBO for intermediate rendering.
		if (spoutreceiver->ReceiveTexture(SenderName, width, height, texture.getTextureData().textureID, texture.getTextureData().textureTarget)) {

			//	If the width and height are changed, the local texture has to be resized.
			if (width != g_Width || height != g_Height) {
				// Update the global width and height
				g_Width = width;
				g_Height = height;
				// Update the local texture to receive the new dimensions
				texture.allocate(g_Width, g_Height, GL_RGBA);
				return; // quit for next round
			}

		}
		else {
			// A texture read failure might happen if the sender
			// is closed. Release the receiver and start again.
			spoutreceiver->ReleaseReceiver();
			bInitialized = false;
			return;
		}
	}
}

/******************************************
 
 BIND
 
 ********************************************/

void SpoutR::bind() {
	texture.bind();
}

void SpoutR::unbind() {
	texture.unbind();
}

/******************************************
 
 EXIT
 
 ********************************************/

void SpoutR::exit() { 
	spoutreceiver->ReleaseReceiver(); // Release the receiver
}