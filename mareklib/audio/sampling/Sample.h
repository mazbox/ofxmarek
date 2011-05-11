/**
 * Sample.h
 *
 * Marek Bereza 31/08/2010
 * 
 * Version 3
 *
 * This class relies on the input no. channels to be the output no. channels
 */
#pragma once

#include "WavFile.h"
#include <string>
#include <vector>

using namespace std;

class Playhead {
public:
	
	// can be negative if you want to start playing 
	// after a certain interval
	int frame;
	
	Playhead(int curr = 0) {
		frame = curr;
	}

};

class Sample {
public:

	Sample() {
		data = NULL;
		numChannels = 1;
		reset();

	}
	
	~Sample() {
		data = NULL;
	}
	
	
private:
	void reset() {
		length = 0;
		looping = false;
		multiplay = false;
		currFrames.clear();
	}
	
	
public:
	bool load(string path) {
		WavFile file;
		bool loadedSuccessfully = file.load(path);
		
		if(loadedSuccessfully) {
			length = file.getLength();
			numChannels = file.getNumChannels();
			data = file.getData();
		} 

		
		
		return loadedSuccessfully;
	}
	
	void setMultiPlay(bool multi) {
		multiplay = multi;
	}
	
	void setLooping(bool loop) { looping = loop; }
	
#define FRAME_BYTE_SIZE (getNumChannels()*sizeof(float))
	
	
	
	void getSamples(float *samples, int frames) {

		// clear out samples
		memset(samples, 0, frames*FRAME_BYTE_SIZE);
		

		if(data!=NULL) {
			
			// loop through each playhead
			for(unsigned int i = 0; i < currFrames.size(); i++) {
				bool isStillPlaying = accumulateFramesForPlayhead(&currFrames[i], samples, frames);
				
				if(!isStillPlaying) {
					// stop playing
					currFrames.erase(currFrames.begin()+i);
					i--;
				}
			}
		}
	}
	
	/**
	 * Returns true if the sound will still be playing the next time round.
	 * The idea is that if it's not, it should be removed from the playhead list.
	 */
	bool accumulateFramesForPlayhead(Playhead *playhead, float *samples, int frames) {
		
		// take up silence before the beginning of the playback of the playhead
		if(playhead->frame<0) {
			int numFramesUntilStart = -playhead->frame;
			// if there's enough silence to fill the whole buffer, just increment the playhead and return
			if(numFramesUntilStart>=frames) {
				playhead->frame += frames;
				return true;
				
				// if there's not enough silence at the beginning, advance the 
				// samples pointer over the silence and update the frame count to
				// how many frames are left over, and point the playhead to the 
				// beginning of the file.
			} else {
				playhead->frame = 0;
				samples += numFramesUntilStart*getNumChannels();
				frames -= numFramesUntilStart;
			}
		}
		// this is where looping will happen
		
		if(playhead->frame + frames < length) { // complete copy
			
			accumulateFrames(samples, playhead->frame, frames);
			playhead->frame += frames;
		} else if(playhead->frame < length) { // at end of sample - part copy
			
			// copy the audio
			int framesToCopy = length - playhead->frame;
			accumulateFrames(samples, playhead->frame, framesToCopy);
			
			if(looping) {
				
				
				if((frames-framesToCopy)>length) {
					printf("[Sample.h] Error: Sample is too short to loop - the program will probably crash!!!\n");
				}
				
				accumulateFrames(samples+framesToCopy*getNumChannels(), 0, (frames - framesToCopy));
				playhead->frame = frames - framesToCopy;
			} else {
				
				return false;
			}
		}
		return true;
	}

	virtual void trigger(int sampleOffset) {
		
		// if we're multiplay, push back another playhead
		if(multiplay) {
			currFrames.push_back(Playhead(sampleOffset));
		} else {
			// otherwise only add one if none exist
			if(currFrames.size()>0) {
				currFrames[0].frame = sampleOffset;
			} else {
				currFrames.push_back(Playhead(sampleOffset));
			}
		}
	}
	
	bool isPlaying() {
		return currFrames.size()>0;
	}
	
	void stop() {
		currFrames.clear();
	}
	
	int getNumChannels() { return numChannels; }
	int getLength() { return length; }
	float *getData() { return data; }
protected:
	
	/**
	 * Copies frames out of the file buffer and into dest
	 */
	void accumulateFrames(float *dest, int offset, int len) {
		offset *= getNumChannels();
		len *= getNumChannels();
		for(int i = 0; i < len; i++) {
			dest[i] += data[i+offset];
		}
	}
	
	
	float *data;
	int numChannels;
	
	// if not in multiplay mode, this is the playhead
	vector<Playhead> currFrames;
	
	int length;
	
	bool looping;
	bool multiplay;
};
