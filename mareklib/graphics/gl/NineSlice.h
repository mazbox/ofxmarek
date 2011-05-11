/**
 * NineSlice.h
 * emptyExample
 *
 * Created by Marek Bereza on 05/04/2011.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofVec2f.h"
class NineSlice {
public:
	void setup(ofImage *img);
	
	// draws with previously defined rect - saves recalculating coordinates
	void draw(); 
	void draw(float x, float y);
	void draw(float x, float y, float width, float height);
	void draw(ofRectangle &rect);
	void setRect(ofRectangle &rect);
private:
	
	ofImage *img;
	ofVec2f vertices[24];
	ofVec2f texCoords[24];
	ofRectangle rect;
};