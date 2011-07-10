/**
 * NovationLaunchPad.h
 * MidiTester
 *
 * Created by Marek Bereza on 28/06/2011.
 *
 */

#define LAUNCHPAD_OFF       12
#define LAUNCHPAD_RED_LOW   13
#define LAUNCHPAD_RED		15
#define LAUNCHPAD_AMBER_LOW 29
#define LAUNCHPAD_AMBER		63
#define LAUNCHPAD_YELLOW	62
#define LAUNCHPAD_GREEN_LOW 28
#define LAUNCHPAD_GREEN		60
#include "ofxMidi.h"

class LaunchpadListener {
public:
	virtual void launchpadNotePressed(int x, int y) {}
	virtual void launchpadNoteReleased(int x, int y) {}
};

class NovationLaunchpad: public ofxMidiListener {
public:
	
	vector<LaunchpadListener*> listeners;
	
	void addListener(LaunchpadListener *listener) {
		listeners.push_back(listener);
	}
	ofMutex midiMutex;
	void connect(bool inputOnly = false) {
		this->inputOnly = inputOnly;
		immediateMode = true;
		for(int i = 0; i < 128; i++) {
			_notes[i] = lastNotes[i] = LAUNCHPAD_OFF;
		}
		//	midiIn.setVerbose(true);
		midiIn.listPorts();
		bool foundLaunchPad = false;
		for(int i = 0; i < midiIn.portNames.size(); i++) {
			if(midiIn.portNames[i].find("aunchpad")!=-1) {
				printf("Found %s (%d)\n", midiIn.portNames[i].c_str(), i);
				foundLaunchPad = true;
				midiIn.openPort(i);
				break;
			}
		}
		
		if(!foundLaunchPad) {
			midiIn.openPort(0);
		}
		
		midiIn.addListener(this);
		
		if(!inputOnly) {
			foundLaunchPad = false;
			midiOut.listPorts();
			for(int i = 0; i < midiOut.portNames.size(); i++) {
				if(midiOut.portNames[i].find("aunchpad")!=-1) {
					printf("Found %s (%d)\n", midiOut.portNames[i].c_str(), i);
					foundLaunchPad = true;
					midiOut.openPort(i);
					break;
				}
			}
			
			if(!foundLaunchPad) {
				printf("Couldn't find launchpad out!\n");
				midiOut.openPort(0);
			}
			
			clear();
		}
	}
	
	void clear() {
		for(int i = 0; i < 8; i++) {
			for(int j = 0; j < 8; j++) {
				off(i, j);
			}
		}
	}
	bool isOn(int x, int y) {
		return _notes[coordsToIndex(x, y)]!=LAUNCHPAD_OFF;
	}
	
	void red(int x, int y) {
		setColour(x, y, LAUNCHPAD_RED);
	}
	
	void green(int x, int y) {
		setColour(x, y, LAUNCHPAD_GREEN);
	}
	
	void amber(int x, int y) {
		setColour(x, y, LAUNCHPAD_AMBER);
	}
	
	void yellow(int x, int y) {
		setColour(x, y, LAUNCHPAD_YELLOW);
	}
	
	void off(int x, int y) {
		setColour(x, y, LAUNCHPAD_OFF);
	}
	
	
	void setImmediateMode(bool immediateMode) {
		this->immediateMode = immediateMode;
	}
	void update() {
		
		midiMutex.lock();
		while(noteEvents.size()>0) {
			int x = noteEvents.front().note % 8;
			int y = noteEvents.front().note / 16;
			int vel = noteEvents.front().vel;
			noteEvents.pop_front();
			if(vel>0) {
				for(int i = 0; i < listeners.size(); i++) {	
					listeners[i]->launchpadNotePressed(x, y);
				}
				
			} else {
				for(int i = 0; i < listeners.size(); i++) {	
					listeners[i]->launchpadNoteReleased(x, y);
				}

			}
		}
		midiMutex.unlock();
		if(inputOnly) return;
		// do all the launchpad calls
		if(!immediateMode) {


			// double buffered update
			for(int x = 0; x < 8; x++) {
				for(int y = 0; y < 8; y++) {
					int index = coordsToIndex(x, y);
					if(_notes[index]!=lastNotes[index]) {
						midiOut.sendNoteOn(1, index, _notes[index]);
					}
				}
			}
			/*
			for(int i = 0; i < 80; i+=2) {
				if(i<64) {
					int x = i%8;
					int y = i/8;
					midiOut.sendNoteOn(3, _notes[coordsToIndex(x, y)], _notes[coordsToIndex(x+1, y)]);
				} else {
					midiOut.sendNoteOn(3, LAUNCHPAD_OFF, LAUNCHPAD_OFF);
				}
			}
			midiOut.sendNoteOn(1, 0, 0);	
			 */
		}
		
		memcpy(lastNotes, _notes, sizeof(int)*128);
	}
	
	void setColour(int x, int y, int colour) {
		if(inputOnly) return;
		int index = coordsToIndex(x, y);
		if(immediateMode) midiOut.sendNoteOn(1, index, colour);
		_notes[index] = colour;
	}
	
	
	
	struct NoteEvent {
		int note;
		int vel;
	};
	
	void newMidiMessage(ofxMidiEventArgs& eventArgs) {
		midiMutex.lock();
		noteEvents.push_back(NoteEvent());
		noteEvents.back().note = eventArgs.byteOne;
		noteEvents.back().vel = eventArgs.byteTwo;

		midiMutex.unlock();
//		if(lights[eventArgs.byteOne]) {
//			// turn the light off
//			lightOff(eventArgs.byteOne);
//		} else {
//			// turn the light on
//			lightOn(eventArgs.byteOne);
//		}
		//printf("port: %d   channel: %d  status %x   1: %x   2: %x\n", eventArgs.port, eventArgs.channel, eventArgs.status, eventArgs.byteOne, eventArgs.byteTwo);
	}
	
private:
	bool inputOnly;
	deque<NoteEvent> noteEvents;
	int _notes[128];
	int lastNotes[128];
	ofxMidiIn midiIn;
	ofxMidiOut midiOut;
	bool immediateMode;
	int coordsToIndex(int x, int y) {
		return x + y*16;
	}
};