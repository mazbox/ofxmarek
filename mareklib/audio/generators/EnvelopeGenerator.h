
#pragma once


class EnvelopeGenerator {
public:
		

	enum { ATTACK, DECAY, SUSTAIN, RELEASE, DONE };
		
	EnvelopeGenerator() {
		reset();
	}
		
	void reset() {
		value = 0;
		state = DONE;
	}

	void noteOn() {
		state = ATTACK;
		value = 0;
		//printf("note onned\n");
	}
		

	bool isOff() {
		return state==DONE || state==RELEASE;
	}
	
	void noteOff() {
		state = RELEASE;
		//printf("----------note offed\n");
	}
	
	
	void setSustainLevel(float level){sustainLevel_ = level;}
	void setAttackTime(float time) {attackRate_ = 1.0 / ( (time+0.0001) * 44100.f);}
	void setDecayTime(float time){decayRate_ = 1.0 / ( (time+0.0001) * 44100.f );};		
	void setReleaseTime(float time){releaseRate_ = 1.0 / ( (time+0.0001) * 44100.f );}
	
	void init(float a, float dd, float s, float r) {
		setAttackTime(a);
		setDecayTime(dd);
		setSustainLevel(s);
		setReleaseTime(r);

	}

		
	float tick() {
		switch (state) {
			case ATTACK:
				value += attackRate_;
				if (value >= 1.f) {
					value = 1.f;
					state = DECAY;
				}
				return value;
				
			case DECAY:
				value -= decayRate_;
				if (value <= sustainLevel_) {
					value = sustainLevel_;
					state = SUSTAIN;
				}
				return value;
			
				
			case RELEASE:
				value -= releaseRate_;
				//value -= 0.001;
				
				if (value <= 0.0) {
					value = 0.f;
					state = DONE;
				}
				return value;
			default: return value;
		}
	}
	
	

		float attackRate_;
		float decayRate_;
		float sustainLevel_;
		float releaseRate_;
	
		float value;
		
		int state;

	};


