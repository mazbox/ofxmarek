/*
 *  BufferResizer.h
 *  iPhoneEmptyExample
 *
 *  Created by Marek Bereza on 25/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */


// leftover size needs to be at least as big as the incoming buffer from rxA_mixSheetToBuffer()
// say 40 BPM at 44100, 16th note = 44100Hz*60seconds/40BPM/16th = 4135 samples ~ 4200 to be safe
#define LEFTOVER_SIZE 4200
#define arraycopy(SRC, SRCPOS, DEST, DESTPOS, LENGTH) memcpy(DEST+DESTPOS, SRC+SRCPOS, LENGTH)



class BufferSource {
public:
	virtual void getNextBuffer(void *c) = 0;
	virtual int getBufferSize() = 0;

};
class BufferResizer {
public:
	char *inputBuffer;
	BufferSource *bufferSource;
	BufferResizer(BufferSource *bufferSource) {
		this->bufferSource = bufferSource;
		pos = 0;
		leftoverSize = 0;
		leftoverPos = 0;
		inputLen = bufferSource->getBufferSize();
		inputBuffer = (char*)malloc(inputLen);
	}
	
	/**
	 * outputs is a pointer to the allocated memory
	 * length is in bytes.
	 */
	void getNextBuffer(char *output, int byteLen) {
		//printf("Get next buffer\n");
		int bufPos				= 0;
		
		// see if there's any leftovers from previous cycle
		if(leftoverSize>0) {
			// if we can use up all the leftovers, then do so
			if(leftoverSize<=byteLen) {
				arraycopy(leftovers, leftoverPos, output, 0, leftoverSize);
				bufPos = leftoverSize;
				
				// reset leftovers
				leftoverSize = 0;
				leftoverPos = 0;
			} else {
				// otherwise, just copy enough to fill the buffer
				arraycopy(leftovers, leftoverPos, output, 0, byteLen);
				bufPos = byteLen;
				leftoverPos += byteLen;
				leftoverSize -= byteLen;
			}
		}
		
		
		//printf("Done with leftovers\n");
		// keep getting samples until the buffer is full
		while(bufPos<byteLen) {
			//printf("%d < %d ?\n", bufPos, byteLen);
			
			bufferSource->getNextBuffer(inputBuffer);
			// if we've just filled up the buffer
			// or not quite filled up the buffer
			// copy, and loop again
			if(bufPos+inputLen<=byteLen) {
				arraycopy(inputBuffer, 0, output, bufPos, inputLen);
				bufPos += inputLen;
			} else {
				// if we've got more than we need
				// just copy to fill, and save the rest as leftovers
				int copyLength = byteLen - bufPos;
				arraycopy(inputBuffer, 0, output, bufPos, copyLength);
				bufPos += copyLength;
				
				// save leftovers
				leftoverSize = inputLen - copyLength;
				arraycopy(inputBuffer, copyLength, leftovers, 0, leftoverSize);
				
			}
		}
		//printf("Returning\n");
	}
	int inputLen;
	char leftovers	[LEFTOVER_SIZE];
	int pos;
	int leftoverSize;
	int leftoverPos;
};