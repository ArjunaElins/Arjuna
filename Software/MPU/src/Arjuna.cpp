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
	Args args = getArgs(argc, argv);

	Container container;

	if (initHardware(&container, &args))
	{
		std::cout << "Program is exiting..." << std::endl;
		return -1;
	}

	// Routine should have container
	startRoutine(&container);

	return 0;
}

/**
 * Start Application Routine
 *
 * This routine start by showing main menu. It will handle user input
 * and call other methods depending on the input.
 */
void startRoutine(Container *container)
{
	char keypress;
	std::string songPath;
	WiringPiKeypad *keypad = container->keypad;

	while (1)
	{
		showMenu();
		keypress = keypad->getKey();

		if (keypress == '*')
			songPath.assign(songSelector(container));
		else if (keypress == '0')
			songPlayer(container, songPath);
	}
}

/**
 * Show Application Menu
 *
 * In this screen, user can select the application operation.
 */
void showMenu(void)
{
	std::cout << "Welcome to Arjuna. Please select mode of operation below:" << std::endl;
	std::cout << " A - Select Song\n"
			  << " B - Play Song \n"
			  << " C - Start Evaluating\n"
			  << " D - Exit\n";
}

/**
 * Song Selector
 *
 * This method will start the song selector.
 * 
 * @param container hardware handler
 */
std::string songSelector(Container *container)
{
	const std::string basedir = "/home/arjuna/Songs/";

	std::ifstream songList(basedir + ".songlist");

	if (songList.is_open())
	{
		printSongList(&songList);
		std::string song = selectSong(container, &songList);

		return basedir + song + "/" + song;
	}
	else
	{
		std::cout << "Error opening file." << std::endl;
		return 0;
	}
}

/**
 * Print Song List
 *
 * This method will parse every line of the file handler and print it
 * to stdout
 * 
 * @param songList [description]
 */
void printSongList(std::ifstream *songList)
{
	std::cout << "Song list:" << std::endl;

	int i = 1;
	std::string song;

	while (getline(*songList, song))
	{
		std::cout << i++ << ". " << song << std::endl;
	}
}

/**
 * Select Song
 *
 * This method will ask the user to choose from opened song list
 * 
 * @param  songList song list handler
 * @return          song name
 */
std::string selectSong(Container *container, std::ifstream *songList)
{
	std::cout << "Press number to select song. Press '*' to select." << std::endl;

	char keypress;
	std::string songNumber;
	WiringPiKeypad *keypad = container->keypad;

	do
	{
		keypress = keypad->getKey();
		songNumber += keypress;
	} while (keypress != '*');

	songNumber.pop_back(); // Remove 'A' from songNumber
	int number = std::stoi(songNumber);
	songList->clear();
	songList->seekg(0, std::ios::beg);

	std::string song;
	for (int i = 0; i < number; i++)
		getline(*songList, song);
	std::cout << "Selected: " << song << std::endl;

	return song;
}

/**
 * Song Player
 *
 * This method is used to play MIDI song. It will open the MIDI file,
 * calculate some numbers, and send MIDI message to output port
 * 
 * @param  container hardware handler
 * @param  songPath  MIDI song location
 */
void songPlayer(Container *container, std::string songPath)
{
	// MIDI data preparation
	MidiFile midi(songPath + ".mid");
	midi.deltaTicks();
	int tpq = midi.getTicksPerQuarterNote();
	double spt = 0.5 / tpq;

	// Finger data preparation
	FingerData finger(songPath + ".fgr");
	std::vector<int> iFinger(midi.getTrackCount(), 0);

	setPlayMode(&midi, getPlayMode(container));

	std::cout << "Playing song \"" << songPath << "\"...\n";
	std::cout << "Press C to go to next checkpoint. Press D to stop.\n";

	delay(500);
	for (int t = 0; t < midi.getTrackCount(); t++)
	{
		for (int e = 0; e < midi[t].size(); e++)
		{
			getTempoSPT(midi[t][e], tpq, &spt);

			if (midi[t][e].isMeta()) continue;

			skipFingerMetadata(finger, &iFinger, midi.getSplitTrack(t, e));
			delayMicroseconds(spt * midi[t][e].tick * 1000000);
			sendMidiMessage(container->io, midi[t][e]);
			sendFeedback(container->rf, finger, &iFinger, midi.getSplitTrack(t, e));
		}
	}
}

/**
 * Get Play Mode
 *
 * This function ask the user to select the play mode
 * 
 * @param  container hardware handler
 * @return           play mode
 */
PlayMode getPlayMode(Container *container)
{
	std::cout << "Select Play Mode." << std::endl
			  << " A - Both hands" << std::endl
			  << " B - Right hands" << std::endl
			  << " C - Left hands" << std::endl;

	char keypress;
	PlayMode mode = BOTH_HANDS;

	keypress = container->keypad->getKey();

	if (keypress == 'A')
		mode = BOTH_HANDS;
	else if (keypress == 'B')
		mode = RIGHT_HAND;
	else if (keypress == 'C')
		mode = LEFT_HAND;

	return mode;
}

/**
 * Set Play Mode
 *
 * This function prepares the MIDI file to play in right, left, or both hand modes
 * 
 * @param midi MIDI object
 * @param mode Chosen mode
 */
void setPlayMode(MidiFile *midi, PlayMode mode)
{
	if (mode == LEFT_HAND)
		midi->deleteTrack(0);
	else if (mode == RIGHT_HAND)
		midi->deleteTrack(1);
	else
		midi->joinTracks();
}

/**
 * Get Tempo in Second Per Tick
 * 	
 * @param e   	MidiEvent
 * @param tpq 	tick peq quarter
 * @param spt 	second per tick
 */
void getTempoSPT(MidiEvent e, int tpq, double *spt)
{
	if (e.getTempoSPT(tpq) > 0)
		*spt = e.getTempoSPT(tpq);
}

/**
 * Send MIDI Message
 *
 * This method will form a MIDI message container and send it to MIDI output port
 * 
 * @param io MIDI i/O port
 * @param e  MIDI event
 */
void sendMidiMessage(MidiIO *io, MidiEvent e)
{
	std::vector<unsigned char> message;

	for (unsigned int i = 0; i < e.size(); i++)
		message.push_back((int) e[i]);

	io->sendMessage(&message);
}

/**
 * Skip Finger Metadata
 * 
 * @param finger finger data
 * @param i      finger index
 * @param t      active track
 */
void skipFingerMetadata(FingerData finger, std::vector<int> *i, int t)
{
	FingerEvent e = finger[t][i->at(t)];

	if (e.getCommand() == 0xF9)
		i->at(t)++;
}

/**
 * Send Feedback to Hand Module
 *
 * This method use the radio transceiver to send payload to hand module
 * 
 * @param rf radio handler
 * @param f  finger data
 * @param i  finger index
 * @param t  active track
 */
void sendFeedback(ORF24 *rf, FingerData f, std::vector<int> *i, int t)
{
	FingerEvent e = f[t][i->at(t)];
	unsigned char command = e.getCommand();
	unsigned char finger = e.getData();
	unsigned char payload = 0;

	if (t) // Left hand
	{
		rf->openWritingPipe("ArS02");
	}
	else // Right hand
	{
		finger = inverse(finger);
		rf->openWritingPipe("ArS01");
	}

	if (command == 0x80 || command == 0x90)
	{
		payload = command | (finger - 1) * 2;
		rf->write(&payload, 1);
	}

	i->at(t)++;
}

/**
 * Inverse Finger Number
 *
 * Command for right hand module need to be inverted before sending.
 * This function will do the inversion.
 * 
 * @param  finger finger number
 * @return        inverted finger number
 */
unsigned char inverse(unsigned char finger)
{
	unsigned char inv = 0;

	switch (finger)
	{
		case 1:		inv = 5;	break;
		case 2:		inv = 4;	break;
		case 3:		inv = 3;	break;
		case 4:		inv = 2;	break;
		case 5:		inv = 1;	break;	
	}

	return inv;
}