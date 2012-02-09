#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "Quicktime/Quicktime.h"



//---------------------------------------------

class ofxQTKitVideoTexture: public ofBaseDraws {

public:


	ofxQTKitVideoTexture ();
	virtual ~ofxQTKitVideoTexture();

	bool 	loadMovie(string name);
	
	// It's a Cocoa QTMovie, void*'s so it's C++ compatible
	void	*movie;
	
	// custom cocoa class to listen to the movie to find when it's done
	void    *listener;
	
	// other quicktime defs
	QTVisualContextRef      qtVisualContext;
	CVImageBufferRef		currentFrame;
	void threadedFunction();

	void draw(float x, float y) { draw(x,y,width,height); }
	void draw(float x, float y, float w, float h);
	float width;
	float height;

	void 				play();
	void 				stop();
	
	bool 				bLoaded;

	bool				getIsMovieDone();
	float 				getPosition();
	float 				getSpeed();
	float 				getDuration();


	void 				setPosition(float pct);
	void 				setLoopState(int state);
	void   				setSpeed(float speed);
	void				setVolume(float volume);
	void 				setPaused(bool bPause);

	float 				getHeight();
	float 				getWidth();
	void rewind();

	void idle();
	
	void bind();
	void unbind();
	bool update();	
	bool getMovieIsPlaying();
	static int videoCount;

	
	
	void movieEnded();
	

	
protected:
	string fileName;
	float  				speed;
	bool 				bPlaying;
	float duration;
	bool movieIsDone;
	unsigned char *frameData;
	

};

