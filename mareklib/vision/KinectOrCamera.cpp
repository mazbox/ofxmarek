/**
 * KinectOrCamera.cpp
 * magic
 *
 * Created by Marek Bereza on 13/04/2011.
 *
 */

#include "KinectOrCamera.h"
KinectOrCamera::KinectOrCamera() {
	bUseTexture = true;
}

void KinectOrCamera::listDevices() {
	ofLog(OF_LOG_WARNING, "KinectOrCamera::listDevices(): This function is not implemented yet");
}
bool KinectOrCamera::initGrabber(int w, int h) {
	bool success = true;
	if(w!=640 || h!= 480) {
		ofLog(OF_LOG_ERROR, "KinectOrCamera can only be used with 640x480 res. Trying with 640x480");
		success = false;
	}
	
	width = 640;
	height = 480;
	usingKinect = true;
	// try kinect first
	greyscaleBuffer = NULL;

	   
	kinect.init();
	usingKinect = kinect.open();
	if(usingKinect) {
		
	} else {
		// init video grabber
		success = camera.initGrabber(width, height);
		greyscaleBuffer = new unsigned char[width*height];
	}
	
	if(success && bUseTexture) {
		tex.allocate(width, height, GL_RGB);
	}
	return success;
	
}

bool KinectOrCamera::isFrameNew() {
	if(usingKinect) {
		return true;
	} else {
		return camera.isFrameNew();
	}
}

void KinectOrCamera::update() {
	if(usingKinect) {
		kinect.update();
		
	} else {
		camera.grabFrame();
	}
	if(isFrameNew() && bUseTexture) {
		tex.loadData(getPixels(), (int)tex.getWidth(), (int)tex.getHeight(), GL_RGB);
	}
}
void KinectOrCamera::close() {
	if(usingKinect) {
		kinect.close();
	} else {
		camera.close();
		delete [] greyscaleBuffer;
	}
}

ofPixelsRef KinectOrCamera::getPixelsRef() {
	ofPixels pr;
	return pr;
}
unsigned char *KinectOrCamera::getPixels() {
	if(usingKinect) {
		return kinect.getPixels();
	} else {
		return camera.getPixels();
	}
}
unsigned char *KinectOrCamera::getCalibratedPixels() {
	if(usingKinect) {
		return kinect.getCalibratedRGBPixels();
	} else {
		return camera.getPixels();
	}
}

unsigned char *KinectOrCamera::getDepthPixels() {
	if(usingKinect) {
		return kinect.getDepthPixels();
	} else {
		unsigned char *pix = camera.getPixels();
		int numPixels = width*height;
		for(int i = 0; i < numPixels; i++) {
			greyscaleBuffer[i] = pix[i*3];
		}
		return greyscaleBuffer;
	}
}

void KinectOrCamera::draw(float x, float y, float width, float height) {
	if(bUseTexture) {
		tex.draw(x, y, width, height);
	} else {
		ofLog(OF_LOG_ERROR, "Cannot draw kinect if not using textures");
	}
}