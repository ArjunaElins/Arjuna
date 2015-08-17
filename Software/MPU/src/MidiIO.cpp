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
 * Default Constructor
 *
 * This constructor sets input and output port to 1
 */
MidiIO::MidiIO()
{
	inPort = 1;
	outPort = 1;

	initIO();
}

/**
 * Constructor
 *
 * This constructor sets input and outpot port to provided arguments
 *
 * in and out arguments are human friendly number (starts from 1)
 * while MidiIO numbering starts from 0
 *
 * @param	int 	in 		input port
 * @param 	int 	out 	output port
 */
MidiIO::MidiIO(int in, int out): inPort(in - 1), outPort(out - 1)
{
	initIO();
}

/**
 * Inititalize MIDI IO handler
 */
void MidiIO::initIO(void)
{
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
}

/**
 * List MIDI Input Port
 *
 * Scan all available input port and print it to stdout
 */
void MidiIO::listMidiInPort(void)
{
	unsigned int nPorts = in->getPortCount();

	std::cout << "\nThere are " << nPorts << " MIDI input source available.\n";

	std::string portName;

	for (unsigned int i = 0; i < nPorts; i++)
	{
		try
		{
			portName = in->getPortName(i);
		}
		catch (RtMidiError &error)
		{
			error.printMessage();
			exit(EXIT_FAILURE);
		}

		std::cout << " Input port #" << i + 1 << ": " << portName << std::endl;
	}
}

/**
 * List MIDI Output Port
 *
 * Scan all available output port and print it to stdout
 */
void MidiIO::listMidiOutPort(void)
{
	unsigned int nPorts = out->getPortCount();

	std::cout << "\nThere are " << nPorts << " MIDI output port available.\n";

	std::string portName;

	for (unsigned int i = 0; i < nPorts; i++)
	{
		try
		{
			portName = out->getPortName(i);
		}
		catch (RtMidiError &error)
		{
			error.printMessage();
			exit(EXIT_FAILURE);
		}

		std::cout << " Output port #" << i + 1 << ": " << portName << std::endl;
	}
}

/**
 * Open MIDI Input Port
 * 
 * @return 	status
 */
int MidiIO::openMidiInPort(void)
{
	std::cout << "\nOpening input port #" << inPort + 1 << ": "
			  << in->getPortName(inPort) << "...\n";

	if (inPort >= in->getPortCount())
	{
		std::cout << "\nInput port #" << inPort + 1 << " is not available.\n";
		return -1;
	}

	in->openPort(inPort, "Arjuna MIDI Input");

	std::cout << "Input port #" << inPort + 1 << ": " << in->getPortName(inPort)
			  << " is open.\n";

	return 0;
}

/**
 * Open MIDI Input Port
 * 
 * @param  port 	port number
 * @return      	status
 */
int MidiIO::openMidiInPort(int port)
{
	inPort = port - 1;

	return openMidiInPort();
}

/**
 * Open MIDI output port
 * 
 * @return  	status
 */
int MidiIO::openMidiOutPort(void)
{
	std::cout << "\nOpening output port #" << outPort + 1 << ": "
			  << out->getPortName(outPort) << "...\n";

	if (outPort >= out->getPortCount())
	{
		std::cout << "\nOutput port #" << outPort + 1 << " is not available.\n";
		return -1;
	}

	out->openPort(outPort, "Arjuna MIDI Output");

	std::cout << "Output port #" << outPort + 1 << ": " << out->getPortName(outPort)
			  << " is open.\n";

	return 0;
}

/**
 * Open MIDI output port
 * 
 * @param  port 	port number
 * @return      	status
 */
int MidiIO::openMidiOutPort(int port)
{
	outPort = port - 1;

	return openMidiOutPort();
}

/**
 * Send MIDI message
 * @param message 	message container
 */
void MidiIO::sendMessage(std::vector<unsigned char> *message)
{
	out->sendMessage(message);
}

/**
 * Set a callback function to be invoked for incoming MIDI messages
 * 
 * @param callback 	callback function
 */
void MidiIO::setCallback(RtMidiIn::RtMidiCallback callback)
{
	in->setCallback(callback);
}

/**
 * Set a callback function to be invoked for incoming MIDI messages
 * 
 * @param callback 	callback function
 * @param userData 	data to pass to callback function
 */
void MidiIO::setCallback(RtMidiIn::RtMidiCallback callback, void *userData)
{
	in->setCallback(callback, userData);
}

/**
 * Fill the user-provided vector with the data bytes for the next available MIDI message in the input queue and return the event delta-time in seconds.
 * 
 * @param  message 	user-provided vector
 * @return          stamp
 */
double MidiIO::getMessage(std::vector<unsigned char> *message)
{
	return in->getMessage(message);
}