
#include "ofMain.h"
#include "ofVec2f.h"

// gets the intersection of two infinitely long lines described by two points each.
ofPoint getIntersection(ofPoint a1, ofPoint a2, ofPoint b1, ofPoint b2);

bool pointIsInsideTriangle(ofVec2f pt, ofVec2f v1, ofVec2f v2, ofVec2f v3);