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

#include "Arjuna.h"

/**
 * Command Line Argument Container
 *
 * This global variable is used to contain command line arguments.
 * It is parsed on the start of the application, and then used all over
 * the code.
 */
struct Args args;

/**
 * MIDI Input/Output Handler
 *
 * This global variable is used to handlee MIDI Input/Output activity,
 * including sending and receiving MIDI messages
 */
MidiIO *io;

/**
 * Radio Transceiver Handler
 *
 * This global variable is used to handle radio transceiver activity,
 * including sending and receiving packages
 */
ORF24 *rf;

/**
 * Keypad Matrix Handler
 *
 * This global variable is used to handle keyboard matrix activity,
 * mainly handle key press
 */
WiringPiKeypad *keypad;

/**
 * Main Function
 *
 * This is the starting point of the program.
 * 
 * @param  argc arguments count
 * @param  argv arguments vector
 * @return      status
 */
int main(int argc, char *argv[])
{
	args = getArgs(argc, argv);

	if (initHardware())
	{
		std::cout << "Program is exiting..." << std::endl;
		return -1;
	}

	startRoutine();

	return 0;
}

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
int initHardware(void)
{
	if (args.debugEnabled)
		std::cout << "Setting up WiringPi..." << std::endl;

	if (wiringPiSetup())
	{
		std::cout << "Failed to set up WiringPi." << std::endl;
		return -1;
	}

	if (midiIOSetup())
	{
		std::cout << "Failed to set up MIDI I/O." << std::endl;
		return -1;
	}

	if (radioSetup())
	{
		std::cout << "Failed to set up radio transceiver." << std::endl;
		return -1;
	}

	if (keypadSetup())
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
int midiIOSetup(void)
{
	if (args.debugEnabled)
		std::cout << "Setting up MIDI I/O..." << std::endl;

	io = new MidiIO;

	io->enableDebug(args.debugEnabled);
	
	if (io->openMidiOutPort())
	{
		std::cout << "Error in opening MIDI output port." << std::endl;
		return -1;
	}

	if (io->openMidiInPort())
	{
		std::cout << "Error in opening MIDI input port." << std::endl;
		return -1;
	}

	return 0;
}

/**
 * Setup nRf24L01+ Radio Transceiver
 * 
 * @return  status
 */
int radioSetup(void)
{
	if (args.debugEnabled)
		std::cout << "Setting up radio transceiver..." << std::endl;

	rf = new ORF24(21);

	if (args.debugEnabled)
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
int keypadSetup(void)
{
	if (args.debugEnabled)
		std::cout << "Setting up keypad matrix..." << std::endl;

	int row[4] = {1, 2, 3, 4};
	int column[4] = {21, 22, 23, 24};
	std::vector<std::vector<char>> matrix {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};

	keypad = new WiringPiKeypad(4, 4);
	keypad->setRowPin(row);
	keypad->setColumnPin(column);
	keypad->setMatrix(matrix);

	return 0;
}

/**
 * Start Application Routine
 *
 * This routine start by showing main menu. It will handle user input
 * and call other methods depending on the input.
 */
void startRoutine(void)
{
	char keypress;
	std::string songPath;

	while (1)
	{
		showMenu();
		keypress = keypad->getKey();

		// Operation selector here
	}
}

/**
 * Show Application Menu
 *
 * In this screen, user can select the application operation.
 */
void showMenu(void)
{
	std::cout << "Welcome to Arjuna. Please select mode of operation below:"
	std::cout << " A - Select Song\n"
			  << " B - Play Song \n"
			  << " C - Start Evaluating\n"
			  << " D - Exit\n";
}