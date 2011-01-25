/*
 *  ofxOBJModel.cpp
 *
 */

#include "ofxOBJModel.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#ifndef FLT_MAX
	#include <float.h>
#endif

#pragma mark string utils
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}


#pragma mark ofxOBJModel
bool ofxOBJModel::load(string path) {
	filePath = path;
	path = ofToDataPath(path, true);
	
	string line;
	
	for(int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
	meshes.clear();
	
	ObjMesh *currMesh = NULL;
	
	// this is a list of all points
	// that we can drop after parsing
	vector<ofPoint> points; 
	vector<ofPoint> normals;
	vector<ofPoint> texCoords;
	
	// obj file format vertexes are 1-indexed
	points.push_back(ofPoint());
	normals.push_back(ofPoint());
	
	ifstream myfile (path.c_str());
	if (myfile.is_open()) {
		while (! myfile.eof()) {
			getline (myfile,line);
			
			
			// parse the obj format here.
			//
			// the only things we're interested in is
			// lines beginning with 'g' - this says start of new object
			// lines beginning with 'v ' - coordinate of a vertex
			// lines beginning with 'vn ' - vertex normals                   -- todo
			// lines beginning with 'vt ' - texcoords (either 2 or 3 values) -- todo
			// lines beginning with 'f ' - specifies a face of a shape
			// 			we take each number before the slash as the index
			// 			of the vertex to join up to create a face.
			
			if(line.find("g ")==0) { // new object definition
				currMesh = new ObjMesh(line.substr(2));
				meshes.push_back(currMesh);
			} else if(line.find("v ")==0) { // new vertex
				points.push_back(parseCoords(line));
			} else if(line.find("vn ")==0) {
				normals.push_back(parseCoords(line));
			} else if(line.find("vt ")==0) {
				texCoords.push_back(parseCoords(line));
			} else if(line.find("f ")==0) { // face definition
				
				if(currMesh!=NULL) {
					line = line.substr(2); // lop off "f "
					vector<string> indices = split(line, ' ');
					// remove any texcoords (/xxx's)
					
					ObjFace *face = new ObjFace();
					for(int i = 0; i < indices.size(); i++) {
						vector<string> parts = ofSplitString(indices[i], "/");
						
						// first index is always a point
						face->points.push_back(points[atoi(parts[0].c_str())]);

						if(parts.size()==2) {
							face->texCoords.push_back(texCoords[atoi(parts[1].c_str())]);
						} else if(parts.size()==3) {
							face->normals.push_back(normals[atoi(parts[2].c_str())]);
							if(parts[1]!="") {
								face->texCoords.push_back(texCoords[atoi(parts[1].c_str())]);
							}
						}

					}
					currMesh->addFace(face);
				}
			}				
		}
		
		
		myfile.close();
		
		printf("Successfully loaded %s\n-----\nVertices: %d\nMeshes: %d\nNormals: %d\nTexCoords: %d\n", path.c_str(), points.size(), meshes.size(), normals.size(), texCoords.size());
		
		return true;
	} else {
		printf("Couldn't find the OBJ file %s\n", path.c_str());
		return false;
	}
}

	
bool ofxOBJModel::save(string file) {
	if(file=="") {
		file = filePath;
		if(filePath=="") {
			filePath = "untitled.obj";
		}
	}
	file = ofToDataPath(file, true);
	string contents = "";
	int vertexIndex = 1;
	string faceStrings = "";
	
	for(int i = 0; i < meshes.size(); i++) {
		contents += "g " + meshes[i]->name + "\n";
		
		for(int j = 0; j < meshes[i]->faces.size(); j++) {
			string faceString = "f";
			for(int k = 0; k < meshes[i]->faces[j]->points.size(); k++) {
				ofPoint p = meshes[i]->faces[j]->points[k];
				contents += "v " + ofToString(p.x) + " " + ofToString(p.y) + " " + ofToString(p.z) + "\n";
				faceString += " " + ofToString(vertexIndex);
				vertexIndex++;
			}
			faceStrings += faceString + "\n";
			
			
		}
		contents += "\n" + faceStrings + "\n";
		faceStrings = "";
		
	}
	
	ofstream myfile (file.c_str());
	if (myfile.is_open()) {
		myfile << contents;
		myfile.close();
		return true;
	} else {
		printf("Could not write to file\n");
		return false;
	}
	
	
}


ofPoint ofxOBJModel::parseCoords(string line) {
	ofPoint p;
	line = line.substr(line.find(" ")+1);
	vector<string> elements = split(line, ' ');
	if(elements.size()<2) {
		printf("Error line does not have at least 2 coordinates: \"%s\"\n", line.c_str());
		return p;
	}
	p.x = atof(elements[0].c_str());
	p.y = atof(elements[1].c_str());
	if(elements.size()>=3) {
		p.z = atof(elements[2].c_str());
	}
	
	return p;
}
	
/**
 * Takes a line from the obj file beginning with a "v " and 
 * turns it into an ofPoint.
 */
ofPoint ofxOBJModel::parseVertex(string line) {
	ofPoint p;
	if(line.find("v ")!=0) {
		printf("Warning, line does not have vertex info in it: \"%s\"\n", line.c_str());
		return p;
	}
	line = line.substr(2);
	
	vector<string> elements = split(line, ' ');
	if(elements.size()!=3) {
		printf("Error line does not have 3 coordinates: \"%s\"\n", line.c_str());
		return p;
	}
	
	p.x = atof(elements[0].c_str());
	p.y = atof(elements[1].c_str());
	p.z = atof(elements[2].c_str());
	
	return p;
	
}


void ofxOBJModel::draw(bool drawSolid) {
	for(int i = 0; i < meshes.size(); i++) {
		meshes[i]->draw(drawSolid);
	}
}



vector<string> ofxOBJModel::getMeshNames() {
	vector<string> meshNames;
	for(int i = 0; i < meshes.size(); i++) {
		meshNames.push_back(meshes[i]->name);
	}
	return meshNames;
}

void ofxOBJModel::getBounds(ofPoint *minPoint, ofPoint *maxPoint) {
	minPoint->x = minPoint->y = minPoint->z = FLT_MAX;
	maxPoint->x = maxPoint->y = maxPoint->z = -FLT_MAX;

	for(int i = 0; i < meshes.size(); i++) {
		ofPoint min, max;
		meshes[i]->getBounds(&min, &max);
		
		if(max.x>maxPoint->x) maxPoint->x = max.x;
		if(max.y>maxPoint->y) maxPoint->y = max.y;
		if(max.z>maxPoint->z) maxPoint->z = max.z;
		
		if(min.x<minPoint->x) minPoint->x = min.x;
		if(min.y<minPoint->y) minPoint->y = min.y;
		if(min.z<minPoint->z) minPoint->z = min.z;
	}

}


ObjMesh *ofxOBJModel::getMesh(string name) {
	for(int i = 0; i < meshes.size(); i++) {
		if(meshes[i]->name==name) {
			return meshes[i];
		}
	}
	printf("\n--------\nWarning Warning!!!! No mesh named '%s', the program will probably crash.\nPlease update the OBJ file in cinema 4d to have a mesh called '%s'\n", name.c_str(), name.c_str());
	return NULL;
}



#pragma mark ObjMesh	
ObjMesh::ObjMesh(string _name) {
	name = _name;
}

ObjMesh::~ObjMesh() {
	for(int i = 0; i < faces.size(); i++) {
		delete faces[i];
	}
	faces.clear();
}


void ObjMesh::addFace(ObjFace* face) {
	faces.push_back(face);
}

void ObjMesh::draw(bool drawSolid) {
	for(int i = 0; i < faces.size(); i++) {
		faces[i]->draw(drawSolid);
	}
}
// makes the code look nicer.
#define FOR_EACH_POINT_IN_MESH for(int i = 0; i < faces.size(); i++) for(int j = 0; j < faces[i]->points.size(); j++)

void ObjMesh::getBounds(ofPoint *minPoint, ofPoint *maxPoint) {
	minPoint->x = minPoint->y = minPoint->z = FLT_MAX;
	maxPoint->x = maxPoint->y = maxPoint->z = -FLT_MAX;
	FOR_EACH_POINT_IN_MESH {
		ofPoint p = faces[i]->points[j];
		if(p.x>maxPoint->x) maxPoint->x = p.x;
		if(p.y>maxPoint->y) maxPoint->y = p.y;
		if(p.z>maxPoint->z) maxPoint->z = p.z;
			
		if(p.x<minPoint->x) minPoint->x = p.x;
		if(p.y<minPoint->y) minPoint->y = p.y;
		if(p.z<minPoint->z) minPoint->z = p.z;
	}
}

// this moves the whole mesh by the specified amount
void ObjMesh::translate(float dx, float dy, float dz) {
	ofPoint delta = ofPoint(dx, dy, dz);
	FOR_EACH_POINT_IN_MESH {
		faces[i]->points[j] += delta;
	}
}


// these functions move the extremity of the mesh.
// eg moveTop() will move the top vertex/vertices by
// the specified amount.
void ObjMesh::moveTop	(float delta) {
	float extrema = FLT_MAX;

	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].y<extrema) {
			extrema = faces[i]->points[j].y;
		}
	}
	
	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].y==extrema) {
			faces[i]->points[j].y += delta;
		}
	}
}

void ObjMesh::moveBottom	(float delta) {
	
	float extrema = -FLT_MAX;
	
	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].y>extrema) {
			extrema = faces[i]->points[j].y;
		}
	}
	
	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].y==extrema) {
			faces[i]->points[j].y += delta;
		}
	}
}
	
	
void ObjMesh::moveLeft	(float delta) {
	
	
	float extrema = FLT_MAX;
	
	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].x<extrema) {
			extrema = faces[i]->points[j].x;
		}
	}
	
	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].x==extrema) {
			faces[i]->points[j].x += delta;
		}
	}
}

void ObjMesh::moveRight	(float delta) {
	float extrema = -FLT_MAX;
	
	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].x>extrema) {
			extrema = faces[i]->points[j].x;
		}
	}
	
	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i]->points[j].x==extrema) {
			faces[i]->points[j].x += delta;
		}
	}
}

/**
 * Changes the direction of all the vertices. 
 * i.e. clockwise to anti-clockwise
 */
void ObjMesh::flipDirection() {
	for(int i = 0; i < faces.size(); i++) {
		faces[i]->flipDirection();
	}
}


/**
 * Shifts the points along one
 */
void ObjMesh::shiftPointsLeft() {
	for(int i = 0; i < faces.size(); i++) {
		faces[i]->shiftPointsLeft();
	}
}

void ObjMesh::shiftPointsRight() {
	for(int i = 0; i < faces.size(); i++) {
		faces[i]->shiftPointsRight();
	}
}





	
	
#pragma mark ObjFace

ofRectangle ObjFace::get2DRect() {
	ofRectangle rect;
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;
	for(int i = 0; i < points.size(); i++) {
		if(points[i].x>maxX) maxX = points[i].x;
		if(points[i].y>maxY) maxY = points[i].y;
		if(points[i].x<minX) minX = points[i].x;
		if(points[i].y<minY) minY = points[i].y;
	}
	
	rect.x = minX;
	rect.y = minY;
	rect.width = maxX - minX;
	rect.height = maxY - minY;
	return rect;
}


void ObjFace::draw(bool drawSolid) {
	
	if(drawSolid) {
		if(points.size()==3) {
			glBegin(GL_TRIANGLES);
		} else if(points.size()==4) {
			glBegin(GL_QUADS);
		} else {
			glBegin(GL_POLYGON);
		}
		
		bool hasNormals = points.size()==normals.size();
		bool hasTexCoords = points.size()==texCoords.size();
		
		for(int i = 0; i < points.size(); i++) {
			if(hasNormals) glNormal3f(normals[i].x, normals[i].y, normals[i].z);
			if(hasTexCoords) glTexCoord2f(texCoords[i].x, texCoords[i].y);
			glVertex3f(points[i].x, points[i].y, points[i].z);
		}
		glEnd();
		for(int i = 0; i < points.size(); i++) {
			glPushMatrix();
			glTranslatef(points[i].x, points[i].y, points[i].z);
			ofDrawBitmapString(ofToString(texCoords[i].x)+","+ofToString(texCoords[i].y), 0, 0);
			glPopMatrix();
		}
	} else {
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < points.size(); i++) {
			glVertex3f(points[i].x, points[i].y, points[i].z);
		}
		glEnd();
	}
}


/**
 * Changes the direction of all the vertices. 
 * i.e. clockwise to anti-clockwise
 */
void ObjFace::flipDirection() {
	std::reverse(points.begin(), points.end());
}


/**
 * Shifts the points along one
 */
void ObjFace::shiftPointsLeft() {
	std::rotate(points.begin(), points.begin()+points.size()-1, points.end());
}

void ObjFace::shiftPointsRight() {
	std::rotate(points.begin(), points.begin()+1, points.end());
}



void ObjFace::getBounds(ofPoint *min, ofPoint *max) {
	
	min->x = min->y = min->z = FLT_MAX;
	max->x = max->y = max->z = -FLT_MAX;

	for(int i = 0; i < points.size(); i++) {
		if(points[i].x>max->x) max->x = points[i].x;
		if(points[i].y>max->y) max->y = points[i].y;
		if(points[i].z>max->z) max->z = points[i].z;
		if(points[i].x<min->x) min->x = points[i].x;
		if(points[i].y<min->y) min->y = points[i].y;
		if(points[i].z<min->z) min->z = points[i].z;
	}
}