/*
 *  GlowingBorder.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 26/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

/**
 * This creates a glowing border around a polygon
 */
class GlowingBorder {
public:
	
	
	GlowingBorder() {
		normals				= NULL;
		cornerCoordCounts	= NULL;
		cornerCoords		= NULL;
		cornerTexCoords		= NULL;
		edgeCoords			= NULL;
		edgeTexCoords		= NULL;
		
	}
	
	~GlowingBorder() {
		if(normals!=NULL) {
			delete [] normals;
			delete [] cornerCoordCounts;
			for(int i = 0; i < numPoints; i++) {
				delete [] edgeCoords[i];
				delete [] edgeTexCoords[i];
				delete [] cornerCoords[i];
				delete [] cornerTexCoords[i];
			}
			delete [] edgeCoords;
			delete [] edgeTexCoords;
			delete [] cornerCoords;
			delete [] cornerTexCoords;
		}
	}

	void setup(ofImage *img, int numPoints) {
		this->img = img;
		this->numPoints = numPoints;
		this->radius = this->img->getWidth()/2;
		
		// allocate memory for all data needed
		normals = new ofVec2f[numPoints];
		cornerCoordCounts = new int[numPoints];
		
		edgeCoords = new ofVec2f*[numPoints];
		edgeTexCoords = new ofVec2f*[numPoints];
		
		cornerCoords = new ofVec2f*[numPoints];
		cornerTexCoords = new ofVec2f*[numPoints];
		
		
		for(int i = 0; i < numPoints; i++) {
			edgeCoords[i]		= new ofVec2f[4];
			edgeTexCoords[i]	= new ofVec2f[4];
			cornerCoords[i]		= new ofVec2f[7];
			cornerTexCoords[i]	= new ofVec2f[7];
		}
		
	}
	
	
	
	
	
	float normalizeAngle(float angle) {
		if(angle>0) {
			while(angle>PI) {
				angle -= PI*2;
			}
		} else {
			while(angle<-PI) {
				angle += PI*2;
			}
		}
		return angle;
	}
	
#define SQUARE_ERROR 0
#define SQUARE_RIGHT 1
#define SQUARE_TOP 2
#define SQUARE_BOTTOM 3
#define SQUARE_LEFT 4
	
	int findIntersection(float alpha, ofPoint a, float radius, ofPoint &intersection) {
		// left
		if(alpha>-3*PI/4 && alpha<-PI/4) {
			if(ofLineSegmentIntersection(ofPoint(), a, ofPoint(-radius, -radius), ofPoint(-radius, radius), intersection)) {
				return SQUARE_LEFT;
			}
			// bottom
		} else if(alpha<-3*PI/4 || alpha>3*PI/4) {
			if(ofLineSegmentIntersection(ofPoint(), a, ofPoint(-radius, radius), ofPoint(radius, radius), intersection)) {
				return SQUARE_BOTTOM;
			}
			// right
		} else if(alpha>PI/4 && alpha<3*PI/4) {
			if(ofLineSegmentIntersection(ofPoint(), a, ofPoint(radius, radius), ofPoint(radius, -radius), intersection)) {
				return SQUARE_RIGHT;
			}
			// top
		} else if(alpha>-PI/4 && alpha<PI/4) {
			if(ofLineSegmentIntersection(ofPoint(), a, ofPoint(radius, -radius), ofPoint(-radius, -radius), intersection)) {
				return SQUARE_TOP;
			}
		}
		return SQUARE_ERROR;
	}
	
	
	void drawRectSegment(ofPoint centre, float radius, float alpha, float beta) {
		
		// normalize angles to -PI<angle<PI
		alpha = normalizeAngle(alpha);
		beta = normalizeAngle(beta);
		
	
		ofVec2f a(0, -radius*2);
		ofVec2f b(0, -radius*2);
		a.rotateRad(alpha);
		b.rotateRad(beta);
		
		glPushMatrix();
		glTranslatef(centre.x, centre.y, 0);
		ofPoint intersectionA, intersectionB;
		
		
		int sideA = findIntersection(alpha, a, radius, intersectionA);
		int sideB = findIntersection(beta, b, radius, intersectionB);
		
		int currSide = sideA;
		//ball.bind();
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(radius, radius);
		glVertex2f(0, 0);
		glTexCoord2f(intersectionA.x+radius, intersectionA.y+radius);
		glVertex2f(intersectionA.x, intersectionA.y);
		if(sideA!=SQUARE_ERROR && sideB!=SQUARE_ERROR) {
			while(currSide!=sideB) {
				// draw the side, 
				if(currSide==SQUARE_LEFT) {
					glTexCoord2f(0, 0);
					glVertex2f(-radius, -radius);
					currSide = SQUARE_TOP;
				} else if(currSide==SQUARE_TOP) {
					glTexCoord2f(radius*2, 0);
					glVertex2f(radius, -radius);
					currSide = SQUARE_RIGHT;
				} else if(currSide==SQUARE_RIGHT) {
					glTexCoord2f(radius*2, radius*2);
					glVertex2f(radius, radius);
					currSide = SQUARE_BOTTOM;
				} else if(currSide==SQUARE_BOTTOM) {
					glTexCoord2f(0, radius*2);
					glVertex2f(-radius, radius);
					currSide = SQUARE_LEFT;
				}
			}
		}
		glTexCoord2f(intersectionB.x+radius, intersectionB.y+radius);
		glVertex2f(intersectionB.x, intersectionB.y);
		glEnd();
		//ball.unbind();
		//ofCircle(intersection.x, intersection.y, 3);
		
		glPopMatrix();
	}
	
	
	
	void drawCorners() {
	
		
		ofSetColor(color);
		
		// draw fluff on the 3 corners
		for(int i = 0; i < numPoints; i++) {

			int pointBefore = i-1;
			if(pointBefore<0) pointBefore += numPoints;
			ofPoint normal = normals[pointBefore];

			float alpha = atan2(normal.y,normal.x)   + PI/2;
			float beta = atan2(normals[i].y,normals[i].x) + PI/2;

			drawRectSegment(points[i], radius, alpha, beta);
		}
		
		
		
	//	glEnableClientState(GL_VERTEX_ARRAY);
//		glVertexPointer(3, GL_FLOAT, 0, &points[0]);
//		glDrawArrays(GL_TRIANGLES, 0, 3);
//		glDisableClientState(GL_COLOR_ARRAY);
		
	}
	
	void drawEdges() {

		
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		for(int i = 0; i < numPoints; i++) {
			
			glVertexPointer(2, GL_FLOAT, 0, &edgeCoords[i][0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &edgeTexCoords[i][0]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		
		

	}
	
	
	void _drawCorners() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		for(int i = 0; i < numPoints; i++) {
			glPushMatrix();
			glTranslatef(points[i].x, points[i].y, 0);
			glVertexPointer(2, GL_FLOAT, 0, &cornerCoords[i][0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &cornerTexCoords[i][0]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, cornerCoordCounts[i]);
			glPopMatrix();
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
	}

	void calculateNormals() {
		for(int i = 0; i < numPoints; i++) {
			normals[i] = points[(i+1)%numPoints] - points[i];
			normals[i] = ofVec2f(normals[i].y, -normals[i].x);
			normals[i].normalize();
		}
	}
	
	
	void calculateEdges() {
		for(int i = 0; i < numPoints; i++) {
			ofVec3f o1, o2;
			
			ofPoint a = points[i];
			ofPoint b = points[(i+1)%numPoints];
			
			o1 = a + normals[i]*radius;
			o2 = b + normals[i]*radius;
			ofVec2f topTexCoord(radius, 0);
			
			// angle is the angle of the normal with -y
			float angle = atan(normals[i].x/normals[i].y);
			topTexCoord.rotateRad(1, ofVec2f( radius, radius));

			edgeTexCoords[i][0] = ofVec2f(topTexCoord.x, topTexCoord.y);
			edgeCoords[i][0] = ofVec2f(o1.x, o1.y);
			edgeTexCoords[i][1] = ofVec2f(radius, radius);
			edgeCoords[i][1] = ofVec2f(a.x, a.y);
			edgeTexCoords[i][2] = ofVec2f(topTexCoord.x, topTexCoord.y);
			edgeCoords[i][2] = ofVec2f(o2.x, o2.y);
			edgeTexCoords[i][3] = ofVec2f(radius, radius);
			edgeCoords[i][3] = ofVec2f(b.x, b.y);
		}
	}
	
	void calculateAngles() {
		// cache the angles here for calculateEdges and calculateCorners
		
	}
	
	
	
	
	void calculateRectSegment(int pointId, float alpha, float beta) {
		
		ofPoint centre = points[pointId];
		
		// normalize angles to -PI<angle<PI
		alpha = normalizeAngle(alpha);
		beta = normalizeAngle(beta);
		
		
		ofVec2f a(0, -radius*2);
		ofVec2f b(0, -radius*2);
		a.rotateRad(alpha);
		b.rotateRad(beta);
		

		ofPoint intersectionA, intersectionB;
		
		
		int sideA = findIntersection(alpha, a, radius, intersectionA);
		int sideB = findIntersection(beta, b, radius, intersectionB);
		
		int currSide = sideA;

			
		int pos = 0;
		
		cornerTexCoords[pointId][pos] = ofVec2f(radius, radius);
		cornerCoords[pointId][pos] = ofVec2f(0, 0);
		pos++;
		cornerTexCoords[pointId][pos] = ofVec2f(intersectionA.x+radius, intersectionA.y+radius);
		cornerCoords[pointId][pos] = ofVec2f(intersectionA.x, intersectionA.y);
		pos++;
		if(sideA!=SQUARE_ERROR && sideB!=SQUARE_ERROR) {
			while(currSide!=sideB) {
				// draw the side, 
				if(currSide==SQUARE_LEFT) {
					cornerTexCoords[pointId][pos] = ofVec2f(0, 0);
					cornerCoords[pointId][pos] = ofVec2f(-radius, -radius);
					pos++;
					currSide = SQUARE_TOP;
				} else if(currSide==SQUARE_TOP) {
					cornerTexCoords[pointId][pos] = ofVec2f(radius*2, 0);
					cornerCoords[pointId][pos] = ofVec2f(radius, -radius);
					pos++;
					currSide = SQUARE_RIGHT;
				} else if(currSide==SQUARE_RIGHT) {
					cornerTexCoords[pointId][pos] = ofVec2f(radius*2, radius*2);
					cornerCoords[pointId][pos] = ofVec2f(radius, radius);
					pos++;
					currSide = SQUARE_BOTTOM;
				} else if(currSide==SQUARE_BOTTOM) {
					cornerTexCoords[pointId][pos] = ofVec2f(0, radius*2);
					cornerCoords[pointId][pos] = ofVec2f(-radius, radius);
					pos++;
					currSide = SQUARE_LEFT;
				}
			}
		}
		
		cornerTexCoords[pointId][pos] = ofVec2f(intersectionB.x+radius, intersectionB.y+radius);
		cornerCoords[pointId][pos] = ofVec2f(intersectionB.x, intersectionB.y);
		pos++;
		cornerCoordCounts[pointId] = pos;
	}
	
	
	
	void calculateCorners() {
		
		// draw fluff on the 3 corners
		for(int i = 0; i < numPoints; i++) {
			
			int pointBefore = i-1;
			if(pointBefore<0) pointBefore += numPoints;
			ofPoint normal = normals[pointBefore];
			
			float alpha = atan2(normal.y,normal.x)   + PI/2;
			float beta = atan2(normals[i].y,normals[i].x) + PI/2;
			
			calculateRectSegment(i, alpha, beta);
		}
		
	}
	
	
	
	
	// works for 3 or 4 points. More, I don't know.
	void draw(ofPoint *points, ofColor c) {
		this->points = points;
		
		calculateNormals();
		calculateAngles();
		calculateCorners();
		calculateEdges();
		
		
		color = c;
		ofSetColor(c);
		ball.bind();
		_drawCorners();
		drawEdges();
		ball.unbind();
		
		
		
	}
private:
	ofImage *img;
	float radius;
	ofVec2f *normals;
	ofPoint *points;
	ofVec2f **edgeCoords;
	ofVec2f **edgeTexCoords;
	
	ofVec2f **cornerCoords;
	ofVec2f **cornerTexCoords;
	int *cornerCoordCounts;

	
	int numPoints;
	ofColor color;
};