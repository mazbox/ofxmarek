/*
 *  ColorUtils.h
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

// gets the R, G or B component out of a hex colour
#define R(A)  ((A >> 16) & 0xff)
#define G(A) ((A >> 8) & 0xff)
#define B(A) ((A >> 0) & 0xff)

int blendColor(int color1, int color2, float amt = 0.5);


int colorFloat255ToHex(float r, float g, float b);