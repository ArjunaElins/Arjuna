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

#ifndef _MIDI_IO_H_
#define _MIDI_IO_H_

#include <iostream>

#include "Debug.h"
#include "RtMidi.h"

/**
 * MidiIO Class Interface
 *
 * MidiIO is a class to interface with MIDI device. It allows the application
 * to open and close input and output port with the MIDI device, and send them
 * MIDI messages.
 */
class MidiIO
{
private:

	/**
	 * Input Port Number
	 */
	unsigned int inPort;

	/**
	 * Output Port Number
	 */
	unsigned int outPort;

	/**
	 * Debug Level
	 *
	 * The higher the level, more information will be shown
	 */
	DebugLevel debugLevel;

	/**
	 * RtMidi Input instance
	 */
	RtMidiIn *in;

	/**
	 * RtMidi Output instance
	 */
	RtMidiOut *out;

	/**
	 * Initialize MIDI IO
	 *
	 * This method creates RtMidi input and output instance
	 */
	void initIO(void);

public:
	/**
	 * MidiIO Class Constructor
	 *
	 * This constructor set the default input and output port
	 */
	MidiIO();

	/**
	 * MidiIO Class Constructor
	 *
	 * This constructor receive input and output port as arguments and set
	 * those ports to the class properties.
	 *
	 * @param  int 	in 	input port
	 * @param  int 	out	output port
	 */
	MidiIO(int in, int out);

	/**
	 * Set Debug Level
	 *
	 * @param  DebugLevel  level  debug level
	 */
	void setDebugLevel(DebugLevel level);

	/**
	 * Open MIDI Input Port
	 * 
	 * @return  status
	 */
	int openMidiInPort(void);

	/**
	 * Open MIDI Output Port
	 * 
	 * @return  status
	 */
	int openMidiOutPort(void);
};

#endif