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

#ifndef _FINGER_DATA_H
#define _FINGER_DATA_H

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <cstdio>

class FingerEvent
{
private:
	char data;
public:
	void setData(char d);
	char getData(void);
	bool isCheckpoint(void);
};

class FingerTrack
{
private:
	int trackLength;
	std::vector<FingerEvent> events;
public:
	void setTrackLength(char *p);
	int getTrackLength(void);
	void push(FingerEvent event);
	FingerEvent operator[](int i);
};

class FingerData
{
private:
	int trackCount;
	std::vector<FingerTrack> tracks;
	char *open(std::string filepath);
	void parse(char *buffer);

public:
	FingerData(std::string filepath);
	int getTrackCount(void);
	FingerTrack operator[](int i);
	void printAllEvents(void);
};

#endif