/**
 * MonoSample.h
 *
 * Monophonic sample.
 *
 * Marek Bereza 31/08/2010
 * 
 * Version 1
 */
#include "WavFile.h"
#include <string>
using namespace std;


class MonoSample {
public:

	MonoSample() {
		reset();

	}
	
private:
	void reset() {
		playing = false;
		currFrame = 0;
		length = 0;
		looping = true;
	}
	
public:
	bool load(string path) {
		
		bool loadedSuccessfully = file.load(path);
		
		if(loadedSuccessfully) {
			length = file.getLength();
		} else {

		}
		
		return loadedSuccessfully;
	}
	
	void setLooping(bool loop) { looping = loop; }
	
#define FRAME_BYTE_SIZE (getNumChannels()*sizeof(float))
	
	
	void getSamples(float *samples, int frames) {
		
		if(currFrame>=length) playing = false;

		// if we're not playing then send silence
		if(!playing || file.data==NULL) {
			memset(samples, 0, frames*FRAME_BYTE_SIZE);
		} else {
			
			
			// this is where looping will happen
			if(currFrame + frames < length) { // complete copy
				int bytesToCopy = frames*FRAME_BYTE_SIZE;
				memcpy(samples, file.data+currFrame*getNumChannels(), bytesToCopy);
				currFrame += frames;
			} else if(currFrame < length) { // at end of sample - part copy
				
				
				
				memset(samples, 0, frames*FRAME_BYTE_SIZE);
				
				// copy the audio
				int framesToCopy = length - currFrame;
				memcpy(samples, file.data+currFrame*getNumChannels(), framesToCopy*FRAME_BYTE_SIZE);
				
				if(looping) {
					
					
					if((frames-framesToCopy)>length) {
						printf("[Sample.h] Error: Sample is too short to loop - the program will probably crash!!!\n");
					}
					
					
					memcpy(samples+framesToCopy*getNumChannels(), file.data, (frames - framesToCopy)*FRAME_BYTE_SIZE);
					//memset(samples+framesToCopy*FRAME_BYTE_SIZE, 0, (frames - framesToCopy)*FRAME_BYTE_SIZE);
					currFrame = frames - framesToCopy;
				} else {
					// fill the rest with silence
					memset(samples+framesToCopy*getNumChannels(), 0, (frames - framesToCopy)*FRAME_BYTE_SIZE);
					// stop playing
					playing = false;
					currFrame += frames;
				}
			}
		}
	}
	
	virtual void trigger() {
		playing = true;
		currFrame = 0;
	}
	
	bool isPlaying() {
		return playing;
	}
	
	int getNumChannels() { return file.channels; }
	
private:
	WavFile file;
	int currFrame;
	int length;
	bool playing;
	bool looping;
};
