/*
 *  ofxPd.cpp
 *  PdPlayer
 *
 *  Created by Marek Bereza on 11/04/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ZenGarden.h"
#include "ofxZenGarden.h"
#include "ofMain.h"

#include <fstream>


extern "C" {
	void callbackFunction(ZGCallbackFunction function, void *userData, void *ptr) {
		switch (function) {
			case ZG_PRINT_STD: {
				ofstream file("/Users/marek/Desktop/log.txt", ofstream::app);
				file << (char *)ptr << endl;
				file.close();
				printf("%s\n", (char *) ptr);
				break;
			}
			case ZG_PRINT_ERR: {
				ofstream file("/Users/marek/Desktop/log.txt", ofstream::app);
				file << "ERROR: " << (char *)ptr << endl;
				file.close();
				printf("ERROR: %s\n", (char *) ptr);
				break;
			}
			default: {
				break;
			}
		}
	}
};



ofxZenGarden::ofxZenGarden() {
	context = NULL;
	outputBuffer = NULL;
	inputBuffer = NULL;
	running = false;
	hasADC = false;
}

bool ofxZenGarden::hasInput() {
	return hasADC;
}

void ofxZenGarden::getDirAndFile(const char *path, char *outDir, char *outFile) { 
	char *lastSlash = strrchr(path, '/'); 
	sprintf(outDir, ""); 
	if(lastSlash==NULL) { 
		sprintf(outFile, "%s", path); 
	} else { 
		strncpy(outDir, path, 1+1+lastSlash-path); 
		outDir[1+lastSlash-path] = '\0'; 
		strcpy(outFile, lastSlash+1); 
	} 
} 




void ofxZenGarden::setBlockSize(int bufferSize) {
	
	if(bufferSize!=blockSize) {
		printf("Readjusting block size from %d to %d\n", blockSize, bufferSize);
		load(path, numInputChannels, numOutputChannels, sampleRate, bufferSize);
	}
}



void ofxZenGarden::load(string patchFile, int inputs, int outputs, int samplerate, int bufferSize) {
	patchFile = ofToDataPath(patchFile);
	running = false;

	hasADC = false;
	blockSize = bufferSize;
	numInputChannels = inputs;
	numOutputChannels = outputs;
	sampleRate = samplerate;
	path = patchFile;
	if(inputBuffer!=NULL) delete [] inputBuffer;
	if(outputBuffer!=NULL) delete [] outputBuffer;
	
	outputBuffer = new float[numOutputChannels*blockSize];
	inputBuffer = new float[numInputChannels*blockSize];
	char file[256];
	char dir[512];
	getDirAndFile(patchFile.c_str(), dir, file);
	if(context!=NULL) {
		delete context;
		context = NULL;
	}
	printf("Starting graph with in: %d out: %d sr: %d bs: %d\n", numInputChannels, numOutputChannels, sampleRate, blockSize);
	
	
	context = zg_new_context(numInputChannels, numOutputChannels, blockSize, sampleRate,
										callbackFunction, NULL);
	ZGGraph *graph = zg_new_graph(context, dir, file);
	if (graph == NULL) {
		printf("PdGraph could not be created. Is the filename correct?\n");
		zg_delete_context(context);
		return;
	}
	
	zg_attach_graph(context, graph);
	
	
	
	
	// look through the pd file and see if there's a dac~ in it
	
	
	
	string line;
	ifstream myfile (patchFile.c_str());
	if (myfile.is_open()) {
		while (! myfile.eof() ) {
			getline (myfile,line);
			if(line.find("adc~;")!=-1) {
				hasADC = true;
				break;
			}
		}
		myfile.close();
	}
	
	
	if(hasADC) {
		printf("Has input\n");
	} else {
		printf("No input\n");
	}


	running = true;
}


ofxZenGarden::~ofxZenGarden() {
	if(context!=NULL)
		delete context;
	if(inputBuffer!=NULL) {
		delete [] inputBuffer;
	}
	if(outputBuffer!=NULL) {
		delete [] outputBuffer;
	}
}

void ofxZenGarden::sendFloat(string messageName, float value) {
	if(!running) return;

	zg_send_message(context, messageName.c_str(), "f", value);
	//PdMessage *message = graph->scheduleExternalMessage((char*)messageName.c_str());
	//message->setMessage(1, FLOAT, value);

}

void ofxZenGarden::sendBang(string messageName) {
	if(!running) return;

	zg_send_message(context, messageName.c_str(), "b");
	//PdMessage *message = graph->scheduleExternalMessage((char*)messageName.c_str());
	//message->setMessage(1, BANG);

}

void ofxZenGarden::sendMidiNote(int channel, int noteNum, int velocity, int blockOffset) {
	if(!running) return;

	zg_send_midinote(context, channel, noteNum, velocity, blockOffset);
	//PdMessage *message = graph->scheduleExternalMessage((char*)messageName.c_str());
	//message->setMessage(1, BANG);

}

void ofxZenGarden::process(float *input, float *output, int frameCount) {
	if(context==NULL || !running) return;
	setBlockSize(frameCount);
	memset(output, 0, frameCount*sizeof(float)*numOutputChannels);

	zg_process(context, input, outputBuffer);

	
	
	if(numOutputChannels==1) {
		memcpy(output, outputBuffer, sizeof(float)*frameCount);
	} else if(numOutputChannels==2) {
		for(int i = 0; i < frameCount; i++) { 
			output[i*2] = outputBuffer[i];
			output[i*2 + 1] = outputBuffer[i + blockSize];
		}
	} else {
		for(int i = 0; i < blockSize; i++) { 
			for(int channel = 0; channel < numOutputChannels; channel++) { 
				output[i*numOutputChannels + channel] = outputBuffer[i + channel*blockSize]; 
			} 
		}
	}
}

void ofxZenGarden::processNonInterleaved(float *input, float *output, int frameCount) {
	if(context==NULL || !running) return;
	setBlockSize(frameCount);
	memset(output, 0, frameCount*sizeof(float)*numOutputChannels);

	zg_process(context, input, output);
	
}

