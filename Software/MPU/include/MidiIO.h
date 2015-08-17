/**
 * Arjuna: Alat Bantu Pembelajaran Piano untuk Tunanetra
 *
 * Developed by:
 * Ilham Imaduddin
 * Ahmad Shalahuddin
 * Piquitha Della Audyna
 *
 * Elektronika dan Instrumentasi
 * Universitas Gadjah Mada
 * 
 * Copyright (c) 2015 Arjuna
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#include <iostream>
#include "RtMidi.h"

class MidiIO
{
private:
	unsigned int inPort;
	unsigned int outPort;
	RtMidiIn *in = 0;
	RtMidiOut *out = 0;

	void initIO(void);

public:
	MidiIO();
	MidiIO(int in, int out);
	void listMidiInPort(void);
	void listMidiOutPort(void);
	int openMidiInPort(void);
	int openMidiInPort(int port);
	int openMidiOutPort(void);
	int openMidiOutPort(int port);
	void sendMessage(std::vector<unsigned char> *message);
	void setCallback(RtMidiIn::RtMidiCallback callback);
	void setCallback(RtMidiIn::RtMidiCallback callback, void *userData);
	double getMessage(std::vector<unsigned char> *message);
};