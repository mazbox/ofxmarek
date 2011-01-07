/*
 *  FlowField.h
 *  FordSMax
 *
 *  Created by Marek Bereza on 26/07/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#pragma once
struct fieldpoint {
    float x;
    float y;
};


/**
 * \brief basic flow field
 */
class FlowField {
    
public:

	FlowField() {
		field = NULL;
	}
	
	/**
	 * set this up with dimensions
	 */
    void setup(int x, int y) {
		multiplier = 1000.f;
		dampening = 0.99;
		dissipation = 0.5;
        w = x;
        h = y;
        field = new fieldpoint*[w];
		
        for(int i = 0; i < w; i++) {
			field[i] = new fieldpoint[h];
            for(int j = 0; j < h; j++) {
                field[i][j].x = 0;
				field[i][j].y = 0;
                
            }
        }
    }
	
	/**
	 * Blurs the flow field to dissipate the forces
	 */
    void update() {
        // blur here
        if(field==NULL) return;
        for(int x = 1; x < w-1; x++) {
            for(int y = 1; y < h-1; y++) {
                
                field[x][y].x = dampening * (field[x-1][y].x + field[x][y].x + field[x+1][y].x + field[x][y-1].x + field[x][y+1].x)/5;
                field[x][y].y = dampening * (field[x-1][y].y + field[x][y].y + field[x+1][y].y + field[x][y-1].y + field[x][y+1].y)/5;
            }
        }
        for(int x = 0; x < w; x++) {
            field[x][0].x = 0;
            field[x][0].y = 0;
            field[x][h-1].x = 0;
            field[x][h-1].y = 0;
        }
        
        for(int y = 0; y < h; y++) {
            field[0][y].x = 0;
            field[w-1][y].x = 0;
            field[0][y].y = 0;
            field[w-1][y].y = 0;
        }
    }
	
	
    
	/**
	 * Adds a force at a point in the flow field. Coordinates are normalized
	 */
    void addForce(float x, float y, float dx, float dy) {
		if (field == NULL) return;
        int xx = floor(x*(float)w);
        int yy = floor(y*(float)h);
        if(xx<0 || yy < 0 || xx>=w || yy>=h) return;

        field[xx][yy].x += dx*multiplier;
        field[xx][yy].y += dy*multiplier;
    }
    
	void draw() {
		draw(0, 0, w, h);
	}
	
	/**
	 * Gets a force at a point (normalized)
	 */
	fieldpoint forceAtPos(float x, float y) {
		fieldpoint p;
		p.x = 0;
		p.y = 0;
		int xx = floor(x*(float)w);
        int yy = floor(y*(float)h);
        if(xx<0 || yy < 0 || xx>=w || yy>=h) return p;

		return field[xx][yy];
	}
    
	void draw(float x, float y, float width, float height) {
		if(field==NULL) return;
		glColor3f(1, 1, 1);
		glPushMatrix();
		{
			glTranslatef(x, y, 0);
			glScalef(width/(float)w, height/(float)h, 1);
			
			glBegin(GL_LINES);
			for(int i = 0; i < w; i++) {
				for(int j = 0; j < h; j++) {
					glVertex2f(i, j);
					glVertex2f(i+field[i][j].x, j+field[i][j].y);
				}
			}
			glEnd();
		}
		glPopMatrix();
    }
	
private:
    float multiplier;
    float dampening;
	float dissipation;
    int w;
    int h;	
	
    fieldpoint **field;
	
};