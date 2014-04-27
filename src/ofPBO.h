/*
 * ofPBO.h
 *
 *  Created on: 08/04/2012
 *      Author: arturo
 */

#pragma once
#include "ofConstants.h"
#include "ofTexture.h"

class ofPBO {
public:
	ofPBO();
	virtual ~ofPBO();

	void allocate(ofTexture & tex, int numPBOs);
	void loadData(const ofPixels & pixels);
	void updateTexture();

private:
	ofTexture texture;
	vector<GLuint> pboIds;
	size_t index;
	unsigned int dataSize;
};

