#include "testApp.h"

bool exploding = false;
int counter = 0;
//--------------------------------------------------------------
void testApp::setup(){
	model.read("torus.stl");
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };
	
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	ofEnableSmoothing();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);


}

//--------------------------------------------------------------
void testApp::update(){
	if(exploding) {
		for(int i = 0; i < model.triangles.size(); i++) {
			model.triangles[i].translate(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1));
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	glPushMatrix();
	glTranslatef(ofGetWidth()/2,ofGetHeight()/2, 0);
	glScalef(10,10,10);
	glRotatef(ofGetFrameNum(), 1,1,0);
	model.draw();
	glPopMatrix();
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 4)+ " FPS\nClick to explode the 3d model, press 's' to save out an STL file of the current geometry, press 'r' to reset the model", 10, ofGetHeight()-20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {
		case 's': model.write(ofToString(counter++) + ".stl"); break;
		case 'r': 
			exploding = false;
			model.read("torus.stl"); 
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	exploding = true;
	

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

