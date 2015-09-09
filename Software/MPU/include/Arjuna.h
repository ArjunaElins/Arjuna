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
#include <tclap/CmdLine.h>
#include <wiringPi.h>

#include "MidiIO.h"
#include "ORF24.h"
#include "WiringPiKeypad"

struct Args {
	bool debugEnabled;
	bool keyboardEnabled;
};

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
 * Get Command Line Arguments
 *
 * This function uses TCLAP library to parse command line arguments
 * 
 * @param  argc arguments count
 * @param  argv arguments vector
 * @return      parsed arguments
 */
struct Args getArgs(int argc, char *argv[]);

/**
 * Initial Hardware Setup
 *
 * This function initialize the device to interface with hardware.
 * 
 * @return setup status
 */
int initHardware(void);

/**
 * Setup MIDI Input/Output
 * 
 * @return  status
 */
int MidiIOSetup(void);

/**
 * Setup nRf24L01+ Radio Transceiver
 * 
 * @return  status
 */
int RadioSetup(void);

/**
 * Setup keypad matrix
 * 
 * @return  status
 */
int keypadSetup(void);

#endif