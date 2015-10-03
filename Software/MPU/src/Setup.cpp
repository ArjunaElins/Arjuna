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

#include "Setup.h"

/**
 * Get Command Line Arguments
 *
 * This function uses TCLAP library to parse command line arguments
 * 
 * @param  argc arguments count
 * @param  argv arguments vector
 * @return      parsed arguments
 */
struct Args getArgs(int argc, char *argv[])
{
	TCLAP::CmdLine cmd("Arjuna: Piano Learning Device For The Visually Impaired", ' ', "1.0.0.alpha-1");
	TCLAP::SwitchArg enableDebugSwitch("d", "debug", "Show debug information.", cmd, false);
	TCLAP::SwitchArg enableKeyboardSwitch("k", "keyboard", "Enable keyboard input.", cmd, false);

	cmd.parse(argc, argv);

	struct Args parsedArgs;
	parsedArgs.debugEnabled = enableDebugSwitch.getValue();
	parsedArgs.keyboardEnabled = enableKeyboardSwitch.getValue();

	return parsedArgs;
}

 /**
 * Initial Hardware Setup
 *
 * This function initialize the device to interface with hardware.
 * 
 * @return setup status
 */
int initHardware(struct Container *container, struct Args *args)
{
	if (args->debugEnabled)
		std::cout << "Setting up WiringPi..." << std::endl;

	if (wiringPiSetup())
	{
		std::cout << "Failed to set up WiringPi." << std::endl;
		return -1;
	}

	if (midiIOSetup(container, args))
	{
		std::cout << "Failed to set up MIDI I/O." << std::endl;
		return -1;
	}

	if (radioSetup(container, args))
	{
		std::cout << "Failed to set up radio transceiver." << std::endl;
		return -1;
	}

	if (keypadSetup(container, args))
	{
		std::cout << "Failed to set up keypad." << std::endl;
		return -1;
	}

	return 0;
}

/**
 * Setup MIDI Input/Output
 * 
 * @return  status
 */
int midiIOSetup(struct Container *container, struct Args *args)
{
	if (args->debugEnabled)
		std::cout << "Setting up MIDI I/O..." << std::endl;

	container->io = new MidiIO;
	MidiIO *io = container->io;

	io->enableDebug(args->debugEnabled);

	return 0;
}

/**
 * Setup nRf24L01+ Radio Transceiver
 * 
 * @return  status
 */
int radioSetup(struct Container *container, struct Args *args)
{
	if (args->debugEnabled)
		std::cout << "Setting up radio transceiver..." << std::endl;

	container->rf = new ORF24(21);
	ORF24 *rf = container->rf;

	if (args->debugEnabled)
		rf->enableDebug();

	rf->begin();
	rf->setPayloadSize(1);
	rf->setChannel(76);
	rf->setCRCLength(CRC_2_BYTE);
	rf->setPowerLevel(RF_PA_HIGH);

	return 0;
}

/**
 * Setup keypad matrix
 * 
 * @return  status
 */
int keypadSetup(struct Container *container, struct Args *args)
{
	if (args->debugEnabled)
		std::cout << "Setting up keypad matrix..." << std::endl;

	int row[4] = {21, 22, 23, 24};
	int column[4] = {1, 2, 3, 4};
	std::vector<std::vector<char>> matrix {
		{'D', 'C', 'B', 'A'},
		{'#', '9', '6', '3'},
		{'0', '8', '5', '2'},
		{'*', '7', '4', '1'}
	};

	container->keypad = new WiringPiKeypad(4, 4);
	WiringPiKeypad *keypad = container->keypad;

	keypad->setRowPin(row);
	keypad->setColumnPin(column);
	keypad->setMatrix(matrix);

	return 0;
}