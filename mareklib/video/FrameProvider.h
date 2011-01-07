/*
 *  FrameProvider.h
 *  IceGenerator
 *
 *  Created by Marek Bereza on 25/07/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

template <class T>
class FrameProvider {
public:
	
	
	// implement these!
	virtual bool load(string dir) = 0;
	virtual T getFrame(int requestedFrame = -1) = 0;
	virtual int getLength() = 0;
	
	
	
	
	FrameProvider() {
		looping = false;
		frame = 0;
	}

	
	
	bool done() {
		return (!looping) && frame>getLength()-1;
	}
	void rewind() {
		frame = 0;
	}
	
	float getPosition() {
		return (float)frame/(float)getLength();
	}
	
	void setLooping(bool _looping) {
		looping = _looping;
	}
	
	
protected:
	
	
	int frame;
	bool looping;
};