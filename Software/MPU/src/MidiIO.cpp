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

#include "MidiIO.h"

/**
* MidiIO Class Constructor
*
* This constructor set the default input and output port to 1
*/
MidiIO::MidiIO(): inPort(1), outPort(1)
{
	initIO();
}

/**
 * MidiIO Class Constructor
 *
 * This constructor receive input and output port as arguments and set
 * those ports to the class properties.
 *
 * The in and out arguments are human friendly number (starts from 1)
 * while MidiIO numbering starts from 0
 *
 * @param  int 	in 	input port
 * @param  int 	out	output port
 */
MidiIO::MidiIO(int in, int out): inPort(in - 1), outPort(out - 1)
{
	initIO();
}

/**
 * Initialize MIDI IO
 *
 * This method creates RtMidi input and output instance
 */
void MidiIO::initIO(void)
{
	if (debug)
		std::cout << "Creating MIDI In and MIDI Out instance..." << std::endl;

	try
	{
		in = new RtMidiIn();
		out = new RtMidiOut();
	}
	catch (RtMidiError &error)
	{
		error.printMessage();
		exit(EXIT_FAILURE);
	}

	if (debug && in)
		std::cout << "MIDI In instance created." << std::endl;

	if (debug && out)
		std::cout << "MIDI In instance created." << std::endl;
}

/**
 * Enable Debug
 *
 * This option will show debug informatio if set to true
 *
 * @param 	bool 	enable
 */
void MidiIO::enableDebug(bool enable)
{
	debug = enable;
}

/**
 * Open MIDI Input Port
 * 
 * @return  status
 */
int MidiIO::openMidiInPort(void)
{
	if (debug)
		std::cout << "\n  Opening input port #" << inPort + 1 << ": "
				  << in->getPortName(inPort) << "...\n";

	if (inPort >= in->getPortCount())
	{
		if (debug)
			std::cout << "\n  Input port #" << inPort + 1 << " is not available.\n";
		
		return -1;
	}

	in->openPort(inPort, "Arjuna MIDI Input");

	if (debug)
		std::cout << "  Input port #" << inPort + 1 << ": " << in->getPortName(inPort)
				  << " is open.\n";

	return 0;
}

/**
 * Open MIDI output port
 * 
 * @return  	status
 */
int MidiIO::openMidiOutPort(void)
{
	if (debug)
		std::cout << "\n  Opening output port #" << outPort + 1 << ": "
				  << out->getPortName(outPort) << "...\n";

	if (outPort >= out->getPortCount())
	{
		if (debug)
			std::cout << "\n  Output port #" << outPort + 1 << " is not available.\n";
		
		return -1;
	}

	out->openPort(outPort, "Arjuna MIDI Output");

	if (debug)
		std::cout << "  Output port #" << outPort + 1 << ": " << out->getPortName(outPort)
				  << " is open.\n";

	return 0;
}

/**
 * Send MIDI Message to Output Port
 * 
 * @param message 	message container
 */
void MidiIO::sendMessage(std::vector<unsigned char> *message)
{
	out->sendMessage(message);
}

/**
 * Receive MIDI message from Input port
 * 
 * @param  message 	message container
 * @return         	stamp
 */
double MidiIO::getMessage(std::vector<unsigned char> *message)
{
	return in->getMessage(message);
}