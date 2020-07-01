#pragma once
#include "wx/wfstream.h"

/*
	File class that stores and manipulates information from a wave file.
*/

class myWaveFile : public wxFFile
{
public:
	myWaveFile(wxString filepath);
	~myWaveFile();
	bool readHeader();
	void readSubChunk1();
	void readSubChunk2();
	
	void constrainToScreen(const int screenWidth, const int screenHeight);
	long getDataAmplitude(const int index) const;
	unsigned short getAudioFormat() const;
	unsigned short getChannels() const;
	uint32_t getSampleRate() const;
	uint32_t getByteRate() const;
	unsigned short getBitsPerSample() const;
	int getNumberOfSamples() const;

private:
	long getMaxAmplitude(const int screenWidth) const;
	void constrainWidth(const int screenWidth);
	void constrainHeight(const int screenWidth, const int screenHeight);

	//Header
	uint32_t filesize;

	//SubChunk1
	uint32_t chunk1Size;
	unsigned short audioFormat;
	unsigned short channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	int numberOfSamples;
	
	//SubChunk2
	uint32_t chunk2Size;
	uint8_t * dataArray8b = NULL;
	short* dataArray16b = NULL;
	long* sampleData = NULL;
};

