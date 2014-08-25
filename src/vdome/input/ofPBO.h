/*
 * ofPBO.h
 *
 *  Created on: 08/04/2012
 *      Author: arturo
 */

#pragma once
#include "ofConstants.h"
#include "ofTexture.h"
#include "ofThread.h"
#include "Poco/Condition.h"
namespace vd{

class ofPBO: public ofThread {
public:
	ofPBO();
	virtual ~ofPBO();

	void allocate(int numPBOs, int w, int h);
	void loadData(const ofPixels & pixels);
	void updateTexture();
	void threadedFunction();
    void update();
    void wait();
    
	Poco::Condition condition;
    ofTexture texture;

private:
	vector<GLuint> pboIds;
	size_t indexUploading, indexToUpdate;
	unsigned int dataSize;
	GLubyte* gpu_ptr;
	const unsigned char* cpu_ptr;
	bool pboUpdated;
	bool lastDataUploaded;
    GLsync Fence;
};
}