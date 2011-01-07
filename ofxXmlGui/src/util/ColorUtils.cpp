/*
 *  ColorUtils.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ColorUtils.h"

int blendColor(int color1, int color2, float amt) {
	float r = R(color1)*amt + R(color2)*(1.f-amt);
	float g = G(color1)*amt + G(color2)*(1.f-amt);
	float b = B(color1)*amt + B(color2)*(1.f-amt);
	
	return colorFloat255ToHex(r, g, b);
}

int colorFloat255ToHex(float r, float g, float b) {
	int rr = r;
	int gg = g;
	int bb = b;
	return (rr << 16) + (gg << 8) + bb;
}