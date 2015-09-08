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

	struct Args args;
	args.debugEnabled = enableDebugSwitch.getValue();
	args.keyboardEnabled = enableKeyboardSwitch.getValue();

	return args;
}