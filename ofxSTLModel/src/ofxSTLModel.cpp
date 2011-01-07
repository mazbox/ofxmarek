// adapted from the STL class by Marius Watz (part of unlekkerlib from processing)
// taken from http://workshop.evolutionzone.com/unlekkerlib/ on 06/12/09
// converted to C++ by Marek Bereza


#include "ofxSTLModel.h"

	
	
ofxSTLModel::ofxSTLModel() {
	mult = 1;
}

/**
 * Draws the object.
 */
void ofxSTLModel::draw() {
	glBegin(GL_TRIANGLES);
	for(int i=0; i<triangles.size(); i++) {
		glNormal3f(triangles[i].v[0], triangles[i].v[1], triangles[i].v[2]);
		glVertex3f(triangles[i].v[3], triangles[i].v[4], triangles[i].v[5]);
		glVertex3f(triangles[i].v[6], triangles[i].v[7], triangles[i].v[8]);
		glVertex3f(triangles[i].v[9], triangles[i].v[10], triangles[i].v[11]);
	}
	glEnd();
}

/**
 * Calculates the bounding box of the object. 
 */
	
void ofxSTLModel::calcBounds() {
	minx=10000;
	maxx=-10000;
	miny=10000;
	maxy=-10000;
	minz=10000;
	maxz=-10000;
	
	int id=0;
	for(int i=0; i<triangles.size(); i++) {
		id=3;
		for(int j=0; j<3; j++) {
			if(triangles[i].v[id]<minx) minx=triangles[i].v[id];
			else if(triangles[i].v[id]>maxx) maxx=triangles[i].v[id];
			id++;
			
			if(triangles[i].v[id]<miny) miny=triangles[i].v[id];
			else if(triangles[i].v[id]>maxy) maxy=triangles[i].v[id];
			id++;
			
			if(triangles[i].v[id]<minz) minz=triangles[i].v[id];
			else if(triangles[i].v[id]>maxz) maxz=triangles[i].v[id];
			id++;
			
		}
	}
}
	
/**
 * Centers the object around the world origin. 
 */

void ofxSTLModel::center() {
	calcBounds();
	float tx=(minx+maxx)/2;
	float ty=(miny+maxy)/2;
	float tz=(minz+maxz)/2;
	for(int i=0; i<triangles.size(); i++) triangles[i].translate(-tx,-ty,-tz);
}

	
/**
 * Normalizes the object to a absolute scale 
 */

void ofxSTLModel::normalize(float m) {
	calcBounds();
	
	float max=maxx-minx;
	if(maxy-miny>max) max=maxy-miny;
	if(maxz-minz>max) max=maxz-minz;
	
	float tx=m/max;
	float ty=m/max;
	float tz=m/max;
	for(int i=0; i<triangles.size(); i++) triangles[i].scale(tx,ty,tz);
}
	
	
	/////////////////////////////////////////////
	// FUNCTIONS FOR STL INPUT
	
void ofxSTLModel::read(string path) {
	
	char header[80];
	char buf[50]; // 12 32bit floats + 2 bytes = 50 bytes per triangle
	int num = 0;
	
	ifstream file(ofToDataPath(path).c_str(), ios::in|ios::binary);
	
	printf("Reading %s\n", path.c_str());
	
	file.read(header, 80);
	file.read((char*)&num, 4);
	
	printf("%d facets\n", num);
	triangles.clear();
	
	for(int i=0; i<num; i++) {
		file.read(buf, 50);
		triangles.push_back(STLFace());
		triangles[i].parseFace(buf);
		//if(i%1000==0) printf("%d triangles read.", i);
	}
	file.close();
}

	
	
	
/////////////////////////////////////////////
// FUNCTIONS FOR RAW STL OUTPUT



void ofxSTLModel::write(string path) {
	int num = triangles.size();
	ofstream file(ofToDataPath(path).c_str(), ofstream::binary);
	
	char header[80];
	
	memset(header, 0, 80);
	file.write(header, 80);
	file.write((char*)&num, 4);
	char buf[50];
	memset(header, 0, 50);
	for(int i = 0; i < triangles.size(); i++) {
		memcpy(buf, triangles[i].getData(), 48);
		file.write(buf, 50);
	}
	
	file.close();
}

	
		
	
void ofxSTLModel::addTriangle(float nX, float nY, float nZ,
				 float x1, float y1, float z1,
				 float x2, float y2, float z2,
				 float x3, float y3, float z3
				 ) {
	
	triangles.push_back(STLFace());
	float data[12];
	
	data[0] = nX;
	data[1] = nY;
	data[2] = nZ;
	
	data[3] = x1;
	data[4] = y1;
	data[5] = z1;
	
	data[6] = x2;
	data[7] = y2;
	data[8] = z2;
	
	data[9] = x3;
	data[10] = y3;
	data[11] = z3;
	
	
	triangles[triangles.size()-1].parseFace((char*)data);
	
}

