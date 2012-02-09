#pragma once
#include "ZenGarden.h"

#include <string>
using namespace std;


class ofxZenGarden {
public:
	ofxZenGarden();
	
	void getDirAndFile(const char *path, char *outDir, char *outFile);
	int getBlockSize() { return blockSize; } 
	
	
	void setBlockSize(int bufferSize);	
	void load(string patchFile, int inputs, int outputs, int samplerate, int bufferSize);
	
	// returns whether this patch has an audio input
	bool hasInput();
	
	~ofxZenGarden();
	void sendFloat(string messageName, float value);
	void sendBang(string messageName);	
	void sendMidiNote(int channel, int noteNum, int velocity, int blockOffset = 0);
	
	void process(float *input, float *output, int frameCount);
	void processNonInterleaved(float *input, float *output, int frameCount);
	
	bool isRunning() { return running; }
private:
	
	ZGContext *context;
	
	int blockSize;
	int numInputChannels;
	int numOutputChannels;
	int sampleRate;
	string path;
	
	bool hasADC;
	// for interleaving
	float *inputBuffer;
	float *outputBuffer;
	bool running;
};
