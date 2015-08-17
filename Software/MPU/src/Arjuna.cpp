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

MidiIO *io;
ORF24 *rf;
WiringPiKeypad *keypad;

int main(int argc, char *argv[])
{
	wiringPiSetup();
	initMidiIO();
	initRadio(true);
	initKeypad();

	char keypress;
	std::string songPath;

	while (1)
	{
		menu();
		keypress = keypad->getKey();

		if (keypress == 'A')
			songPath = selectSong();
		else if (keypress == 'B')
			play(songPath);
		else if (keypress == 'C')
			evaluate(songPath);
		else if (keypress == 'D')
			break;
	}

	cleanup();

	std::cout << "Exiting...\n";

	return 0;
}

void menu(void)
{
	std::cout << "\nMENU\n"
			  << " A - Select song\n"
			  << " B - Play song\n"
			  << " C - Start evaluating \n"
			  << " D - Exit\n";
}



/*
 |------------------------------------------------
 | Initialization and cleanup 	
 |------------------------------------------------
 */

void initMidiIO(void)
{
	std::cout << "Initializing MIDI I/O...\n";

	io = new MidiIO;
	io->openMidiOutPort();
	io->openMidiInPort();

	std::cout << "MIDI I/O iniitlized.\n";
}

void initRadio(void)
{
	initRadio(false);
}

void initRadio(bool debug)
{
	std::cout << "Initializing radio communication...\n";

	rf = new ORF24(21);

	if (debug)
		rf->enableDebug();

	rf->begin();
	rf->setPayloadSize(1);
	rf->setChannel(76);
	rf->setCRCLength(CRC_2_BYTE);
	rf->setPowerLevel(RF_PA_HIGH);

	std::cout << "Radio communication initialized.\n";
}

void initKeypad(void)
{
	std::cout << "Initializing keypad...\n";

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

	std::cout << "Keypad initialized.\n";
}

void cleanup(void)
{
	std::cout << "Cleaning up...\n";

	delete io;
	delete rf;
	delete keypad;
}



/*
 |------------------------------------------------
 | Player	
 |------------------------------------------------
 */

std::string selectSong(void)
{
	const std::string basedir = "/home/arjuna/Songs/";

	std::string song;
	ifstream songList(basedir + ".songlist");

	if (songList.is_open())
	{
		int i = 1;
		std::cout << "\nSong list:\n";

		while (getline(songList, song))
		{
			std::cout << i++ << ". " << song << std::endl;
		}

		std::cout << "Press number to select song. Press 'A' to select." << std::endl;

		char keypress;
		std::string songNumber;

		do
		{
			keypress = keypad->getKey();
			songNumber += keypress;
		} while (keypress != 'A');

		songNumber.pop_back();
		int number = std::stoi(songNumber);
		songList.clear();
		songList.seekg(0, std::ios::beg);

		for (int i = 0; i < number; i++)
		{
			getline(songList, song);
		}
		std::cout << "Selected: " << song << std::endl;

		std::string songPath = basedir + song + "/" + song;

		return songPath;
	}
	else
	{
		std::cout << "Error opening file.\n";
		exit(EXIT_FAILURE);
	}
}

void play(std::string songPath)
{
	std::cout << "Playing song \"" << songPath << "\"...\n";
	std::cout << "Press C to go to next checkpoint. Press D to stop.\n";

	startPlayer(songPath, PLAY_ALL);

	std::cout << "Song stopped.\n";
}

void startPlayer(std::string songPath, PlayMode mode)
{
	char keypress;
	bool terminator = true;
	std::thread input(keypadHandler, &keypress, &terminator);

	MidiFile midi(songPath + ".mid");
	FingerData finger(songPath + ".fgr");

	midi.deltaTicks();
	int fingerTrackCount = midi.getTrackCount();

	int iFinger[fingerTrackCount];
	for (int i = 0; i < fingerTrackCount; i++)
		iFinger[i] = 0;

	if (mode == PLAY_ALL)
		midi.joinTracks();

	int tpq = midi.getTicksPerQuarterNote();
	double spt = 0.5 / tpq;
	int trackCount = midi.getTrackCount();
	std::vector<unsigned char> message;

	int activeTrack;
	for (int t = 0; t < trackCount; t++)
	{
		for (int e = 0; e < midi[t].size(); e++)
		{
			if (midi[t][e].getTempoSPT(tpq) > 0)
				spt = midi[t][e].getTempoSPT(tpq);

			if (midi[t][e].isMeta())
				continue;

			for (unsigned int i = 0; i < midi[t][e].size(); i++)
				message.push_back((int) midi[t][e][i]);

			delayMicroseconds(spt * midi[t][e].tick * 1000000);

			activeTrack = midi.getSplitTrack(t, e);

			io->sendMessage(&message);
			if (finger[activeTrack][iFinger[activeTrack]].getCommand() == 0xF9)
				iFinger[activeTrack]++;

			parseCommand(finger[activeTrack][iFinger[activeTrack]++], mode, (Hand) activeTrack);

			message.clear();

			switch (keypress)
			{
				case 'C':
					delay(1000);
				case 'D':
					keypress = 0;
					input.join();
					return;
			}
		}
	}

	terminator = false;
	input.join();
}

void evaluate(std::string songPath)
{
	std::cout << "Evaluating song \"" << songPath << "\"...\n";
	std::cout << "Press C to go to next checkpoint. Press D to stop.\n";

	startEvaluator(songPath, EVALUATE_ALL);

	std::cout << "Song stopped.\n";
}

void startEvaluator(std::string songPath, PlayMode mode)
{
	char keypress;
	bool terminator = true;
	std::thread input(keypadHandler, &keypress, &terminator);

	MidiFile midi(songPath + ".mid");
	FingerData finger(songPath + ".fgr");

	int trackCount = midi.getTrackCount();
	int iFinger[trackCount];
	for (int i = 0; i < trackCount; i++)
		iFinger[i] = 0;

	midi.deltaTicks();
	midi.joinTracks();

	int iNote = 0;
	while (keypress != 'D')
	{
		int dump = 0;
		std::vector<Key> keys;
		std::vector<std::vector<unsigned char>> messages;

		do
		{
			Key key;

			if (midi[0][iNote].isNoteOn())
			{
				key = {midi.getSplitTrack(0, iNote), midi[0][iNote][1]};
				keys.push_back(key);
			}
			else
			{
				dump++;
			}
			iNote++;

			if (iNote >= midi[0].size())
			{
				terminator = false;
				input.join();
				return;
			}
		} while (midi[0][iNote].tick == 0);

		for (unsigned int i = 0; i < keys.size(); i++)
		{
			if (finger[keys[i].track][iFinger[keys[i].track]].getCommand() == 0x90)
			{
				keys[i].finger = finger[keys[i].track][iFinger[keys[i].track]].getData();
				iFinger[keys[i].track]++;
			}
			else
			{
				iFinger[keys[i].track]++;
				i--;
			}
		}

		if (keys.size())
		{
			std::cout << "Expected:\t";
			for (unsigned int i = 0; i < keys.size(); i++)
			{
				std::cout << getKey(keys[i].note);
				printf("-%X\t", keys[i].finger);
			}
			std::cout << std::endl;
		}


		unsigned int expected = 0;
		while (expected < keys.size())
		{
			std::vector<unsigned char> message;
			io->getMessage(&message);
			int nBytes = message.size();

			if (nBytes > 0)
			{
				if (message[0] == 0x90)
				{
					messages.push_back(message);
					expected++;
				}
			}

			delay(10);
		}

		if (messages.size())
		{
			std::cout << "Received:\t";
			for (unsigned int i = 0; i < messages.size(); i++)
				std::cout << getKey(messages[i][1]) << "\t";
			std::cout << std::endl << std::endl;
		}

		std::vector<bool> evaluation;
		for (unsigned int i = 0; i < keys.size(); i++)
		{
			unsigned char wrong = 0;
			for (unsigned int j = 0; j < messages.size(); j++)
			{
				if (keys[i].note == messages[j][1])
				{
					wrong = 0;
					break;
				}
				else
				{
					wrong = messages[j][1];
				}
			}

			evaluation.push_back(! wrong);

			if (! evaluation[i])
			{
				std::cout << "Wrong key on ";
				if (keys[i].track)
					std::cout << "left ";
				else
					std::cout << "right ";
				std::cout << "hand.\n";
				parseCommand((Hand) keys[i].track, wrong, keys[i].note, keys[i].finger);
				delayMicroseconds(200);
			}
		}

		for (unsigned int i = 0; i < evaluation.size(); i++)
		{
			if (! evaluation[i])
			{
				for (unsigned int j = 0; j < keys.size(); j++)
				{
					do
					{
						iFinger[keys[j].track]--;
					}
					while (finger[keys[j].track][iFinger[keys[j].track]].getCommand() != 0x90);
				}

				iNote -= (keys.size() + dump);
				break;
			}
		}
	}
}

std::string getKey(int key)
{
	int octave;
	int workNote;
	std::string note;

	octave = key / 12;
	workNote = key % 12;

	switch (workNote + 1)
	{
		case 1:		note = "C";		break;
		case 2:		note = "C#";	break;
		case 3:		note = "D";		break;
		case 4:		note = "D#";	break;
		case 5:		note = "E";		break;
		case 6:		note = "F";		break;
		case 7:		note = "F#";	break;
		case 8:		note = "G";		break;
		case 9:		note = "G#";	break;
		case 10:	note = "A";		break;
		case 11:	note = "A#";	break;
		case 12:	note = "B";		break;
	}

	note.append(to_string(octave));

	return note;
}

void parseCommand(FingerEvent event, PlayMode mode, Hand hand)
{
	unsigned char command = event.getCommand();
	unsigned char finger = event.getData();
	unsigned char payload = 0;

	if (hand == RIGHT_HAND)
	{
		finger = inverse(finger);
		rf->openWritingPipe("ArS01");
	}
	else
	{
		rf->openWritingPipe("ArS02");
	}

	if (mode == PLAY_ALL || mode == PLAY_CHECKPOINT)
	{
		if (command == 0x80 || command == 0x90)
		{
			payload = command | (finger - 1) * 2;
			rf->write(&payload, 1);
		}
	}
}

void parseCommand(Hand hand, unsigned char in, unsigned char note, char finger)
{
	unsigned char payload = 0;

	if (hand == RIGHT_HAND)
	{
		finger = inverse(finger);
		rf->openWritingPipe("ArS01");
	}
	else
	{
		rf->openWritingPipe("ArS02");
	}

	if (note > in)
		payload = 0x90 | (finger * 2 - 1);
	else
		payload = 0x90 | (finger * 2 - 2);

	printf("%X\n", payload);

	rf->write(&payload, 1);
}

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

void keypadHandler(char *keypress, bool *terminator)
{
	while (1)
	{
		*keypress = keypad->getKey(terminator);

		if (*keypress == 'D')
			break;
	}
}