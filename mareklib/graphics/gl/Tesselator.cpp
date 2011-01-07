/*
 *  Tesselator.cpp
 *  Snowboard
 *
 *  Created by Marek Bereza on 08/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "Tesselator.h"
static GLUtesselator * mytess = NULL;
static GLint myShapeType;
std::vector <double*> mynewVectrices;
std::vector <float> mytessVertices;
std::vector <double*> mypolyVertices;

Tesselator *currTess = NULL;
Tesselator::Tesselator() {
	bHasVertices = false;
}


void Tesselator::saveTesselation(vector<float> &newpoints, GLint shapeType) {
	if(shapeType==GL_TRIANGLES) {
		for(int i = 0; i < shapes.size(); i++) {
			
			// add triangles to a previous shape if there is one
			if(shapes[i].shapeType==GL_TRIANGLES) {
				for(int j = 0; j < newpoints.size(); j++) {
					shapes[i].points.push_back(newpoints[j]);
				}
				printf("Combined a shape");
				return;
			}
		}
	}
	
	printf("stype: %d\n", shapeType);
	shapes.push_back(Shape());
	shapes.back().shapeType = shapeType;
	shapes.back().points = newpoints;
	bHasVertices = true;
}
bool Tesselator::hasVertices() { return bHasVertices; }

//----------------------------------------------------------
void CALLBACK MytessError(GLenum errCode){
	const GLubyte* estring;
	estring = gluErrorString( errCode);
	ofLog(OF_LOG_ERROR, "tessError: %s", estring);
}


//----------------------------------------------------------
void CALLBACK MytessBegin(GLint type){
	myShapeType = type;
}

//----------------------------------------------------------
void CALLBACK MytessEnd(){
	//we draw as 3d not 2d: change 3s bellow to 2 and comment the 3rd push_back in tessVertex to do 2D
	currTess->saveTesselation(mytessVertices, myShapeType);

	//mytessVertices.clear();
	//printf("tessEnd\n");
}


//----------------------------------------------------------
void CALLBACK MytessVertex( void* data){
	
	mytessVertices.push_back( ( (double *)data)[0] );
	mytessVertices.push_back( ( (double *)data)[1] );
	mytessVertices.push_back( ( (double *)data)[2] );	//No need for z for now?
	//printf("tessVertex\n");
}


//----------------------------------------------------------
void CALLBACK MytessCombine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData){
    double* vertex = new double[3];
    mynewVectrices.push_back(vertex);
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    *outData = vertex;
}



void Tesselator::beginShape() {
	mytessVertices.clear();
	coords.clear();
	shapes.clear();
	mytess = gluNewTess();


	// --------------------------------------------------------
	// note: 	you could write your own begin and end callbacks
	// 			if you wanted to...
	// 			for example, to count triangles or know which
	// 			type of object tess is giving back, etc...
	// --------------------------------------------------------

	#if defined( TARGET_OSX)
	#ifndef MAC_OS_X_VERSION_10_5
	#define OF_NEED_GLU_FIX
	#endif
	#endif

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// MAC - XCODE USERS PLEASE NOTE - some machines will not be able to compile the code below
	// if this happens uncomment the "OF_NEED_GLU_FIX" line below and it
	// should compile also please post to the forums with the error message, you OS X version,
	// Xcode verison and the CPU type - PPC or Intel. Thanks!
	// (note: this is known problem based on different version of glu.h, we are working on a fix)
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//#define OF_NEED_GLU_FIX

	#ifdef OF_NEED_GLU_FIX
	#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)(...)
	#else
	#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)()
	#endif

	gluTessCallback( mytess, GLU_TESS_BEGIN, OF_GLU_CALLBACK_HACK)&MytessBegin);
	gluTessCallback( mytess , GLU_TESS_VERTEX, OF_GLU_CALLBACK_HACK)&MytessVertex);
	gluTessCallback( mytess, GLU_TESS_COMBINE, OF_GLU_CALLBACK_HACK)&MytessCombine);
	gluTessCallback( mytess, GLU_TESS_END, OF_GLU_CALLBACK_HACK)&MytessEnd);
	gluTessCallback( mytess, GLU_TESS_ERROR, OF_GLU_CALLBACK_HACK)&MytessError);

	gluTessProperty( mytess , GLU_TESS_WINDING_RULE, OF_POLY_WINDING_ODD);

	gluTessProperty( mytess, GLU_TESS_BOUNDARY_ONLY, false);
	gluTessProperty( mytess, GLU_TESS_TOLERANCE, 0);

	/* ------------------------------------------
	 for 2d, this next call (normal) likely helps speed up ....
	 quote : The computation of the normal represents about 10% of
	 the computation time. For example, if all polygons lie in
	 the x-y plane, you can provide the normal by using the
	 -------------------------------------------  */

	gluTessNormal(mytess, 0.0, 0.0, 1.0);
	gluTessBeginPolygon( mytess, NULL);
}



void Tesselator::vertex(float x, float y) {
	
	double* point = new double[3];
 	point[0] = x;
	point[1] = y;
	point[2] = 0;
 	mypolyVertices.push_back(point);
	coords.push_back(ofPoint(x, y));
}
void Tesselator::bezierVertex(float x1, float y1, float x2, float y2, float x3, float y3) {
	
	// if, and only if poly vertices has points, we can make a bezier
	// from the last point
	
	// the resolultion with which we computer this bezier
	// is arbitrary, can we possibly make it dynamic?
	
	if (mypolyVertices.size() > 0){
		
		float x0 = mypolyVertices[mypolyVertices.size()-1][0];
		float y0 = mypolyVertices[mypolyVertices.size()-1][1];
		
		float   ax, bx, cx;
		float   ay, by, cy;
		float   t, t2, t3;
		float   x, y;
		
		// polynomial coefficients
		cx = 3.0f * (x1 - x0);
		bx = 3.0f * (x2 - x1) - cx;
		ax = x3 - x0 - cx - bx;
		
		cy = 3.0f * (y1 - y0);
		by = 3.0f * (y2 - y1) - cy;
		ay = y3 - y0 - cy - by;
		
		// arbitrary ! can we fix??
		int resolution = 20;
		
		for (int i = 0; i < resolution; i++){
			t 	=  (float)i / (float)(resolution-1);
			t2 = t * t;
			t3 = t2 * t;
			x = (ax * t3) + (bx * t2) + (cx * t) + x0;
			y = (ay * t3) + (by * t2) + (cy * t) + y0;
			vertex(x,y);
		}
		
		
	}
}
void Tesselator::endShape(bool close) {
	
	
	//
	if(close && mypolyVertices.size()>0) {
		
		double* point = new double[3];
		point[0] = mypolyVertices[0][0];
		point[1] = mypolyVertices[0][1];
		point[2] = 0;
		mypolyVertices.push_back(point);
	}
	
	
	currTess = this;
	if ( mytess != NULL){
		gluTessBeginContour( mytess);
		for (int i=0; i<(int)mypolyVertices.size(); i++) {
			gluTessVertex( mytess, mypolyVertices[i],mypolyVertices[i]);
		}
		
		gluTessEndContour( mytess);
		
	}


	if ( mytess != NULL){
		// no matter what we did / do, we need to delete the tesselator object
		gluTessEndPolygon( mytess);
		gluDeleteTess( mytess);
		mytess = NULL;
	}

// now clear the vertices on the dynamically allocated data
	for(vector<double*>::iterator itr=mypolyVertices.begin();
        itr!=mypolyVertices.end();
        ++itr){
        delete [] (*itr);
    }
    mypolyVertices.clear();
	
    // combine callback also makes new vertices, let's clear them:
    for(vector<double*>::iterator itr=mynewVectrices.begin();
        itr!=mynewVectrices.end();
        ++itr){
        delete [] (*itr);
    }
    mynewVectrices.clear();

}

void Tesselator::draw() {
	glEnableClientState(GL_VERTEX_ARRAY);
	for(int i = 0; i < shapes.size(); i++) {
		glVertexPointer(3, GL_FLOAT, 0, &shapes[i].points[0]);
		glDrawArrays(shapes[i].shapeType, 0, shapes[i].points.size()/3);
	}
}

