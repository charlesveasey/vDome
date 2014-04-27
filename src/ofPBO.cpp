/*
 * ofPBO.cpp
 *
 *  Created on: 08/04/2012
 *      Author: arturo
 */

#include "ofPBO.h"


ofPBO::ofPBO() {
	// TODO Auto-generated constructor stub

}

ofPBO::~ofPBO() {
	if(!pboIds.empty()){
		glDeleteBuffersARB(pboIds.size(), &pboIds[0]);
	}
}


void ofPBO::allocate(ofTexture & tex, int numPBOs){
	pboIds.resize(numPBOs);
    glGenBuffersARB(numPBOs, &pboIds[0]);
    int numChannels=1;
    switch(tex.getTextureData().glTypeInternal){
    case GL_LUMINANCE:
    	numChannels = 1;
    	break;
    case GL_RGB:
    	numChannels = 3;
    	break;
    case GL_RGBA:
    	numChannels = 4;
    	break;
    }
    dataSize = tex.getWidth()*tex.getHeight()*numChannels;
    for(int i=0;i<(int)pboIds.size();i++){
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[i]);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
    }
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

    texture = tex;
}

void ofPBO::loadData(const ofPixels & pixels){

	if(pboIds.empty()){
		ofLogError() << "pbo not allocated";
		return;
	}
    index = (index + 1) % pboIds.size();
    int nextIndex = (index + 1) % pboIds.size();

	// bind PBO to update pixel values
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[nextIndex]);

	// map the buffer object into client's memory
	// Note that glMapBufferARB() causes sync issue.
	// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
	// for GPU to finish its job. To avoid waiting (stall), you can call
	// first glBufferDataARB() with NULL pointer before glMapBufferARB().
	// If you do that, the previous data in PBO will be discarded and
	// glMapBufferARB() returns a new allocated pointer immediately
	// even if GPU is still working with the previous data.
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
	GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	if(ptr)
	{
		// update data directly on the mapped buffer
		memcpy(ptr,pixels.getPixels(),dataSize);
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
	}

    // it is good idea to release PBOs with ID 0 after use.
    // Once bound with 0, all pixel operations behave normal ways.
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

}

void ofPBO::updateTexture(){

	// bind the texture and PBO
	texture.bind();
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[index]);
	glTexSubImage2D(texture.getTextureData().textureTarget, 0, 0, 0, texture.getWidth(), texture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, 0);
    texture.unbind();
    // it is good idea to release PBOs with ID 0 after use.
    // Once bound with 0, all pixel operations behave normal ways.
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}
