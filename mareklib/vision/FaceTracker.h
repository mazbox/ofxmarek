/**
 * FaceTracker.h
 * Wizard
 *
 * This is a threaded face tracker. If it crashes on a thread other than the main
 * thread, in a gl call like glGenTextures() you need to set the associated cv image
 * in ofxCvHaarFinder to have setUseTexture(false) - it's because openGl can only run
 * on the main thread.
 *
 * Created by Marek Bereza on 30/06/2011.
 *
 */

#include "ofMain.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvHaarFinder.h"
#include "ofxCvBlobTracker.h"

class FaceTracker: public ofThread, public ofxCvBlobListener {
public:
	
	
	
	int minFaceSize;
	void setup(int width, int height) {
		
		blobTracker.setMinimumDisplacementThreshold(0);
		blobTracker.setListener(this);
		
		minFaceSize = 20;
		this->width = width;
		this->height = height;
		image.setUseTexture(false);
		image.allocate(width, height);
		finder.setup("haarcascade_frontalface_default.xml");
		finder.setScaleHaar(1.1);

	}
	
	void update(ofxCvGrayscaleImage &grey) {
		
		// see if the old faces is done processing. If so
		// copy the faces locally.
		if(!isThreadRunning()) {
			// copy old faces
			faces.clear();

			
			blobTracker.trackBlobs(finder.blobs);
			
			// go through current blobs, and see if last blobs has the same id
			// if it does, add it to the list, if it doesn't don't add it yet
			map<int,ofRectangle>::iterator it;
			for(it = faceBlobs.begin(); it != faceBlobs.end(); it++) {
				if(lastFaceBlobs.find((*it).first)!=lastFaceBlobs.end()) {
					// add the blob
					faces.push_back((*it).second);
					faces.back().x /= width;
					faces.back().y /= height;
					faces.back().width /= width;
					faces.back().height /= height;
				}
			}
			
			// remember for next time
			lastFaceBlobs = faceBlobs;
			image.scaleIntoMe(grey);

			// restart thread
			startThread(false, false);
		} 

	}
	
	void threadedFunction() {
		
		finder.findHaarObjects(image, minFaceSize);
	}
	
	int getNumFaces() {
		return faces.size();
	}
	ofRectangle &getFace(int i) {
		return faces[i];
	}
	
	
	virtual void blobOn( int x, int y, int id, int order ) {
		faceBlobs[id] = blobTracker.getById(id).boundingRect;
	}
	
    virtual void blobMoved( int x, int y, int id, int order ) {
		faceBlobs[id] = blobTracker.getById(id).boundingRect;
	}
	
    virtual void blobOff( int x, int y, int id, int order ) {
		faceBlobs.erase(id);
	}
	


	
	void draw(float x, float y, float w, float h) {
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef((float)w/width, (float)h/height, 1);
		ofNoFill();
		ofSetHexColor(0xFF0000);
		for(int i = 0; i < faces.size(); i++) {
			ofRect(faces[i]);
		}
		ofFill();
		ofSetHexColor(0xFFFFFF);
		glPopMatrix();
	}
private:
	map<int,ofRectangle> faceBlobs;
	map<int,ofRectangle> lastFaceBlobs;
	vector<ofRectangle> faces;
	float width, height;
	ofxCvGrayscaleImage image;
	ofxCvHaarFinder finder;
	ofxCvBlobTracker blobTracker;
};