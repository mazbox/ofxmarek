/**
 * StereoDoubler.h
 *
 * Like a delay, but only does one repeat of the signal.
 * Wetness control is simply how loud the repeat is (0-1)
 *
 * Created by Marek Bereza on 02/04/2011.
 *
 */
#define DOUBLERTIME 22050
#include "Clipping.h"

class StereoDoubler {
public:
	
	StereoDoubler() {
		memset(delayL, 0, DOUBLERTIME*sizeof(float));
		memset(delayR, 0, DOUBLERTIME*sizeof(float));
		posL = 1;
		posR = 1;
		delayTimeL = DOUBLERTIME/4;
		delayTimeR = DOUBLERTIME/4.05;
		
		wetL = 0.7;
		wetR = 0.7;
	}
	
	int getMaxDelayTime() {
		return DOUBLERTIME;
	}
	
	void setDelayTimes(int l, int r) {
		if(l>DOUBLERTIME) l = DOUBLERTIME;
		if(r>DOUBLERTIME) r = DOUBLERTIME;
		this->delayTimeL = l;
		this->delayTimeR = r;
	}
	
	void setWetness(float l, float r) {
		wetL = l;
		wetR = r;
	}
	
	
	
	void process(float *audio, int length) {
		
		float outL, outR;
		for(int i = 0; i < length; i++) {
			
			posL++;
			posR++;
			
			posL %= delayTimeL;
			posR %= delayTimeR;
			
			outL = delayL[posL];
			outR = delayR[posR];
			
			delayL[posL] = audio[i*2];
			delayR[posR] = audio[i*2+1];
			
			audio[i*2  ] += outL*wetL;
			audio[i*2+1] += outR*wetR;			
		}
	}
	
	int delayTimeL;
	int delayTimeR;
	
	float wetL;
	float wetR;
	
	int posL;
	int posR;
	float delayL[DOUBLERTIME];
	float delayR[DOUBLERTIME];
};