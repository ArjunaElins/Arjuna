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

#ifndef _ARJUNA_H_
#define _ARJUNA_H_

#include <iostream>
#include <fstream>
#include <string>

#include "Setup.h"
#include "MidiFile.h"
#include "FingerData.h"

/**
 * Main Function
 *
 * This is the starting point of the program.
 * 
 * @param  argc arg count
 * @param  argv arg vector
 * @return      status
 */
int main(int argc, char *argv[]);

/**
 * Start Application Routine
 *
 * This routine start by showing main menu. It will handle user input
 * and call other methods depending on the input.
 */
void startRoutine(struct Container *container);

/**
 * Show Application Menu
 *
 * In this screen, user can select the application operation.
 */
void showMenu(void);

/**
 * Song Selector
 *
 * This method will start the song selector.
 * 
 * @param container hardware handler
 */
std::string songSelector(struct Container *container);

/**
 * Print Song List
 *
 * This method will parse every line of the file handler and print it
 * to stdout
 * 
 * @param songList [description]
 */
void printSongList(std::ifstream *songList);

/**
 * Select Song
 *
 * This method will ask the user to choose from opened song list
 * 
 * @param  songList song list handler
 * @return          song name
 */
std::string selectSong(struct Container *container, std::ifstream *songList);

/**
 * Song Player
 *
 * This method is used to play MIDI song. It will open the MIDI file,
 * calculate some numbers, and send MIDI message to output port
 * 
 * @param  container hardware handler
 * @param  songPath  MIDI song location
 */
void songPlayer(struct Container *container, std::string songPath);

/**
 * Get Tempo in Second Per Tick
 * 	
 * @param e   	MidiEvent
 * @param tpq 	tick peq quarter
 * @param spt 	second per tick
 */
void getTempoSPT(MidiEvent e, int tpq, double *spt);

/**
 * Send MIDI Message
 *
 * This method will form a MIDI message container and send it to MIDI output port
 * 
 * @param io MIDI i/O port
 * @param e  MIDI event
 */
void sendMidiMessage(MidiIO *io, MidiEvent e);

/**
 * Skip Finger Metadata
 * 
 * @param finger finger data
 * @param i      finger index
 * @param t      active track
 */
void skipFingerMetadata(FingerData finger, std::vector<int> *i, int t);

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
void sendFeedback(ORF24 *rf, FingerData f, std::vector<int> *i, int t);

/**
 * Inverse Finger Number
 *
 * Command for right hand module need to be inverted before sending.
 * This function will do the inversion.
 * 
 * @param  finger finger number
 * @return        inverted finger number
 */
unsigned char inverse(unsigned char finger);

#endif