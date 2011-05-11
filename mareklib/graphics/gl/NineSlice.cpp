/**
 * NineSlice.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 05/04/2011.
 *
 */

#include "NineSlice.h"

void NineSlice::setup(ofImage *img) {
	this->img = img;
	// row 1
	texCoords[0] = ofVec2f(0,0);
	texCoords[1] = ofVec2f(0,0.5);
	texCoords[2] = ofVec2f(0.5, 0);
	texCoords[3] = ofVec2f(0.5, 0.5);
	texCoords[4] = ofVec2f(0.5, 0);
	texCoords[5] = ofVec2f(0.5, 0.5);
	texCoords[6] = ofVec2f(1, 0);
	texCoords[7] = ofVec2f(1, 0.5);
	
	// row 2
	texCoords[8] = ofVec2f(0,0.5);
	texCoords[9] = ofVec2f(0,0.5);
	texCoords[10] = ofVec2f(0.5, 0.5);
	texCoords[11] = ofVec2f(0.5, 0.5);
	texCoords[12] = ofVec2f(0.5, 0.5);
	texCoords[13] = ofVec2f(0.5, 0.5);
	texCoords[14] = ofVec2f(1, 0.5);
	texCoords[15] = ofVec2f(1, 0.5);
	
	// row 3
	texCoords[16] = ofVec2f(0,0.5);
	texCoords[17] = ofVec2f(0,1);
	texCoords[18] = ofVec2f(0.5, 0.5);
	texCoords[19] = ofVec2f(0.5, 1);
	texCoords[20] = ofVec2f(0.5, 0.5);
	texCoords[21] = ofVec2f(0.5, 1);
	texCoords[22] = ofVec2f(1, 0.5);
	texCoords[23] = ofVec2f(1, 1);
}

void NineSlice::draw(float x, float y, float width, float height) {
	ofRectangle r = ofRectangle(x, y, width, height);
	draw(r);
}

void NineSlice::draw(ofRectangle &rect) {
	if(rect.width!=this->rect.width || rect.height!=this->rect.height) {
		setRect(rect);
		draw();
	} else if(rect.x==this->rect.width && rect.height==this->rect.height) {
		draw();
	} else {
		draw(rect.x, rect.y);
	}
	
}

void NineSlice::draw(float x, float y) {
	float offsetX = -rect.x + x;
	float offsetY = -rect.y + y;
	glPushMatrix();
	glTranslatef(offsetX, offsetY, 0);
	draw();
	glPopMatrix();
}
void NineSlice::draw() {
	img->getTextureReference().bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	for(int i = 0; i < 3; i++) {
		glVertexPointer(2, GL_FLOAT, 0, &vertices[i*8]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[i*8]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	img->getTextureReference().unbind();
}

void NineSlice::setRect(ofRectangle &rect) {
	
	this->rect = rect;
	
	float w = img->getWidth();
	float h = img->getHeight();
	float w2 = w/2;
	float h2 = h/2;
	
	// row 1
	float top = rect.y;
	float bot = rect.y+h2;
	vertices[0] = ofVec2f(rect.x, top);
	vertices[1] = ofVec2f(rect.x, bot);
	vertices[2] = ofVec2f(rect.x+w2, top);
	vertices[3] = ofVec2f(rect.x+w2, bot);
	vertices[4] = ofVec2f(rect.x+rect.width-w2, top);
	vertices[5] = ofVec2f(rect.x+rect.width-w2, bot);
	vertices[6] = ofVec2f(rect.x+rect.width, top);
	vertices[7] = ofVec2f(rect.x+rect.width, bot);

	// row 2
	top = bot;
	bot = rect.y+rect.height-h2;
	vertices[8] = ofVec2f(rect.x, top);
	vertices[9] = ofVec2f(rect.x, bot);
	vertices[10] = ofVec2f(rect.x+w2, top);
	vertices[11] = ofVec2f(rect.x+w2, bot);
	vertices[12] = ofVec2f(rect.x+rect.width-w2, top);
	vertices[13] = ofVec2f(rect.x+rect.width-w2, bot);
	vertices[14] = ofVec2f(rect.x+rect.width, top);
	vertices[15] = ofVec2f(rect.x+rect.width, bot);
	
	// row 1
	top = bot;
	bot = rect.y+rect.height;
	vertices[16] = ofVec2f(rect.x, top);
	vertices[17] = ofVec2f(rect.x, bot);
	vertices[18] = ofVec2f(rect.x+w2, top);
	vertices[19] = ofVec2f(rect.x+w2, bot);
	vertices[20] = ofVec2f(rect.x+rect.width-w2, top);
	vertices[21] = ofVec2f(rect.x+rect.width-w2, bot);
	vertices[22] = ofVec2f(rect.x+rect.width, top);
	vertices[23] = ofVec2f(rect.x+rect.width, bot);
	
	//for(int i = 0; i < 24; i++) vertices[i] = ofVec2f(ofRandomWidth(), ofRandomHeight());
}
