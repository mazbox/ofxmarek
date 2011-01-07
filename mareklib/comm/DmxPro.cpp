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
 * included in all copies or substantial _portions of the Software.
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
 
#include "DmxPro.h"



DmxPro::DmxPro()
:
_pDmxPacket(NULL)
{
	setup(24);	
}

DmxPro::~DmxPro() {
	_port.close();
	delete []_pDmxPacket;
	_pDmxPacket = NULL;
	_connected = false;
}

void DmxPro::setup(int channels){
    _channels = channels;
	_packetSize = _channels + 5;
	delete []_pDmxPacket;
	_pDmxPacket = NULL;
	_pDmxPacket = new unsigned char[_packetSize];
	
	// first 4 bytes are control bytes
	_pDmxPacket[0]=DMX_PRO_START_MSG;
	_pDmxPacket[1]=DMX_PRO_SEND_PACKET;
	_pDmxPacket[2]=_channels & 255; // Data Length LSB
	_pDmxPacket[3]=(_channels >> 8) & 255; // Data Length MSB
	// dmx channel data, set all channels to zero
	for (int i=4; i<(_channels+4); ++i) {
		_pDmxPacket[i]=0;
	}
	// last byte is control
	_pDmxPacket[_packetSize-1]=DMX_PRO_END_MSG;
}


bool DmxPro::connect(string device, int channels){
/*
	char *buf = new char[device.size()+1];
	strcpy(buf,device.c_str());
	int _portStatus = connect(buf, channels);
	delete []buf;
	return _portStatus;
*/
	//_initialized = false;
	setup(channels);
	_port.enumerateDevices();
	_port.setVerbose(false);
	_connected = _port.setup(device.c_str(), 57600); 
	return _connected;
}

bool DmxPro::isConnected(){
	return _connected;
}


void DmxPro::disconnect(){
		//_port.closePort();
		//_portStatus = -1;
		_port.close();
}



void DmxPro::sendPacket(){
	
	_port.writeBytes(_pDmxPacket, _packetSize);
	
}

// channels range from 0-511, levels from 0-255
void DmxPro::sendLevel(int channel, int level, bool force){
    bool send = force;
	if(_pDmxPacket[4+channel]!=level)
		send = true;
		
	_pDmxPacket[4+channel] = level;
	
	if(send)
		sendPacket();
}

void DmxPro::sendZeros(){
	for (int i=4; i<(_channels+4); i++) {
		_pDmxPacket[i]=0;
	}
	sendPacket();
}

// valid channels range from 0-509
int DmxPro::getLevel(int channel){
	return (int)(_pDmxPacket[4+channel]);
}
