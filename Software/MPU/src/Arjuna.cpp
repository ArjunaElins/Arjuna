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

		if (keypress == SELECT_SONG_BUTTON)
		{
			songPath.assign(songSelector(container));
		}
		else if (keypress == PLAY_SONG_BUTTON)
		{
			startMPA(container, songPath, PLAYER);
		}
		else if (keypress == EVALUATOR_BUTTON)
		{
			startMPA(container, songPath, EVALUATOR);
		}
		else if (keypress == STOP_BUTTON)
		{
			return;
		}
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
	} while (keypress != SELECT_SONG_BUTTON);

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

/* Start MIDI Processing Algorithm
 *
 * This function is a bootstrap for the MIDI Processing Algorithm.
 * It will call the player or the evaluator, depending on user selection
 * 
 * @param container hadrware handler
 * @param songPath  selected song path
 * @param mode      MPA operation mode
 */
void startMPA(Container *container, std::string songPath, MPUOperation operation)
{
	MidiFile midi(songPath + ".mid");
	FingerData finger(songPath + ".fgr");

	PlayMode mode = getPlayMode(container->keypad);
	setPlayMode(&midi, mode);

	if (operation == PLAYER)
	{
		std::cout << "Playing song \"" + songPath + "\"..." << std::endl;
		play(container, &midi, &finger, mode);
	}
	else
	{
		std::cout << "Evaluating song \"" + songPath + "\"..." << std::endl;
		evaluate(container, &midi, &finger, mode);
	}
}

/**
 * Song Player
 *
 * This method is used to play MIDI song. It will open the MIDI file,
 * calculate some numbers, and send MIDI message to output port
 * 
 * @param  container hardware handler
 * @param  midi 	 MIDI file handler
 * @param  finger 	 finger data handler
 * @param  mode 	 selected play mode
 */
void play(Container *container, MidiFile *midi, FingerData *finger, PlayMode mode)
{
	int tpq = midi->getTicksPerQuarterNote();
	double spt = 0.5 / tpq;
	int t = (mode == LEFT_HAND) ? 1 : 0;
	std::vector<char> f(2, 0);

	delay(500);
	for (int e = 0; e < (*midi)[t].getSize(); e++)
	{
		delayMicroseconds(spt * midi->getEvent(t, e).tick * 1000000);
		if (midi->getEvent(t, e).isMeta())
			continue;
		sendMidiMessage(container->io, midi->getEvent(t, e));
 
 		if (midi->getEvent(t, e).isNoteOn())
 		{
	 		int ft = (mode == BOTH_HANDS) ? midi->getSplitTrack(t, e) : t;
			sendFeedback(container->rf, finger->getData(ft, f[ft]++), ft, true);
 		}
	}
}

/**
 * Song Evaluator
 *
 * This function is used to compare MIDI input with MIDI data and give
 * response to hands odule
 * @param container handware handler
 * @param midi      MIDI file handler
 * @param finger    finger data handler
 * @param mode      selected play mode
 */
void evaluate(Container *container, MidiFile *midi, FingerData *finger, PlayMode mode)
{
	int t = (mode == LEFT_HAND) ? 1 : 0;
	int m = 0;
	std::vector<char> f(2, 0);
	bool status = true;

	while (status)
	{
		std::vector<Key> keys;
		status = getUnisonNote(midi, &m, t, &keys);
		getUnisonFinger(finger, &f, &keys);
		getInputAndEvaluate(container, keys);
	}
}

/**
 * Get Unison Note
 *
 * This function groups note played at the same time
 * 
 * @param  midi MIDI file handler
 * @param  m    MIDI file index
 * @param  t    MIDI track number
 * @param  keys Keys container
 * @return      status
 */
bool getUnisonNote(MidiFile *midi, int *m, int t, std::vector<Key> *keys)
{
	bool status = true;
	do
	{
		Key key;

		if (midi->getEvent(t, *m).isNoteOn())
		{
			key.track = midi->getSplitTrack(t, *m);
			key.note = midi->getEvent(t, *m)[1];
			keys->push_back(key);
		}
		*m += 1;

		if (*m >= (*midi)[0].size())
			status = false;

	} while (midi->getEvent(t, *m).tick == 0 && status);

	return status;
}

/**
 * Get Unison Finger
 * This function groups finger played at the same time
 * 
 * @param finger finger data handler
 * @param f      finger data index
 * @param keys   Keys container
 */
void getUnisonFinger(FingerData *finger, std::vector<char> *f, std::vector<Key> *keys)
{
	for(unsigned int i = 0; i < keys->size(); i++)
	{
		int t = keys->at(i).track;
		keys->at(i).finger = finger->getData(t, f->at(t));
		f->at(t) += 1;
	}
}

/**
 * Get MIDI Input
 * 
 * @param io       MIDI IO handler
 * @param expected number of expected input
 */
void getInputAndEvaluate(Container *container, std::vector<Key> keys)
{
	unsigned int i = 0;

	while (i < keys.size())
	{
		std::vector<unsigned char> message;
		container->io->getMessage(&message);

		if (message.size() > 0)
		{
 			if (message[0] == 0x90)
			{
				if (! compare(container->rf, &keys, message[1]))
				{
					printf("Wrong.\nExpected: ");
					for (unsigned int i = 0; i < keys.size(); i++)
						printf("%X ", keys[i].note);
					
					printf("\nReceived: %X\n", message[1]);
				}
			}
		}

		delay(10);
	}
}

/**
 * Compare MIDI Input with MIDI Data
 *
 * @param  rf 	Trasceiver handler
 * @param  keys MIDI Data
 * @param  note MIDI Input
 * @return      Compare result
 */
bool compare(ORF24 *rf, std::vector<Key> *keys, unsigned char note)
{
	bool wrong = true;
	bool right = true;

	for (unsigned int i = 0; i < keys->size(); i++)
	{
		if (keys->at(i).note != note)
		{
			wrong = true;
			right = (note > keys->at(i).note) ? true : false;  
			sendFeedback(rf, keys->at(i).finger, keys->at(i).track, right);			
		}
		else
		{
			wrong = false;
			keys->erase(keys->begin() + i);
			break;
		}
	}

	return !wrong;
}

/**
 * Get Play Mode
 *
 * This function ask the user to select the play mode
 * 
 * @param  keypad 	keypad handler
 * @return          play mode
 */
PlayMode getPlayMode(WiringPiKeypad *keypad)
{
	std::cout << "Select Play Mode." << std::endl
			  << " A - Both hands" << std::endl
			  << " B - Right hands" << std::endl
			  << " C - Left hands" << std::endl;

	char keypress;
	PlayMode mode = BOTH_HANDS;

	keypress = keypad->getKey();

	if (keypress == BOTH_HANDS_MODE_BUTTON)
		mode = BOTH_HANDS;
	else if (keypress == RIGHT_HAND_MODE_BUTTON)
		mode = RIGHT_HAND;
	else if (keypress == LEFT_HAND_MODE_BUTTON)
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
	midi->deltaTicks();

	if (mode == BOTH_HANDS)
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
 * Send Feedback to Hand Module
 *
 * This method use the radio transceiver to send payload to hand module
 * 
 * @param rf 		radio handler
 * @param f  		finger data
 * @param t  		active track
 * @param right 	turn on right vibrator. If false, then turn on left vibrator
 */
void sendFeedback(ORF24 *rf, char f, int t, bool right)
{
	const unsigned char command = 0x90;
	unsigned char payload = 0;

	if (t) // Left hand
	{
		rf->openWritingPipe("ArS02");
	}
	else // Right hand
	{
		f = inverse(f);
		rf->openWritingPipe("ArS01");
	}
	printf("Feedback: %X %X\n", t, f);

	if (right)
		payload = command | (f * 2 - 1);
	else
		payload = command | (f * 2 - 2);

	rf->write(&payload, 1);
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

/**
 * Keypad Handler
 *
 * This function should be called in separate thread to wair for user action
 * while doing other operation
 * 
 * @param keypad     keypad handler
 * @param keypress   keypress handler
 * @param terminator keypad terminator
 */
void keypadHandler(WiringPiKeypad *keypad, char *keypress, bool *terminator)
{
	while (1)
	{
		*keypress = keypad->getKey(terminator);

		if (*keypress == STOP_BUTTON)
		{
			break;
		}
	}
}