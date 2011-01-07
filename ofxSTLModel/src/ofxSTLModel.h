// adapted from the STL class by Marius Watz (part of unlekkerlib from processing)
// taken from http://workshop.evolutionzone.com/unlekkerlib/ on 06/12/09
// converted to C++ by Marek Bereza

/**
 * Class to export and import binary STL (stereolithography) files from Processing. 
 * STL files are typically used for rapid prototyping, and consist of simple triangle data with a face normal. 
 * See <a href="http://en.wikipedia.org/wiki/STL_(file_format)">Wikipedia: STL</a> for more info about the format. 
 * <p>Using Processing's <code>beginRaw()</code> mechanism geometry can be output through this class.  
 * STL files can also be loaded into memory.</p>
 * <p>I recommend the excellent Open Source object viewer 
 * <a href="http://meshlab.sourceforge.net/">MeshLab</a> for checking STL models. 
 * It will show inverted normals, invalid polygons and so on. It will even attempt
 * some limited repairs of damaged files.</p> 
 */

#include "STLFace.h"
#include <fstream>
#include "ofMain.h"

class ofxSTLModel {
public:

	ofxSTLModel();
	
	/**
	 * Draws the STL model with GL triangles
	 */
	void draw();
	
	/**
	 * Read a binary encoded STL file. (put it in the data folder)
	 */
	void read(string path);
	
	/**
	 * Write a binary encoded STL file. (created in the data folder)
	 */
	void write(string path);
	
	
	/**
	 * STL models are made of triangles only, so you need
	 * to add triangle by triangle.
	 * The first 3 parameters are the triangle's normal, 
	 * the other 9 are the coordinates of the 3 points.
	 */
	void addTriangle(float nX, float nY, float nZ,
					 float x1, float y1, float z1,
					 float x2, float y2, float z2,
					 float x3, float y3, float z3
	);	
	
	/** Array of STLFace objects comprising the current geometry. */
	vector<STLFace> triangles;

	/**
	 * Centers the object around the world origin. 
	 */
	void center();
	
	/**
	 * Normalizes the object to a absolute scale 
	 */
	void normalize(float m);
	
private:
	float minx,maxx,miny,maxy,minz,maxz,centerx,centery,centerz;
	float mult;
	void calcBounds();
	
	

		

		
};
