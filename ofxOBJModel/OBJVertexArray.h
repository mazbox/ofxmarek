/*
 *  STLVertexArray.h
 *  Blob
 *
 *  Created by Marek Bereza on 09/08/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include <map>

class OBJVertexArray {
public:
	
	
	bool replaceAll( const string &original, string
					&finalString, const string & sourceString, const string & targetString )
	{
		finalString = original;
		int len = sourceString.size();
		bool changesMade=false;
		
		int pos=0;
		while( pos != -1)
		{
			pos = finalString.find( sourceString, pos );
			
			if ( pos != -1 )
			{
				finalString.erase( pos, len );
				if (targetString.size() > 0 )
					finalString.insert( pos, targetString );
				changesMade=true;
			}
			
		}
		return changesMade;
	}

	
	/**
	 * If bCalculateFlatNormals is set to true, then this
	 * class also calculates normals per triangle and
	 * when drawing with drawFlat() will draw an array of triangles
	 * rather than indices.
	 */
	bool load(string path, bool bCalculateFlatNormals = true) {

		string filePath = path;
		path = ofToDataPath(path, true);
		
		string line;
		
		points.clear();
		normals.clear();
		indices.clear();
		
		// obj file format vertexes are 1-indexed
		points.push_back(ofPoint());
		
		ifstream myfile (path.c_str());
		
		
		if (myfile.is_open()) {
			string contents = "";
			while (! myfile.eof()) {
				getline (myfile,line);
				contents += line + "\n";
			}
			myfile.close();
			
			// remove any soft returns
			string out = "";
			string newline = "\n";
			if(contents.find("\r\n")) {
				newline = "\r\n";			
			} else if(contents.find("\r")) {
				newline = "\r";
			} else if(contents.find("\n")) {
				newline = "\n";
			}
			replaceAll(contents, out, " \\"+newline, "");
			
			contents = out;
			
			
			// parse line-by-line
			vector<string> lines = ofSplitString(contents, newline);
			
			
			
			
			for(int i = 0; i < lines.size(); i++) {
				
				string line = lines[i];
				
				if(line.find("v ")==0) { // new vertex
					points.push_back(parseVertex(line));
				} else if(line.find("vn ")==0) { // new vertex
					ofxVec3f n = parseVertex(line);
					n.normalize();
					//n.set(0);
					normals.push_back(n);
				} else if(line.find("f ")==0) { // face definition
					
					
					
					line = line.substr(2); // lop off "f "
					vector<string> ind = ofSplitString(line, " ");
					// remove any texcoords (/xxx's)
					
					
					for(int i = 0; i < ind.size(); i++) {
						
						int slashPos = ind[i].find("/");
						if(slashPos!=-1) {
							int vertexIndex = ofToInt(ind[i].substr(0, slashPos));
							indices.push_back(vertexIndex);
						}
					}
				}
			}
			
			printf("Successfully loaded %s\n-----\nVertices: %d\nIndices: %d\n", path.c_str(), (int)points.size(), (int)indices.size());
			
			if(bCalculateFlatNormals) {
				calculateFlatNormals();
			}
			//calculateSmoothNormals();
			return true;
		} else {
			printf("Couldn't find the OBJ file %s\n", path.c_str());
			return false;
		}
	}
	
	

	void calculateFlatNormals() {
		// loop through each triangle
		for(int i = 0; i < indices.size(); i += 3) {
			
			ofxVec3f a = points[indices[i]];
			ofxVec3f b = points[indices[i+1]];
			ofxVec3f c = points[indices[i+2]];
			
			// calculate the normal for the triangle
			ofxVec3f normal = (a-b).getCrossed(c-b);
			normal.normalize();
			normal *= -1;
			flatNormals.push_back(normal);
			flatNormals.push_back(normal);
			flatNormals.push_back(normal);
			
			// save the points
			flatPoints.push_back(a);
			flatPoints.push_back(points[indices[i+1]]);
			flatPoints.push_back(points[indices[i+2]]);
		}
	}
	
	
	
	void calculateSmoothNormals() {
		calculateFlatNormals();
		int mode = 1;
		
		
		int Vertices;
		int Surfaces;
		
		int i,p1,p2,p3;
		
		//CSurface *Surface;
		ofxVec3f N,N1,N2,N3;
		ofxVec3f AvgNormal;
		
		int SurfaceIndex;
		int count;
		
		multimap<int, int> Hash;
		pair<multimap<int, int>::iterator, 
		multimap<int, int>::iterator> ht;   
		multimap<int, int>::iterator it;
		
		Surfaces = indices.size()/3;
		Vertices = points.size();
		
		switch(mode) {
				
			case 0 :
				
				for ( i=0; i<Vertices; i++ ) normals[i] = ofxVec3f(0, 0, 0);
				
				////////////////////////////
				// classic computation
				
				for ( i=0; i<Surfaces; i++ ) {
					
					
//					Surface = Mesh->SurfaceList [ i ];
					
					
					// indices of triangles
					p1 = i*3 + 0;
					p2 = i*3 + 1;
					p3 = i*3 + 2;
					
					// take any of the normals for this surface
					N=flatNormals[p1];
					
					normals[p1] += N;
					normals[p2] += N;
					normals[p3] += N;
				}
				
				///////////////////////////////////////
				// load vertex normal values
				
				for ( i=0; i<Vertices; i++ ) normals[i].normalize();

				
				break;
				
							
			case 1 :
				
				///////////////////////////////
				// normal computation taking
				// account of shared vertices
				
				
				for ( i = 0; i < Surfaces; i++)
				{
					p1 = indices[i*3 + 0];
					p2 = indices[i*3 + 1];
					p3 = indices[i*3 + 2];
					
					
					std::pair<int, int>  pp1 ( p1,i);
					std::pair<int, int>  pp2 ( p2,i);
					std::pair<int, int>  pp3 ( p3,i);
					
					Hash.insert( pp1 );
					Hash.insert( pp2 );
					Hash.insert( pp3 );
				}
				
				/////////////////////////////////////
				// zeroing normals
				
				for ( i=0; i<Vertices; i++ )
					normals[i] = ofxVec3f(0, 0, 0);
				
				for ( i = 0; i < Vertices; i++)
				{
					
					ht = Hash.equal_range( i );
					
					N.set(0,0,0);
					
					count = 0;
					
					for ( it  = ht.first; it != ht.second; ++it ) 
					{
						SurfaceIndex=(*it).second;
						N += flatNormals[SurfaceIndex/3];
						count++;
					}
					
					N /= count;
					N.normalize();
					
					normals[i] = N;
				}
				
				break;
				
				
		};
	}
	
	
	
	
	void draw() {
		drawSmooth();
	}
	
	void drawFlat() {
		
		

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
			
		glVertexPointer( 3, GL_FLOAT, sizeof(ofxVec3f), &flatPoints[0].x );
		glNormalPointer(GL_FLOAT, sizeof(ofxVec3f), &flatNormals[0].x );

		glDrawArrays(GL_TRIANGLES, 0, flatPoints.size());
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
	}
	
	void drawSmooth() {
		glEnable(GL_DEPTH_TEST);
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, sizeof(ofxVec3f), &points[0].x );
		
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer(GL_FLOAT, sizeof(ofxVec3f), &normals[0].x );
		
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		
		
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		
		glDisable(GL_DEPTH_TEST);
		
	}
	
	
	/**
	 
	 
	 * Takes a line from the obj file beginning with a "v " and 
	 * turns it into an ofPoint.
	 */
	ofPoint parseVertex(string line) {
		ofPoint p;
		if(line.find("v ")!=0 && line.find("vn ")!=0) {
			printf("Error, line does not have vertex info in it: \"%s\"\n", line.c_str());
			return p;
		}
		if(line.find("v ")==0) {
			line = line.substr(2);
		} else {
			line = line.substr(3);
		}
		
		vector<string> elements = ofSplitString(line, " ");
		if(elements.size()!=3) {
			printf("Error line does not have 3 coordinates: \"%s\"\n", line.c_str());
			return p;
		}
		
		p.x = atof(elements[0].c_str());
		p.y = atof(elements[1].c_str());
		p.z = atof(elements[2].c_str());
		
		return p;
		
	}

	
	
	vector<ofxVec3f> flatPoints;
	vector<ofxVec3f> flatNormals;
	
	vector<ofxVec3f> points;
	vector<ofxVec3f> normals;
	vector<unsigned int> indices;
};