/**
 * KinectOrCamera.h
 * magic
 *
 * Created by Marek Bereza on 13/04/2011.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofxKinect.h"


class KinectOrCamera: public ofBaseVideoGrabber, public ofBaseDraws, public ofBaseHasTexture {
public:
	KinectOrCamera();
	
	void listDevices();
	bool initGrabber(int w, int h);
	bool isFrameNew();
	void update();
	void close();
	
	float getWidth() { return width; }
	float getHeight() { return height; }
	
	int width;
	int height;
	
	void draw() { draw(0, 0, width, height); }
	void draw(float x, float y) { draw(x, y, width, height); }
	void draw(float x, float y, float width, float height);
	
	ofPixelsRef			getPixelsRef();
	unsigned char *getPixels();
	unsigned char *getDepthPixels();
	void setUseTexture(bool bUseTex) { bUseTexture = bUseTex; };
	ofTexture &getTextureReference() { return tex; }
private:
	ofVideoGrabber camera;
	unsigned char *greyscaleBuffer;
	ofxKinect kinect;
	bool usingKinect;
	bool bUseTexture;
	ofTexture tex;
};