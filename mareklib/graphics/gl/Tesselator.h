// this doesn't work right now

#include "ofMain.h"
class Tesselator {
public:
	Tesselator();

	void beginShape();
	void vertex(float x, float y);
	void bezierVertex(float x1, float y1, float x2, float y2, float x3, float y3);
	void endShape(bool close = true);
	
	void draw();
	bool hasVertices();
	void saveTesselation(vector<float> &newpoints, GLint shapeType);
private:
	
	// stores individual shapes of the tesselations
	class Shape {
	public:
		GLint shapeType;
		vector<float> points;
	};
	
	vector<Shape> shapes;
	
	bool bHasVertices;
	vector<ofPoint> coords;
};