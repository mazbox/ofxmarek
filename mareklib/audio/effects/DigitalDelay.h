#include <string.h>

#define kMAX_DELAY		44100

class DigitalDelay {

public:
	bool inited;
	float buffer[kMAX_DELAY];
	
		DigitalDelay() {
			inited = false;
			delay = 0;
			pos = 0;
			feedback = 0.f;
			mix = 0.0f;
			setDelay(5000);
			delay = 1000;
		}
		
	
		int getDelay() {
			return delay;
		}
		
		bool setDelay(int newDelay) {
			if(newDelay!=delay) {
				pos = 0;
				delay = newDelay;
				memset(buffer, 0, delay*sizeof(float));
			}
			return true;
		}
		
		
	void reset() {
		memset(buffer, 0, kMAX_DELAY*sizeof(float));
	}
	
		
	int getMaxDelay() {
		return kMAX_DELAY;
	}
			
	
	float out;
	float process(float in) {	
		pos++;
		pos %= delay;
		out = buffer[pos];
		buffer[pos] = feedback*buffer[pos] + in;
		return in + (out - in)*mix;
	};

	
	float feedback;
	float mix;
	int delay;
	
private:
	int pos;
		
};
 