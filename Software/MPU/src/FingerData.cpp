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

#include "FingerData.h"

FingerData::FingerData(std::string filepath)
{
	char *buffer = open(filepath);
	parse(buffer);
}

char *FingerData::open(std::string filepath)
{
	std::streampos size;
	char *buffer = 0;

	std::ifstream file(filepath, std::ios::in | std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		size = file.tellg();
		buffer = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();
	}

	return buffer;
}

void FingerData::parse(char *buffer)
{
	char *p = buffer;
	if (*p != 'M' && *(p+1) != 'T' && *(p+2) != 'f' && *(p+3) != 'g')
	{
		std::cout << "Bad finger data file.\n";
		return;
	}
	p += 4;

	trackCount = *(p++) << 8;
	trackCount += *(p++);

	for (int t = 0 ; t < trackCount; t++)
	{
		if (*p != 'M' && *(p+1) != 'T' && *(p+2) != 'r' && *(p+3) != 'k')
		{
			std::cout << "Bad finger data track.\n";
			return;
		}
		p += 4;

		FingerTrack track;
		track.setTrackLength(p);
		p += 4;

		for (int i = 0; i < track.getTrackLength(); i++)
		{
			FingerEvent event;
			event.setData(*(p++));
			track.push(event);
		}

		tracks.push_back(track);
	}
}

int FingerData::getTrackCount(void)
{
	return trackCount;
}

char FingerData::getData(int t, int e)
{
	return tracks[t][e].getData();
}

void FingerData::printAllEvents(void)
{
	for (int t = 0; t < trackCount; t++)
	{
		std::cout << "Track: " << t + 1 << std::endl;
		for (int e = 0; e < tracks[t].getTrackLength(); e++)
		{
			printf("%X ", tracks[t][e].getData());
		}
		std::cout << std::endl;
	}
}

FingerTrack FingerData::operator[](int i)
{
	return tracks[i];
}

void FingerTrack::setTrackLength(char *p)
{
	trackLength = *(p++) << 24;
	trackLength += *(p++) << 16;
	trackLength += *(p++) << 8;
	trackLength += *(p++);
}

int FingerTrack::getTrackLength(void)
{
	return trackLength;
}

void FingerTrack::push(FingerEvent event)
{
	events.push_back(event);
}

FingerEvent FingerTrack::operator[](int i)
{
	return events[i];
}

void FingerEvent::setData(char d)
{
	data = d;
}

char FingerEvent::getData(void)
{
	return data;
}

bool FingerEvent::isCheckpoint(void)
{
	if (data == 0xC0)
		return true;
	else
		return false;
}