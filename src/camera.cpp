/*
 *  camera.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 10/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "camera.h"
#include "ofLog.h"

//----------------------------------------
camera::camera() :
isOrtho(false),
fov(60),
nearClip(0),
farClip(0),
lensOffset(0.0f, 0.0f),
forceAspectRatio(false),
isActive(false),
vFlip(false)
{
}

//----------------------------------------
void camera::setFov(float f) {
	fov = f;
}

//----------------------------------------
void camera::setNearClip(float f) {
	nearClip = f;
}

//----------------------------------------
void camera::setFarClip(float f) {
	farClip = f;
}

//----------------------------------------
void camera::setLensOffset(const ofVec2f & lensOffset){
	this->lensOffset = lensOffset;
}

//----------------------------------------
void camera::setAspectRatio(float aspectRatio){
	this->aspectRatio = aspectRatio;
	setForceAspectRatio(true);
}

//----------------------------------------
void camera::setForceAspectRatio(bool forceAspectRatio){
	this->forceAspectRatio = forceAspectRatio;
}

//----------------------------------------
void camera::setupPerspective(bool _vFlip, float fov, float nearDist, float farDist, const ofVec2f & lensOffset){
	ofRectangle orientedViewport = ofGetNativeViewport();
	float eyeX = orientedViewport.width / 2;
	float eyeY = orientedViewport.height / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	setFov(fov);
	setNearClip(nearDist);
	setFarClip(farDist);
	setLensOffset(lensOffset);
	setForceAspectRatio(false);

	setPosition(eyeX,eyeY,dist);
	lookAt(ofVec3f(eyeX,eyeY,0),ofVec3f(0,1,0));
	vFlip = _vFlip;
}

//----------------------------------------
void camera::setupOffAxisViewPortal(const ofVec3f & topLeft, const ofVec3f & bottomLeft, const ofVec3f & bottomRight){
	ofVec3f bottomEdge = bottomRight - bottomLeft; // plane x axis
	ofVec3f leftEdge = topLeft - bottomLeft; // plane y axis
	ofVec3f bottomEdgeNorm = bottomEdge.normalized();
	ofVec3f leftEdgeNorm = leftEdge.normalized();
	ofVec3f bottomLeftToCam = this->getPosition() - bottomLeft;
	
	ofVec3f cameraLookVector = leftEdgeNorm.getCrossed(bottomEdgeNorm);
	
	ofVec3f cameraUpVector = bottomEdgeNorm.getCrossed(cameraLookVector);
	
	lookAt(cameraLookVector + this->getPosition(), cameraUpVector);

	//lensoffset
	ofVec2f lensOffset;
	lensOffset.x = -bottomLeftToCam.dot(bottomEdgeNorm) * 2.0f / bottomEdge.length() + 1.0f;
	lensOffset.y = -bottomLeftToCam.dot(leftEdgeNorm) * 2.0f / leftEdge.length() + 1.0f;
	setLensOffset(lensOffset);
	setAspectRatio( bottomEdge.length() / leftEdge.length() );
	float distanceAlongOpticalAxis = abs(bottomLeftToCam.dot(cameraLookVector));
	setFov(2.0f * RAD_TO_DEG * atan( (leftEdge.length() / 2.0f) / distanceAlongOpticalAxis));
}


//----------------------------------------
void camera::setVFlip(bool vflip){
	vFlip = vflip;
}

//----------------------------------------
bool camera::isVFlipped(){
	return vFlip;
}

//----------------------------------------
void camera::enableOrtho() {
	isOrtho = true;
}

//----------------------------------------
void camera::disableOrtho() {
	isOrtho = false;
}

//----------------------------------------
bool camera::getOrtho() const {
	return isOrtho;
}

//----------------------------------------
float camera::getImagePlaneDistance(ofRectangle viewport) const {
	return viewport.height / (2.0f * tanf(PI * fov / 360.0f));
}

ofMatrix4x4 projectionMatrix;

//----------------------------------------
void camera::begin(ofRectangle viewport) {
	if(!isActive) ofPushView();
	isActive = true;

	// autocalculate near/far clip planes if not set by user
	calcClipPlanes(viewport);

	ofViewport(viewport.x,viewport.y,viewport.width,viewport.height);
	ofSetOrientation(ofGetOrientation(),vFlip);

	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadMatrix( projectionMatrix );

	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadMatrix( getModelViewMatrix() );
}

// if begin(); pushes first, then we need an end to pop
//----------------------------------------
void camera::end() {
	if (isActive)
	{
		ofPopView();
		isActive = false;
	}
}

//----------------------------------------
ofMatrix4x4 camera::getProjectionMatrix(ofRectangle viewport) const {
	if(isOrtho) {
		return ofMatrix4x4::newOrthoMatrix(0, viewport.width, 0, viewport.height, nearClip, farClip);
	}else{
		float aspect = forceAspectRatio ? aspectRatio : viewport.width/viewport.height;
		ofMatrix4x4 matProjection;
		matProjection.makePerspectiveMatrix(fov, aspect, nearClip, farClip);
		matProjection.translate(-lensOffset.x, -lensOffset.y, 0);
		return matProjection;
	}
}

void camera::setProjectionMatrix(ofMatrix4x4 matrix) {
    projectionMatrix = matrix;
}


//----------------------------------------
ofMatrix4x4 camera::getModelViewMatrix() const {
	return ofMatrix4x4::getInverseOf(getGlobalTransformMatrix());
}

//----------------------------------------
ofMatrix4x4 camera::getModelViewProjectionMatrix(ofRectangle viewport) const {
	return getModelViewMatrix() * getProjectionMatrix(viewport);
}

//----------------------------------------
ofVec3f camera::worldToScreen(ofVec3f WorldXYZ, ofRectangle viewport) const {

	ofVec3f CameraXYZ = WorldXYZ * getModelViewProjectionMatrix(viewport);
	ofVec3f ScreenXYZ;

	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;

	ScreenXYZ.z = CameraXYZ.z;

	return ScreenXYZ;

}

//----------------------------------------
ofVec3f camera::screenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport) const {

	//convert from screen to camera
	ofVec3f CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f *(ScreenXYZ.y - viewport.y) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;

	//get inverse camera matrix
	ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(getModelViewProjectionMatrix(viewport));

	//convert camera to world
	return CameraXYZ * inverseCamera;

}

//----------------------------------------
ofVec3f camera::worldToCamera(ofVec3f WorldXYZ, ofRectangle viewport) const {
	return WorldXYZ * getModelViewProjectionMatrix(viewport);
}

//----------------------------------------
ofVec3f camera::cameraToWorld(ofVec3f CameraXYZ, ofRectangle viewport) const {

	ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(getModelViewProjectionMatrix(viewport));

	return CameraXYZ * inverseCamera;
}

//----------------------------------------
void camera::calcClipPlanes(ofRectangle viewport) {
	// autocalculate near/far clip planes if not set by user
	if(nearClip == 0 || farClip == 0) {
		float dist = getImagePlaneDistance(viewport);
		nearClip = (nearClip == 0) ? dist / 100.0f : nearClip;
		farClip = (farClip == 0) ? dist * 10.0f : farClip;
	}
}
