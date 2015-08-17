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

		for (int i = 0; i < track.getTrackLength(); i += 2)
		{
			FingerEvent event;
			
			event.push(*(p++));
			event.push(*(p++));

			track.incEventLength();
			track.push(event);
		}

		tracks.push_back(track);
	}
}

int FingerData::getTrackCount(void)
{
	return trackCount;
}

void FingerData::printAllEvents(void)
{
	for (int t = 0; t < trackCount; t++)
	{
		std::cout << "Track: " << t + 1 << std::endl;
		std::cout << "Event length: " << tracks[t].getEventLength() << std::endl;
		for (int e = 0; e < tracks[t].getEventLength(); e++)
		{
			printf("%d\t%X\t%X\n", e, tracks[t][e].getCommand(), tracks[t][e].getData());
		}
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

void FingerTrack::incEventLength(void)
{
	eventLength++;
}

int FingerTrack::getEventLength(void)
{
	return eventLength;
}

FingerEvent FingerTrack::operator[](int i)
{
	return events[i];
}

void FingerEvent::push(char d)
{
	data.push_back(d);
}

char FingerEvent::getCommand(void)
{
	return data[0];
}

char FingerEvent::getData(void)
{
	return data[1];
}

bool FingerEvent::isNoteOn(void)
{
	if (data[0] == 0x90)
		return true;
	else
		return false;
}

bool FingerEvent::isNoteOff(void)
{
	if (data[0] == 0x80)
		return true;
	else
		return false;
}

bool FingerEvent::isCheckpoint(void)
{
	if (data[0] == 0xC0)
		return true;
	else
		return false;
}

char FingerEvent::operator[](int i)
{
	return data[i];
}