/**
 * OpticalFlow.h
 * Backdrop
 *
 * Mostly taken from ofxCvOpticalFlowLK
 *
 * Created by Marek Bereza on 28/06/2011.
 *
 */
#include "ofxOpenCv.h"

class OpticalFlow: public ofBaseDraws {
public:
	
	OpticalFlow() {
		captureColsStep = 4;
		captureRowsStep = 4;
	}
	
	~OpticalFlow() {
		cvReleaseImage(&vel_x);
		cvReleaseImage(&vel_y);
	}
	
	void setup(int width, int height) {
		this->width = width;
		this->height = height;
		
		vel_x = cvCreateImage( cvSize( width ,height ), IPL_DEPTH_32F, 1  );
		vel_y = cvCreateImage( cvSize( width ,height ), IPL_DEPTH_32F, 1  );

		cvSetZero(vel_x);
		cvSetZero(vel_y);
		
		greyCurr.allocate(width, height);
		greyPrev.allocate(width, height);
	}
	
	
	
	void setCalcStep(int _cols, int _rows) {
		captureColsStep = _cols;
		captureRowsStep = _rows;
	}
	
	
	
	
	
	float getTotalMovement() {
		int x, y;
		float dx = 0, dy = 0;
		for ( y = 0; y < height; y+=captureRowsStep ){
			for ( x = 0; x < width; x+=captureColsStep ){
				
				dx += ABS(cvGetReal2D( vel_x, y, x ));
				dy += ABS(cvGetReal2D( vel_y, y, x ));
			}
		}
		return dx+dy;
	}
	
	void draw() {
		ofSetHexColor(0xffffff);
		ofNoFill();
		float speed;
		
		int x, y, dx, dy;
		for ( y = 0; y < height; y+=captureRowsStep ){
			for ( x = 0; x < width; x+=captureColsStep ){
				
				dx = (int)cvGetReal2D( vel_x, y, x );
				dy = (int)cvGetReal2D( vel_y, y, x );
				//speed = dx * dx + dy * dy;
				ofLine(x, y, x+dx, y+dy);
				
			}
		}
	}
	
	void draw(float x,float y) {
		/*glPushMatrix();
		glTranslatef(x, y, 0);
		draw();
		glPopMatrix();*/
	}
	
	void draw(float x,float y,float w, float h) {
		glPushMatrix();
		glTranslatef(x, y, 1);
		glScalef(w/(float)width, h/(float)width, 1);
		draw();
//		greyPrev.draw(0, 0, width, height);
		glPopMatrix();
	}
	
	
	void forceAtPosNormalized(ofVec2f in, ofVec2f &out) {
		if(in.x>=0 && in.x<=1 && in.y>=0 &&in.y<=1) {
			in *= ofVec2f(width, height);
		
			out.x = cvGetReal2D( vel_x, in.y, in.x );
			out.y = cvGetReal2D( vel_y, in.y, in.x );
		//} else {
		//	printf("OpticalFlow: (%f,%f) out of range!\n", in.x, in.y);
		}
	}
	
	
	
	
	int width;
	int height;
	
	IplImage* vel_x;
	IplImage* vel_y;
	float getWidth() { return width; }
	float getHeight() { return height; }
	
	
	
	int captureColsStep;
	int captureRowsStep;
	
	void blur(int flowBlur) {
		// smooth it
		cvSmooth(vel_x, vel_x, CV_BLUR , flowBlur*2+1);
		cvSmooth(vel_y, vel_y, CV_BLUR , flowBlur*2+1);
	}
	
	
	
	
	void process(ofxCvGrayscaleImage &img, int size = 3) {
		// if it's the same res, just use the passed in image
		if(img.getWidth()==greyCurr.getWidth() && img.getHeight()==greyCurr.getHeight()) {
			cvCalcOpticalFlowLK( greyPrev.getCvImage(), img.getCvImage(),	cvSize( size, size), vel_x, vel_y );
			greyPrev = img;
		} else {
			// otherwise, rescale and perfrom optical flow
			greyCurr.scaleIntoMe(img);
			cvCalcOpticalFlowLK( greyPrev.getCvImage(), greyCurr.getCvImage(),	cvSize( size, size), vel_x, vel_y );
			greyPrev = greyCurr;
		}
	}
private:
	ofxCvGrayscaleImage greyCurr;
	ofxCvGrayscaleImage greyPrev;
};