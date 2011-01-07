/*
 * Copyright 2007-2008 (c) Erik Sjodin, eriksjodin.net
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
 
/*
 Butchered by Marek to have no non-oF dependencies
 */

#pragma once
#include <iostream>

#include <ofSerial.h> 




#ifndef GLUE_IO_DMX_PRO_ENUMS
 		
		#define DMX_PRO_START_MSG 0x7E 
		#define DMX_PRO_END_MSG 0xE7
		
		#define DMX_PRO_SEND_PACKET 6 
		// requests the Widget to periodically send a DMX packet out of the Widget DMX port at the configured DMX output rate.
			
#endif

/**
        I've only tested this above channel 256 using an EDX4 dimmer rack, for some reason I couldn't get it to work above channel 501
		above channel 509 the dmx usb pro stopped sending packets.
**/

/**
 * \brief Interface to USB DMX Pro box. Definitely works on mac, got to test on windows.
 */
class DmxPro{

	public:
	
		DmxPro();

		~DmxPro();

		
		/** 
		 * Connect like a serial port. (it is a serial port) 
		 * valid channel range is 24-512
		 * performance is directly related to the number of channels, so don't use more channels than necessary
		 */
		bool connect(const string device, int channels=24);
	
		/** valid channel range is 24-512  */
		void setup(int channels=24);

		
		void disconnect();
		void flush();
		
		// --- the following senders only sends the packet if the channel data has changed, unless force = true.
		
		/** The usual way to send DMX stuff, channel range is 0-511, valid level range is 0-255 */
		void sendLevel(int channel, int level, bool force=false);

		/** clear out DMX messages. Darkness! */
		void sendZeros();
	
	

		/** remember what you already sent */
		int getLevel(int channel);	
		// valid channel range is 0-511
		
	
		bool isConnected();
		int _connected;
	private:
	
	    void sendPacket();
		// sends the dmx packet
		
		int _channels;
		int _packetSize;
		unsigned char * _pDmxPacket;
		ofSerial _port;
		
};

