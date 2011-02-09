/*
 *  ColorUtils.h
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *
 */

// gets the R, G or B component out of a hex colour
#define hexValR(A)  ((A >> 16) & 0xff)
#define hexValG(A) ((A >> 8) & 0xff)
#define hexValB(A) ((A >> 0) & 0xff)

int blendColor(int color1, int color2, float amt = 0.5);


int colorFloat255ToHex(float r, float g, float b);