/*
 *  BitCrusher.h
 *  Thumper-iPhone
 *
 *  Created by Marek Bereza on 12/07/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#define MULTIPLIER 32767.f

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

class BitCrusher {
public:
	BitCrusher() {
		shift = 0;
		out = 0;
		k = 0;
		distortion = 0;
	}
	

	int temp;
	int holdCount;
	float out;
	float process(float in) {
		if(shift>0) {
			holdCount++;
			if(holdCount>shift) {
				holdCount = 0;
				
				temp = (float)in*MULTIPLIER;
				temp = temp >> shift;
				temp = temp << shift;
				out = (float)temp/MULTIPLIER;
				
			}
			return (distortion>0.f) ? (1.f+k)*out/(1.f+k*ABS(out)):out;
		}
		return (distortion>0.f) ? (1.f+k)*in/(1.f+k*ABS(in)):in;
	}
	
	/**
	 * Set amount of effect, between 0 and 1
	 * Set distortion between 0 and 1
	 */
	void setAmount(float amount, float _distortion) {
		int max = 14;
		float a = amount*max;
		shift = a;
		distortion = _distortion;
		k = 2.f*distortion/(1.01-distortion);
		
	}
	float distortion;
	float k;
	/*
	 Posted by Partice Tarrabia and Bram de Jong - musicdsp.org
	 x = input in [-1..1]
	 y = output
	 k = 2*amount/(1-amount);
	 
	 f(x) = (1+k)*x/(1+k*abs(x))
	 */
	float distort(float in) {
		return (distortion>0.f) ? (1.f+k)*out/(1.f+k*ABS(out)):out;
	}
	
	int shift;
};