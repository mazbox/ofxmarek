/**
 * Clipping.h
 *
 * Created by Marek Bereza on 02/04/2011.
 *
 */

#ifndef CLIP
#define CLIP(A) (A>1?1:A<-1?-1:A)
#endif

float niceClipper(float a);