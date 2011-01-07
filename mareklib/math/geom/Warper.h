/*
 *  Warper.h
 *  FordSMax
 *
 *  Converted from some csharp script
 *  http://code.google.com/p/wiimotetuio/source/browse/trunk/WiimoteTUIO/Warper.cs
 *
 *  Warp from one quad to another
 *  
 *  Usage:
 *
 *  // setup
 *  Warper w;
 *  w.setSource(<sourceQuad>);
 *  w.setDestination(<destinationQuad>);
 *  w.computeWarp();
 *
 *  // warp a point
 *  w.warp(inX, inY, &outX, &outY);
 *
 *  <sourceQuad> and <destinationQuad> are specified 
 * top left, then top right, then bottom left, then bottom right. 
 *
 *  Created by Marek Bereza on 12/08/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */


class Warper {
public:
	float srcX[4];
	float srcY[4];
	float dstX[4];
	float dstY[4];
	float srcMat[16]; 
	float dstMat[16]; 
	float warpMat[16];
	bool dirty;
	
	Warper() {
		setIdentity();
	}
	
	void setIdentity() {
		setSource(0.0f, 0.0f,
				  1.0f, 0.0f,
				  0.0f, 1.0f,
				  1.0f, 1.0f);
		setDestination(0.0f, 0.0f,
					   1.0f, 0.0f,
					   0.0f, 1.0f,
					   1.0f, 1.0f);
		computeWarp();
	}
	
	void setSource(	float x0,
				   float y0, 
				   float x1, 
				   float y1, 
				   float x2,
				   float y2, 
				   float x3, 
				   float y3){
		srcX[0] = x0;
		srcY[0] = y0;
		srcX[1] = x1;
		srcY[1] = y1;
		srcX[2] = x2;
		srcY[2] = y2;
		srcX[3] = x3;
		srcY[3] = y3;
		dirty = true;
	}
	
	void setDestination(float x0,
						float y0, 
						float x1, 
						float y1, 
						float x2,
						float y2, 
						float x3, 
						float y3){
		dstX[0] = x0;
		dstY[0] = y0;
		dstX[1] = x1;
		dstY[1] = y1;
		dstX[2] = x2;
		dstY[2] = y2;
		dstX[3] = x3;
		dstY[3] = y3;
		dirty = true;
	}
	
	
	void computeWarp() {
		computeQuadToSquare(	srcX[0],srcY[0],
							srcX[1],srcY[1],
							srcX[2],srcY[2],
							srcX[3],srcY[3],
							srcMat);
		computeSquareToQuad(	dstX[0], dstY[0],
							dstX[1], dstY[1],
							dstX[2], dstY[2],
							dstX[3], dstY[3],
							dstMat);
		
		
		multMats(srcMat, dstMat, warpMat);
		dirty = false;
	}
	
	void multMats(float *srcMat, float *dstMat, float *resMat) {
		// DSTDO/CBB: could be faster, but not called often enough to matter
		for (int r = 0; r < 4; r++) {
			int ri = r * 4;
			for (int c = 0; c < 4; c++) {
				resMat[ri + c] = (srcMat[ri    ] * dstMat[c     ] +
								  srcMat[ri + 1] * dstMat[c +  4] +
								  srcMat[ri + 2] * dstMat[c +  8] +
								  srcMat[ri + 3] * dstMat[c + 12]);
			}
		}
	}
	
	void computeSquareToQuad(	float x0,
							 float y0, 
							 float x1, 
							 float y1, 
							 float x2,
							 float y2, 
							 float x3, 
							 float y3,
							 float *mat) {
		
		float dx1 = x1 - x2, 	dy1 = y1 - y2;
		float dx2 = x3 - x2, 	dy2 = y3 - y2;
		float sx = x0 - x1 + x2 - x3;
		float sy = y0 - y1 + y2 - y3;
		float g = (sx * dy2 - dx2 * sy) / (dx1 * dy2 - dx2 * dy1);
		float h = (dx1 * sy - sx * dy1) / (dx1 * dy2 - dx2 * dy1);
		float a = x1 - x0 + g * x1;
		float b = x3 - x0 + h * x3;
		float c = x0;
		float d = y1 - y0 + g * y1;
		float e = y3 - y0 + h * y3;
		float f = y0;
		
		mat[ 0] = a;	mat[ 1] = d;	mat[ 2] = 0;	mat[ 3] = g;
		mat[ 4] = b;	mat[ 5] = e;	mat[ 6] = 0;	mat[ 7] = h;
		mat[ 8] = 0;	mat[ 9] = 0;	mat[10] = 1;	mat[11] = 0;
		mat[12] = c;	mat[13] = f;	mat[14] = 0;	mat[15] = 1;
	}
	
	void computeQuadToSquare(	float x0,
							 float y0, 
							 float x1, 
							 float y1, 
							 float x2,
							 float y2, 
							 float x3, 
							 float y3,
							 float *mat) {
		computeSquareToQuad(x0,y0,x1,y1,x2,y2,x3,y3, mat);
		
		// invert through adjoint
		
		float a = mat[ 0],	d = mat[ 1],	/* ignore */		g = mat[ 3];
		float b = mat[ 4],	e = mat[ 5],	/* 3rd col*/		h = mat[ 7];
		/* ignore 3rd row */
		float c = mat[12],	f = mat[13];
		
		float A =     e - f * h;
		float B = c * h - b;
		float C = b * f - c * e;
		float D = f * g - d;
		float E =     a - c * g;
		float F = c * d - a * f;
		float G = d * h - e * g;
		float H = b * g - a * h;
		float I = a * e - b * d;
		
		// Probably unnecessary since 'I' is also scaled by the determinant,
		//   and 'I' scales the homogeneous coordinate, which, in turn,
		//   scales the X,Y coordinates.
		// Determinant  =   a * (e - f * h) + b * (f * g - d) + c * (d * h - e * g);
		float idet = 1.0f / (a * A           + b * D           + c * G);
		
		mat[ 0] = A * idet;	mat[ 1] = D * idet;	mat[ 2] = 0;	mat[ 3] = G * idet;
		mat[ 4] = B * idet;	mat[ 5] = E * idet;	mat[ 6] = 0;	mat[ 7] = H * idet;
		mat[ 8] = 0       ;	mat[ 9] = 0       ;	mat[10] = 1;	mat[11] = 0       ;
		mat[12] = C * idet;	mat[13] = F * idet;	mat[14] = 0;	mat[15] = I * idet;
	}
	
	float *getWarpMatrix()
	{
		return warpMat;
	}
	
	void warp(float srcX, float srcY, float *dstX, float *dstY)
	{
		if (dirty)
			computeWarp();
		warp(warpMat, srcX, srcY, dstX, dstY);
	}
	
	
	void warp(float *mat, float srcX, float srcY, float *dstX, float *dstY) {
		float result[4];
		float z = 0;
		result[0] = (float)(srcX * mat[0] + srcY*mat[4] + z*mat[8] + 1*mat[12]);
		result[1] = (float)(srcX * mat[1] + srcY*mat[5] + z*mat[9] + 1*mat[13]);
		result[2] = (float)(srcX * mat[2] + srcY*mat[6] + z*mat[10] + 1*mat[14]);
		result[3] = (float)(srcX * mat[3] + srcY*mat[7] + z*mat[11] + 1*mat[15]);        
		*dstX = result[0]/result[3];
		*dstY = result[1]/result[3];
		
	}
};