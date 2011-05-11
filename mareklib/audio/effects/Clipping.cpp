/**
 * Clipping.cpp
 *
 * Created by Marek Bereza on 02/04/2011.
 *
 */

#include "Clipping.h"
float niceClipper(float a) {
	return (a - (a*a*a)/3.f)*1.5;
	
}
