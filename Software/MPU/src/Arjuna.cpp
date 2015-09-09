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
	struct Args args = getArgs(argc, argv);

	struct Container container;

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
void startRoutine(struct Container *container)
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
std::string songSelector(struct Container *container)
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
std::string selectSong(struct Container *container, std::ifstream *songList)
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