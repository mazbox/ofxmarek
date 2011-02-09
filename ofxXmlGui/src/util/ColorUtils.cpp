/*
 *  ColorUtils.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *
 */

#include "ColorUtils.h"

int blendColor(int color1, int color2, float amt) {
	float r = hexValR(color1)*amt + hexValR(color2)*(1.f-amt);
	float g = hexValG(color1)*amt + hexValG(color2)*(1.f-amt);
	float b = hexValB(color1)*amt + hexValB(color2)*(1.f-amt);
	
	return colorFloat255ToHex(r, g, b);
}

int colorFloat255ToHex(float r, float g, float b) {
	int rr = r;
	int gg = g;
	int bb = b;
	return (rr << 16) + (gg << 8) + bb;
}