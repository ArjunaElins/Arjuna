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
#include <thread>
#include <fstream>
#include <wiringPi.h>
#include "MidiFile.h"
#include "WiringPiKeypad.h"
#include "ORF24.h"
#include "MidiIO.h"
#include "FingerData.h"

struct Key
{
	int track;
	unsigned char note;
	unsigned char finger;
};

enum PlayMode {PLAY_ALL, PLAY_CHECKPOINT, EVALUATE_ALL, EVALUATE_CHECKPOINT};
enum Hand {RIGHT_HAND, LEFT_HAND};

int main(int argc, char *argv[]);
void menu(void);

/*
 |------------------------------------------------
 | Initialization and cleanup 	
 |------------------------------------------------
 */

void initMidiIO(void);
void initRadio(void);
void initRadio(bool debug);
void initKeypad(void);
void cleanup(void);

/*
 |------------------------------------------------
 | Player	
 |------------------------------------------------
 */

std::string selectSong(void);
void play(std::string songPath);
void startPlayer(std::string songPath, PlayMode mode);
void parseCommand(FingerEvent event, PlayMode mode, Hand hand);
unsigned char inverse(unsigned char finger);
void keypadHandler(char *keypress, bool *terminator);

/*
 |------------------------------------------------
 | Evaluator	
 |------------------------------------------------
 */
void evaluate(std::string songPath);
void startEvaluator(std::string songPath, PlayMode mode);
std::string getKey(int key);
void parseCommand(Hand hand, unsigned char in, unsigned char note, char finger);

#endif