// adapted from the STL class by Marius Watz (part of unlekkerlib from processing)
// taken from http://workshop.evolutionzone.com/unlekkerlib/ on 06/12/09
// converted to C++ by Marek Bereza

/** Polygon class to support STL geometry. */
#include "ofMain.h"

class STLFace {
public:
	/** Array containing the face normal and the three vertices making up this face.
	 * v[0..2] is the normal, v[3..5] is the first vertex etc. */
	float v[12];
	int cnt;

	STLFace() {
		cnt = 0;
	}

  /////////////////////////////////////////////
  // BUILD FACET VERTEX BY VERTEX

	void vertex(float _x,float _y,float _z) {
		v[cnt*3+3]=_x;
		v[cnt*3+4]=_y;
		v[cnt*3+5]=_z;

		cnt++;
		
		if(cnt==3) { 
			// calculate normal vector
			float x,y,z,vx1,vy1,vz1,vx2,vy2,vz2;
			vx1=v[6]-v[3];
			vy1=v[7]-v[4];
			vz1=v[8]-v[5];
			vx2=v[9]-v[3];
			vy2=v[10]-v[4];
			vz2=v[11]-v[5];

			x=vy1*vz2-vy2*vz1;
			y=vz1*vx2-vz2*vx1;
			z=vx1*vy2-vx2*vy1;

			float l=(float)sqrt(x*x+y*y+z*z);
			x/=l;
			y/=l;
			z/=l;
			v[0]=x;
			v[1]=y;
			v[2]=z;
		}
	}

  /////////////////////////////////////////////
  // READ FROM FILE

	void parseFace(char *buf) {
		memcpy(v, buf, 12*sizeof(float));
	}

  /////////////////////////////////////////////
  // TRANSFORMS

	void translate(float tx,float ty,float tz) {
		for(int i=0; i<3; i++) {
			v[3+i*3]+=tx;
			v[3+i*3+1]+=ty;
			v[3+i*3+2]+=tz;
		}
	}

	void scale(float tx,float ty,float tz) {
		for(int i=0; i<3; i++) {
			v[3+i*3]*=tx;
			v[3+i*3+1]*=ty;
			v[3+i*3+2]*=tz;
		}
	}
	
  /////////////////////////////////////////////
  // UTILITY FUNCTIONS

//	void write(ByteBuffer buf) throws IOException {
//		for(int i=0; i<12; i++) buf.putFloat(v[i]);
//	}

	char *getData() {
		return (char*) v;
	}
	string toString() {
		string s = "n  ["+ofToString(v[0], 5)+","+ofToString(v[1], 5)+","+ofToString(v[2], 5)+"]";
		s+="\nv1 ["+ofToString(v[3], 5)+","+ofToString(v[4], 5)+","+ofToString(v[4], 5)+"]";
		s+="\nv2 ["+ofToString(v[6], 5)+","+ofToString(v[7], 5)+","+ofToString(v[8], 5)+"]";
		s+="\nv3 ["+ofToString(v[9], 5)+","+ofToString(v[10], 5)+","+ofToString(v[11], 5)+"]";
		return s;
	}
};
