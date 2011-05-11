/*
 *  PannableSample.h
 *  AudioTest
 *
 *  Created by Marek Bereza on 31/08/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "Sample.h"
#define BUFF_SIZE 8192
class MixableSample: public Sample {
public:
	
	float buffer[BUFF_SIZE];
	
	MixableSample(): Sample() {
		targetPan = pan = 0.5;
		targetVolume = volume = 1.0;
	}
	
	/** Goes from 0 to 1 for left to right */
	void setPan(float _pan) {
		targetPan = _pan;
		if(targetPan<0) targetPan = 0;
		if(targetPan>1) targetPan = 1;
	}
	
	void setVolume(float vol) {
		targetVolume = vol;
	}
	
	void getSamplesStereo(float *samples, int length) {
		
		if(length>BUFF_SIZE) {
			printf("Buffer not big enough!!\n");
			memset(samples, 0, length*2*sizeof(float));
			return;
		}
		
		// get the samples to work on
		getSamples(buffer, length);
		float lerpAmt = 0.05;
		
		if(getNumChannels()==1) {
			for(int i = 0; i < length; i++) {
				
				volume = volume*(1.f-lerpAmt) + targetVolume*lerpAmt;
				pan = pan*(1.f-lerpAmt) + targetPan*lerpAmt;				
				
				samples[i*2] = buffer[i]*(1 - pan)*volume;
				samples[i*2 + 1] = buffer[i]*pan*volume;
			}
		} else if(getNumChannels()==2) {
			for(int i = 0; i < length; i++) {
				
				volume = volume*(1.f-lerpAmt) + targetVolume*lerpAmt;
				pan = pan*(1.f-lerpAmt) + targetPan*lerpAmt;
				
				samples[i*2] = buffer[i*2]*(1 - pan)*volume;
				samples[i*2 + 1] = buffer[i*2+1]*pan*volume;
			}
		}
		
	}
	
private:
	float pan;
	float volume;
	float targetPan;
	float targetVolume;
};