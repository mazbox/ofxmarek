/*
 *  ofxOBJModel.h
 *
 *  This is a really simple wavefront obj loader/saver.
 *  It's no good for anything complicated, just good for 
 *  loading and saving simple 3d meshes for projection mapping.
 *
 *  You can create files this format reads and writes in
 *  Cinema4D, you can name the objects in cinema 4d so you
 *  can reference them here. Each object shows up as an "ObjMesh"
 *  here.
 *  
 *  You must make sure in cinema4d that you convert your shapes
 *  to meshes - you do this by selecting the object and pressing
 *  the 'c' key - the key for 'make editable.. It'll only work on 
 *  3d objects (including planes) so if you have a path, you'll need
 *  to use the extrude nurbs object, with the extrusion depth set to
 *  zero, then press 'c'.
 *
 */

#pragma once

#include "ofMain.h"



class ObjFace {
public:
	vector<ofPoint> points;
	vector<ofPoint> normals;
	vector<ofPoint> texCoords;
	
	void draw(bool drawSolid = true);
	
	/**
	 * Changes the direction of all the vertices. 
	 * i.e. clockwise to anti-clockwise
	 */
	void flipDirection();
	
	
	/**
	 * Shifts the points along one
	 */
	void shiftPointsLeft();
	void shiftPointsRight();
	
	/**
	 * Gives you a rectangle in 2d
	 * of the face.
	 */
	ofRectangle get2DRect();

	/**
	 * Get the minimum and maximum bounding points
	 */
	void getBounds(ofPoint *min, ofPoint *max);
};


class ObjMesh {
public:
	vector<ObjFace*> faces;
	string name;
	
	ObjMesh(string _name = "untitled");
	
	~ObjMesh();
		
	void addFace(ObjFace* face);
	
	void getBounds(ofPoint *minPoint, ofPoint *maxPoint);
	
	void draw(bool drawSolid = true);
	
	// this moves the whole mesh by the specified amount
	void translate(float dx, float dy, float dz);
	
	// these functions move the extremity of the mesh.
	// eg moveTop() will move the top vertex/vertices by
	// the specified amount.
	void moveTop	(float delta);
	void moveBottom	(float delta);
	void moveLeft	(float delta);
	void moveRight	(float delta);
	
	/**
	 * Changes the direction of all the vertices. 
	 * i.e. clockwise to anti-clockwise
	 */
	void flipDirection();
	
	
	/**
	 * Shifts the points along one
	 */
	void shiftPointsLeft();
	void shiftPointsRight();
};


class ofxOBJModel {
public:
	
	/**
	 * Accessible array of meshes in the OBJ file.
	 */ 
	vector<ObjMesh*> meshes;
	
	/**
	 * load an obj file, put in the data/ dir.
	 */
	bool load(string path);
	
	/**
	 * save the obj file, relative to data/ dir.
	 * If you call save with no parameter, it saves
	 * to where the obj was loaded
	 */
	bool save(string file = "");

	/**
	 * Returns a list of the names of the meshes in the OBJ file.
	 */
	vector<string> getMeshNames();
	
	/**
	 * Returns a mesh given its name.
	 */
	ObjMesh *getMesh(string name);
	
	/**
	 * Finds the minimum and maximum points in all the meshes.
	 */
	void getBounds(ofPoint *minPoint, ofPoint *maxPoint);
	
	/**
	 * Draw all the meshes.
	 */
	void draw(bool drawSolid = true);
	
	

	
	
private:
	/**
	 * Takes a line from the obj file beginning with a "v " and 
	 * turns it into an ofPoint.
	 */
	ofPoint parseVertex(string line);
	
	ofPoint parseCoords(string line);
	string filePath;
};



std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);