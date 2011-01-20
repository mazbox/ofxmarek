
// basic SVG 1.0 parser. Don't know if it works for other versions.
// also, it only works for a subset of types of shape.
#pragma once

// this should make it much faster (but it doesn't and is a bit 
// broken to boot - it caches the tesselated 
// shapes of filled paths and draws them back with vertex arrays
//#define USING_TESSELATOR

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxDisplayList.h"


#ifdef USING_TESSELATOR 
#include "Tesselator.h"
#endif
class SVGObject {
public:
	ofColor fillColor;
	virtual void draw() = 0;
	virtual ofRectangle getRect() = 0;
};

class SVGCircle: public SVGObject {
public:
	ofPoint center;
	float radius;
	void draw() {
		ofFill();
		ofSetColor(fillColor);
		ofCircle(center.x, center.y, radius);
	}
	ofRectangle getRect() {
		return ofRectangle(center.x - radius, center.y - radius, radius*2, radius*2);
	}
	
};


#define SVGVector_BezierPoint 1
#define SVGVector_Point 2
class SVGPoint {
public:
	SVGPoint(float x, float y){
		p = ofPoint(x,y);
		c1 = ofPoint(x, y);
		c2 = ofPoint(x, y);
		type = SVGVector_Point;
	}
	
	SVGPoint(float c1x, float c1y, float c2x, float c2y, float px2, float py2){
		p = ofPoint(px2,py2);
		c1 = ofPoint(c1x,c1y);
		c2 = ofPoint(c2x,c2y);
		type = SVGVector_BezierPoint;
	}
	
	int type;
	ofPoint p, c1, c2;
};

class SVGPath: public SVGObject {
public:
	vector<SVGPoint> path;
	vector<ofPoint> vertexs;
	ofxDisplayList displayList;
#ifdef USING_TESSELATOR
	Tesselator tess;
#endif
/*	void draw() {
		
		ofSetColor(fillColor);
#ifdef USING_TESSELATOR
		if(tess.hasVertices()) {
			tess.draw();
			return;
		}
		tess.beginShape();
		printf("Using tess\n");
#else
		ofBeginShape();
#endif
			
			

		for(int k = 0; k < path.size(); k++){

			if( path[k].type == SVGVector_Point ){
					
#ifdef USING_TESSELATOR
				tess.vertex
#else
				ofVertex
#endif
					(path[k].p.x, path[k].p.y);
			
			} else if( path[k].type == SVGVector_BezierPoint ){
#ifdef USING_TESSELATOR
				tess.bezierVertex
#else
				ofBezierVertex
#endif
					(path[k].c1.x, path[k].c1.y, path[k].c2.x, path[k].c2.y, path[k].p.x, path[k].p.y);
			}
		}
#ifdef USING_TESSELATOR
			
			tess.endShape();
#else
			ofEndShape(true);
#endif
		 
	}*/
	
	
	
	
	
	
	
	
	
	
	
	void draw() {
		ofSetColor(fillColor);
		if(!displayList.ready()) {
			displayList.begin(false);
			ofBeginShape();

			
			for(int k = 0; k < path.size(); k++){
				
				if( path[k].type == SVGVector_Point ){
					
					ofVertex

					(path[k].p.x, path[k].p.y);
					
				} else if( path[k].type == SVGVector_BezierPoint ){
					ofBezierVertex(path[k].c1.x, path[k].c1.y, path[k].c2.x, path[k].c2.y, path[k].p.x, path[k].p.y);
				}
			}
			ofEndShape(true);
			displayList.end();
		} 
		displayList.draw();
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	ofRectangle getRect() {
		ofRectangle rect(FLT_MAX, FLT_MAX, FLT_MIN, FLT_MIN);
		for(int i = 0; i < path.size(); i++) {
			if(path[i].p.x<rect.x) rect.x = path[i].p.x;
			if(path[i].p.y<rect.y) rect.y = path[i].p.y;
			if(path[i].p.x>rect.width) rect.width = path[i].p.x;
			if(path[i].p.y>rect.height) rect.height = path[i].p.y;
		}
		
		// the width and height are actually the maximum right now
		rect.width = rect.width - rect.x;
		rect.height = rect.height - rect.y;
		return rect;
	}
};

class SVGLayer {
	
public:
	
	float opacity;
	float alpha;
	
	SVGLayer() {
		opacity = 1;
		alpha = 1;
	}
	
	void setAlpha(float alpha) {
		this->alpha = alpha;
	}
	
	void draw() {
		
		for(int i = 0; i < objects.size(); i++) {
			drawObject(i);
		}
	}
	
	void drawObject(int objectIndex) {
		
		objects[objectIndex]->fillColor.a = opacity*255.f*alpha;
		objects[objectIndex]->draw();
	}
	
	vector<SVGObject*> objects;
};

class SVG {
public:
	ofxXmlSettings xml;
	void setAlpha(float alpha) {
		for(int i = 0; i < layers.size(); i++) {
			layers[i]->setAlpha(alpha);
		}
	}
	
	
	void load(string file) {
		
		if(!xml.loadFile(file)) {
			printf("Couldn't load SVG file '%s'\n", file.c_str());
		}
		int numLayers = xml.getNumTags("svg");
		//printf("%d layers\n", numLayers);
		for(int i = 0; i < numLayers; i++) {
			xml.pushTag("svg", i);
			
			parseLayer();
			
			xml.popTag();
		}
		findDimensions();
	}
	ofRectangle rect;
	void findDimensions() {
		rect.x = FLT_MAX;
		rect.y = FLT_MAX;
		rect.width = FLT_MIN;
		rect.height = FLT_MIN;
		for(int i = 0; i < layers.size(); i++) {
			for(int j = 0; j < layers[i]->objects.size(); j++) {
				SVGObject *o = layers[i]->objects[j];
				ofRectangle orect = o->getRect();
				if(orect.x<rect.x) rect.x = orect.x;
				if(orect.y<rect.y) rect.y = orect.y;
				ofPoint maxPoint = ofPoint(orect.x+orect.width, orect.y+orect.height);
				if(maxPoint.x>rect.width) rect.width = maxPoint.x;
				if(maxPoint.y>rect.height) rect.height = maxPoint.y;
			}
		}
		rect.width -= rect.x;
		rect.height -= rect.y;
		//printf("Rect: %f %f %f %f\n", rect.x, rect.y, rect.width, rect.height);
	}
	
	
	void parseLayer() {
		
		SVGLayer *layer = new SVGLayer();
		
		if(xml.attributeExists("g", "opacity")) {
			layer->opacity = xml.getAttribute("g", "opacity", 0.f);
		}
		
		xml.pushTag("g");
		int numPaths = xml.getNumTags("path");
		for(int i = 0; i < numPaths; i++) {

			SVGPath *path = parsePath(xml.getAttribute("path", "d", "", i));
			path->fillColor = hexToColor(xml.getAttribute("path", "fill", "", i));
			layer->objects.push_back(path);
		}
		
		int numCircles = xml.getNumTags("circle");
		for(int i = 0; i < numCircles; i++) {
			SVGCircle *circle = new SVGCircle();
			circle->center.x = xml.getAttribute("circle", "cx", 0.f, i);
			circle->center.y = xml.getAttribute("circle", "cy", 0.f, i);
			circle->radius = xml.getAttribute("circle", "r", 0.f, i);
			if(xml.attributeExists("circle", "fill", i)) {
				circle->fillColor = hexToColor(xml.getAttribute("circle", "fill", "", i));
			}
			layer->objects.push_back(circle);
		}
		xml.popTag();
		layers.push_back(layer);
	}
	
	SVGPath *parsePath(string data) {
		SVGPath *path = new SVGPath();
			
		
		string sepPath = "";
		
		for(int i = 0; i < data.size(); i++){
			char c = data[i];
			char cb = data[i];
			if(i>0) cb = data[i+1];
			if( c < 31 )continue;
			
			if( c == 'M' || c == 'm' ||
			   c == 'L' || c == 'l' ||
			   c == 'V' || c == 'v' ||
			   c == 'H' || c == 'h' ||
			   c == 'C' || c == 'c' ||
			   c == 'S' || c == 's' ||
			   c == 'z' || c == 'Z' )
			{
				sepPath += "*";
				sepPath.push_back(c);
				//sepPath += "*"; 
			}else if(c == '-' || c == ' ' || c == ','){
				if( c == '-'){
					if( sepPath.length() > 0 && cb >= '0' && cb <= '9' ){
						sepPath.push_back('|');
					} 
					sepPath.push_back(c);
				}else{
					sepPath.push_back('|');
				}
			}else{
				sepPath.push_back(c);
			}
			
		}
		
		pathToVectorData(sepPath, path);

		return path;
	}
	
	
	
	
	
	
	
	
	void pathToVectorData(string pathStr, SVGPath* obj){
		ofPoint drawPt;
		
		vector <string> commandStr = ofSplitString(pathStr, "*");
		
		for(int i = 0; i < commandStr.size(); i++){
			if( commandStr[i].length() ==  0 )continue;
			
			string currStr = commandStr[i];
			char command = currStr[0];
			
			currStr = currStr.substr(1, currStr.length()-1);
			
			//printf("[%c], curString is %s\n", command, currStr.c_str());
			
			vector <string> coordsStr = ofSplitString(currStr, "|");
			vector <float> coords;
			
			for(int i = 0; i < coordsStr.size(); i++){
				coords.push_back(ofToFloat(coordsStr[i]));
			}
			
			int numP = coords.size();
			
			if( ( command == 'm' || command == 'M' ) && numP >= 2){
				if( command == 'm'){
					drawPt.x += coords[0];
					drawPt.y += coords[1];				
				}else{
					drawPt.x = coords[0];
					drawPt.y = coords[1];
				}					
				//currShape.addPoint(drawPt.x, drawPt.y);
				obj->path.push_back(SVGPoint(drawPt.x, drawPt.y));
			}
			
			if( ( command == 'v' || command == 'V' ) && numP >= 1 ){
				
				for(int j = 0; j < numP; j++){
					
					if( command == 'v'){
						drawPt.y += coords[j+0];				
					}else{
						drawPt.y = coords[j+0];
					}					
					//currShape.addPoint(drawPt.x, drawPt.y);
					obj->path.push_back(SVGPoint(drawPt.x, drawPt.y));
				}
			}
			
			if( ( command == 'h' || command == 'H' ) && numP >= 1 ){
				
				for(int j = 0; j < numP; j++){
					
					if( command == 'h'){
						drawPt.x += coords[j+0];				
					}else{
						drawPt.x = coords[j+0];
					}					
					//currShape.addPoint(drawPt.x, drawPt.y);
					obj->path.push_back(SVGPoint(drawPt.x, drawPt.y));
				}
			}
			
			if( ( command == 'l' || command == 'L' )  && numP >= 2 ){
				
				for(int j = 0; j < numP; j+=2){
					
					if( command == 'l'){
						drawPt.x += coords[j+0];
						drawPt.y += coords[j+1];				
					}else{
						drawPt.x = coords[j+0];
						drawPt.y = coords[j+1];
					}					
					//currShape.addPoint(drawPt.x, drawPt.y);
					obj->path.push_back(SVGPoint(drawPt.x, drawPt.y));
				}
			}
			
			if( ( command == 'c' || command == 'C' )  && numP >= 6 ){
				
				for(int j = 0; j < numP; j+=6){
					
					float c1x, c1y;
					float c2x, c2y;
					
					if( command == 'c'){
						c1x = drawPt.x + coords[j+0];
						c1y = drawPt.y + coords[j+1];
						c2x = drawPt.x + coords[j+2];
						c2y = drawPt.y + coords[j+3];
						drawPt.x += coords[j+4];
						drawPt.y += coords[j+5];				
					}else{
						c1x = coords[j+0];
						c1y = coords[j+1];
						c2x = coords[j+2];
						c2y = coords[j+3];
						drawPt.x = coords[j+4];
						drawPt.y = coords[j+5];
					}					
					//currShape.addBezier(c1x, c1y, c2x, c2y, drawPt.x, drawPt.y);
					obj->path.push_back(SVGPoint(c1x, c1y, c2x, c2y, drawPt.x, drawPt.y));
				}
			}
			
			if( ( command == 's' || command == 'S' )  && numP >= 4 ){
				
				for(int j = 0; j < numP; j+=4){
					
					ofPoint prePoint = drawPt;
					
					ofPoint preHandle;
					if( obj->path.size() && obj->path.back().type == SVGVector_BezierPoint ){
						preHandle = obj->path.back().c2 - prePoint;
					}else{
						preHandle = drawPt - prePoint;
					}
					
					float c1x, c1y;
					float c2x, c2y;
					
					if( command == 's'){
						c1x = prePoint.x - preHandle.x;
						c1y = prePoint.y - preHandle.y;
						
						c2x = drawPt.x + coords[j+0];
						c2y = drawPt.y + coords[j+1];
						drawPt.x += coords[j+2];
						drawPt.y += coords[j+3];				
					}else{
						c1x = prePoint.x - preHandle.x;
						c1y = prePoint.y - preHandle.y;
						
						c2x = coords[j+0];
						c2y = coords[j+1];
						drawPt.x = coords[j+2];
						drawPt.y = coords[j+3];
					}					
					//currShape.addBezier(c1x, c1y, c2x, c2y, drawPt.x, drawPt.y);
					obj->path.push_back(SVGPoint(c1x, c1y, c2x, c2y, drawPt.x, drawPt.y));
				}
			}
			
		}
	}
	/*
	
	void ofxSVG::vectorDataToVertexs(ofxSVGPath* path, float resampleDist){	
		int numPts = path->vectorData.size();
		
		ofxVec2f delta;
		
		vector <ofxSVGPoint> pts = path->vectorData;
		
		if( numPts >= 2 ){
			
			for(int k = 0; k < pts.size(); k++){					
				
				bool nAddPts = false;
				float len = 0.0;
				
				if( k > 0 ){
					delta		= pts[k].p - pts[k-1].p;
					len			= delta.length();
					if( len > resampleDist * 2 ){
						nAddPts = true;
					}
				}
				
				if( nAddPts ){
					
					if( pts[k].type == ofxSVGVector_BezierPoint && k > 0 ){
						
						
						vector <ofPoint> bezPts = singleBezierToPtsWithResample(pts[k-1].p.x,  pts[k-1].p.y,  
																				pts[k].c1.x,  pts[k].c1.y, 
																				pts[k].c2.x,  pts[k].c2.y,  
																				pts[k].p.x,  pts[k].p.y, resampleDist);
						
						for(int i = 0; i < bezPts.size(); i++){
							path->vertexs.push_back(bezPts[i]);
						}	
						
					}else{
						
						int numToAdd = ((int)(len / resampleDist))-1;
						
						for(int i = 0; i < numToAdd; i++){
							float pct = ofMap(i+1, 0, numToAdd+1, 0.0, 1.0);
							
							ofPoint iPnt = pts[k].p * pct + pts[k-1].p * (1.0-pct);
							path->vertexs.push_back(iPnt); 
						}
						
					}
					
				}
				
				path->vertexs.push_back(pts[k].p);
			}
			
		}	
	}
	
	
	
	
	
	
	
	
	
	*/
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	int decVal(char a) {
		switch(a) {
			case '0': return 0; break;
			case '1': return 1; break;
			case '2': return 2; break;
			case '3': return 3; break;
			case '4': return 4; break;
			case '5': return 5; break;
			case '6': return 6; break;
			case '7': return 7; break;
			case '8': return 8; break;
			case '9': return 9; break;
			case 'A': return 10; break;
			case 'B': return 11; break;
			case 'C': return 12; break;
			case 'D': return 13; break;
			case 'E': return 14; break;
			case 'F': return 15; break;
			default: return 0; break;
		}
	}
	
	int decVal(char a, char b) {
		return decVal(a)*16 + decVal(b);
	}
	
	ofColor hexToColor(string hexString) {
		int r = decVal(hexString[1], hexString[2]);
		int g = decVal(hexString[3], hexString[4]);
		int b = decVal(hexString[5], hexString[6]);
		return ofColor(r, g, b);
	}
	
	void draw(float x = 0, float y = 0, float size = 0) {
		
		
		beginDrawing(x, y, size);
		for(int i = 0; i < layers.size(); i++) {
			layers[i]->draw();
		}
		endDrawing();
	}
	
	float getSize() {
		return (rect.width>rect.height)?rect.width:rect.height;
	}
	
	void beginDrawing(float x = 0, float y = 0, float size = 0) {
		glPushMatrix();
		float scale = 1;
		if(size!=0) {
			if(rect.width>rect.height) {
				scale = size/rect.width;
			} else {
				scale = size/rect.height;
			}
			glScalef(scale, scale, scale);
		}
		glTranslatef(-rect.x-rect.width/2, -rect.y-rect.height/2, 0);
		if(x!=0) {
			glTranslatef(x/scale, y/scale, 0);
		}
	}
	
	void endDrawing() {
		glPopMatrix();
	}
	
	int getNumLayers() {
		return layers.size();
	}
	
	float getWidth() {
		return rect.width;
	}
	
	float getHeight() {
		return rect.height;
	}
	
	
	void drawLayer(int layerIndex) {
		beginDrawing();
		layers[layerIndex]->draw();
		endDrawing();
	}
	
	// these functions allow you to access items in a layer
	int getNumObjects(int layerIndex = 0) {
		return layers[layerIndex]->objects.size();
	}
	
	void drawObject(int layerIndex, int objectIndex, float size = 0) {
		beginDrawing(0, 0, size);
		layers[layerIndex]->drawObject(objectIndex);
		endDrawing();
	}
	
	void setColor(int color) {
		for(int i = 0; i < layers.size(); i++) {
			for(int j = 0; j < layers[i]->objects.size();j++){
				layers[i]->objects[j]->fillColor.setHex(color);
			}
		}
	}
	vector<SVGLayer*> layers;
};