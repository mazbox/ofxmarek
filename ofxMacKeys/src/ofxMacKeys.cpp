/**
 * ofxMacKeys.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "ofxMacKeys.h"



bool ofxMacShiftKeyDown() {
	return CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & kCGEventFlagMaskShift;
}
bool ofxMacControlKeyDown() {
	return CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & kCGEventFlagMaskControl;
}
bool ofxMacCommandKeyDown() {
	return CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & kCGEventFlagMaskCommand;
}
bool ofxMacCapsLockKeyDown() {
	return CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & 0x10000;
}
bool ofxMacAltKeyDown() {
	return CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & kCGEventFlagMaskAlternate;
}
bool ofxMacFunctionKeyDown() {
	return CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & kCGEventFlagMaskSecondaryFn;
//	CGEventFlags s = CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState);
//	cout << "Testey: " <<s <<endl;
}


