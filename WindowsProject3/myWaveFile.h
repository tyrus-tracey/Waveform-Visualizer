#pragma once
#include "wx/wfstream.h"

class myWaveFile : public wxFFile
{
public:
	myWaveFile(wxString filepath);
	~myWaveFile();
	bool readHeader();
	void readSubChunk1();
	void readSubChunk2();
	int getSampleCount();
	long getDataAmplitude(int index);

private:
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
	unsigned int* dataArray8b = NULL;
	short* dataArray16b = NULL;
	long* sampleData = NULL;
};

