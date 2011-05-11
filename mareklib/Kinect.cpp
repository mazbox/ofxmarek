/*
 *  Kinect.cpp
 *  KinectExample
 *
 *  Created by Marek Bereza on 24/01/2011.
 *
 */


#include "Kinect.h"


Kinect::Kinect(string name): ofNode() {
	numKinectCoords = 0;
	enabled = true;
	this->name = name;
	running = false;
	kinectCoords = NULL;
	kinectColours = NULL;
	nearClip = 0;
	farClip = 255;
	thresholdedPixels = NULL;
	flipX = false;
	flipY = false;
	scale = 1;
	zOffset = 0;
	//	blobTracker.setVerbose();
	vertexArraySize = 0;
	planeCoords[0] = ofVec3f(0,0,3);
	planeCoords[1] = ofVec3f(1,0,3);
	planeCoords[2] = ofVec3f(1,1,3);
	
}


void Kinect::addListener(ofxBlobListener *listener) {
	blobTracker.addListener(listener);
}



bool Kinect::open() {

	bool opened = kinect.init();
	if(!opened) {
		ofLog(OF_LOG_ERROR, "Couldn't init() %s\n", name.c_str());
	}
	running = kinect.open();
	if(running) {
		int numPx			= kinect.getWidth()*kinect.getHeight();
		
		thresholdedPixels	= new unsigned char[numPx];
		depths				= new unsigned char[numPx];
		
		thresholded.allocate(kinect.getWidth(), kinect.getHeight());
		numKinectCoords		= kinect.getWidth()*kinect.getHeight()/(KINECT_RESOLUTION*KINECT_RESOLUTION);
		
		kinectCoords		= new ofVec3f[numKinectCoords];
		kinectColours		= new ofVec3f[numKinectCoords];

	}
	return running;
}

void Kinect::close() {
	if(running) {
		kinect.close();
		running = false;
		delete [] kinectCoords;
		delete [] kinectColours;
		kinectCoords = NULL;
		kinectColours = NULL;
	}
}

void Kinect::update() {
	
	plane.setFrom3Points(planeCoords[0], planeCoords[1], planeCoords[2]);
	
	// do the ofNode stuff
	applyTransformation();
	
	
	// actual processing
	if(running) {
		kinect.update();
		
		hasCreatedVertexArrays = false;
		


		doVision();
		
		
	}
}

void Kinect::doVision() {
	// get depth pixels
	int numPx = kinect.getWidth()*kinect.getHeight();
	memcpy(depths, kinect.getDepthPixels(), numPx);
	
	doThreshold();
	
	// load into a cvImage
	thresholded.setFromPixels(thresholdedPixels, kinect.getWidth(), kinect.getHeight());
	
	// flip as required
	if(flipX || flipY) thresholded.mirror(flipY, flipX);
	
	// find contours
	contourFinder.findContours( thresholded,
							   100, getWidth()*getHeight()/4,
							   5, false);
	

	vector<ofVec3f> blobs;
	for(int i = 0; i < contourFinder.nBlobs; i++) {
		blobs.push_back(
				ofVec3f(find3DBlobCentre(contourFinder.blobs[i]))
		);
		
	}
	
	// this is when the notifictations will fire.
	blobTracker.track(blobs);
}

// this looks for the minimum depth in a blob
// and then calls that the centre (more useful than
// the statistical centre of the blob.
ofVec3f Kinect::find3DBlobCentre(ofxCvBlob &blob) {
	// TODO: this just takes the centre for now, would be nice
	//       to look at the closest point in the blob to the kinect.
	return blob.centroid/ofVec3f(getWidth(), getHeight());
	/*
	ofRectangle rect = blob.boundingRect;

	// now look at every pixel in the blob and find the max/min
	float max = FLT_MIN;
	float min = FLT_MAX;
	
	for(int y = rect.y; y < rect.y+rect.height; y++) {
		int yOffset = y*width;
		for(int x = rect.x; x < rect.x+rect.width; x++) {
			
			float value = depths[x + yOffset];
			if(max<value) max = value;
			if(min>value && value!=0) min = value;
		}
	}
	return 0;*/
}

void Kinect::doThreshold() {
	int numPx = kinect.getWidth()*kinect.getHeight();
	unsigned char minValue = nearClip;
	unsigned char maxValue = farClip;
	if(nearClip>farClip) {
		minValue = farClip;
		maxValue = nearClip;
	} else if(minValue==maxValue) {
		minValue = maxValue - 1;
	}
	for(int i = 0; i < numPx; i++) {
		// clamp and normalize
		if(depths[i]>minValue && depths[i]<maxValue) {
			thresholdedPixels[i] = 255;
		} else {
			thresholdedPixels[i] = 0;
		}
	}
}

void Kinect::createVertexArrays() {
	float *depths = kinect.getDistancePixels();
	unsigned char *colours = kinect.getCalibratedRGBPixels();
	
	int p = 0;
	int w = kinect.getWidth();
	// copy the pixels into the vertex arrays
	float minZ = FLT_MAX;
	float maxZ = FLT_MIN;
	for(int i = 0; i < w; i+=KINECT_RESOLUTION) {
		for(int j = 0; j < kinect.getHeight(); j+=KINECT_RESOLUTION) {
			int index = w*j + i;
			int resIndex = p;
	
			// skip any 0'z
			if(depths[index]>0) {
				p++;
				
				// centimetres to metres, hence *0.01
				kinectCoords[resIndex] = ofVec3f(i*0.01*scale, j*0.01*scale, depths[index]*0.01*scale+zOffset);
				kinectColours[resIndex] = ofVec3f((float)colours[index*3+0]/255.f, (float)colours[index*3+1]/255.f, (float)colours[index*3+2]/255.f);
				if(depths[index]>maxZ) maxZ = depths[index];
				if(depths[index]<minZ) minZ = depths[index];
				
				// do plane thing here.
				float dist = plane.distanceToPoint(kinectCoords[resIndex]);
				if(dist<0) kinectColours[resIndex] = ofVec3f(1, 0, 1);
			}
		}
	}
//	printf("Min %f     Max %f\n", minZ, maxZ);
	vertexArraySize = p;
}

void Kinect::customDraw() {
	
	// this will only generate vertex arrays once per frame!
	if(!hasCreatedVertexArrays && running) {
		createVertexArrays();
		hasCreatedVertexArrays = true;
	}
	ofSetHexColor(0x00FF00);

	ofBox(0, 0, 0, 0.2);
	glBegin(GL_LINES);
	
	
	float h = 2;
	float w = h*getWidth()/getHeight();
	
	w *= 0.4;
	h *= 0.4;
	
	float z = 1;
	glVertex3f(0, 0, 0);
	glVertex3f(-w, -h, z);
	
	glVertex3f(-w, -h, z);
	glVertex3f(-w, h, z);
	
	glVertex3f(-w, h, z);
	glVertex3f(0, 0, 0);
	
	
	
	glVertex3f(0, 0, 0);
	glVertex3f(w, h, z);
	
	glVertex3f(w, h, z);
	glVertex3f(w, -h, z);
	
	glVertex3f(w, -h, z);
	glVertex3f(0, 0, 0);
	
	glVertex3f(w, -h, z);
	glVertex3f(-w, -h, z);
	
	glVertex3f(-w, h, z);
	glVertex3f(w, h, z);
	
	
	
	glEnd();
	
	
	if(kinectCoords==NULL) return;
	
	glPushMatrix();
	glTranslatef(-getWidth()*0.01*scale/2, -getHeight()*0.01*scale/2, 0);
	glColorPointer(3, GL_FLOAT, 0, kinectColours);
	glVertexPointer(3, GL_FLOAT, 0, kinectCoords);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_POINTS, 0, vertexArraySize);
	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableClientState(GL_COLOR_ARRAY);
	
	
	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 3; i++) {
		glVertex3f(planeCoords[i].x, planeCoords[i].y, planeCoords[i].z);
	}
	glEnd();
	
	glPopMatrix();
}


float Kinect::getWidth() {
	return kinect.getWidth();
}

float Kinect::getHeight() {
	return kinect.getHeight();
}
void Kinect::drawCalibration(ofRectangle rect) {
	if(thresholdedPixels!=NULL) {
		depthRect = ofRectangle(rect.x, rect.y+rect.height/2, rect.width/2, rect.height/2);
		kinect.draw(rect.x, rect.y, rect.width/2, rect.height/2);
		kinect.drawDepth(depthRect);
		thresholded.draw(rect.x+rect.width/2, rect.y, rect.width/2, rect.height/2);
		contourFinder.draw(rect.x+rect.width/2, rect.y, rect.width/2, rect.height/2);
		blobTracker.draw(rect.x+rect.width/2, rect.y, rect.width/2, rect.height/2);
	}
}
int planePointIndex = 0;
void Kinect::mouseReleased(float x, float y, int button) {
	if(depthRect.inside(ofPoint(x, y))) {

		// convert the point in the gui to a point in the kinect.
		ofPoint p(x,y);
		p.x -= depthRect.x;
		p.y -= depthRect.y;
		p.x *= getWidth()/depthRect.width;
		p.y *= getHeight()/depthRect.height;
		
		// find the z
		float z = kinect.getDistanceAt(p)*0.01*scale + zOffset;
		
		// this is one of the coordinates that should be 
		ofVec3f planePoint = ofVec3f(p.x*0.01*scale, p.y*0.01*scale, z);
		planeCoords[planePointIndex] = planePoint;
		planePointIndex++;
		if(planePointIndex>=3) planePointIndex = 0;
		printf("Plane points:\n\t%f, %f, %f\n\t%f, %f, %f\n\t%f, %f, %f\n", 
			   planeCoords[0].x,
			   planeCoords[0].y,
			   planeCoords[0].z,
			   planeCoords[1].x,
			   planeCoords[1].y,
			   planeCoords[1].z,
			   planeCoords[2].x,
			   planeCoords[2].y,
			   planeCoords[2].z
		);
	}
}