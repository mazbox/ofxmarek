/*
 *  Delay.h
 *  audioInputExample
 *
 *  Created by Marek Bereza on 26/09/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#define DELAYTIME 22050
#include "Clipping.h"

class StereoDelay {
public:
	StereoDelay() {
		memset(delayL, 0, DELAYTIME*sizeof(float));
		memset(delayR, 0, DELAYTIME*sizeof(float));
		posL = 1;
		posR = 1;
		delayTimeL = DELAYTIME/4;
		delayTimeR = DELAYTIME/4.05;
		feedbackL = 0.7;
		feedbackR = 0.7;
		mixL = 0.5;
		mixR = 0.5;
	}
	
	int getMaxDelayTime() {
		return DELAYTIME;
	}
	
	void setDelayTimes(int l, int r) {
		if(l>DELAYTIME) l = DELAYTIME;
		if(r>DELAYTIME) r = DELAYTIME;
		this->delayTimeL = l;
		this->delayTimeR = r;
	}
	void setFeedbacks(float feedbackL, float feedbackR) {
		this->feedbackL = feedbackL;
		this->feedbackR = feedbackR;
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
			
			
			delayL[posL] = feedbackL*delayL[posL]+audio[i*2];
			delayR[posR] = feedbackR*delayR[posR]+audio[i*2+1];
			
			
			
			audio[i*2  ] += (outL - audio[i*2])*mixL;
			audio[i*2+1] += (outR - audio[i*2+1])*mixR;			
		}
	}
	
	int delayTimeL;
	int delayTimeR;
	float feedbackL;
	float feedbackR;
	float mixL;
	float mixR;
					   
	int posL;
	int posR;
	float delayL[DELAYTIME];
	float delayR[DELAYTIME];
};