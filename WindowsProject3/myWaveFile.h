#pragma once
#include "wx/wfstream.h"

class myWaveFile : public wxFFile
{
public:
	myWaveFile();
	~myWaveFile();
	bool readHeader();
	void readSubChunk1();
	void readSubChunk2();

private:
	wxString filename;
	uint32_t filesize;
	uint32_t chunk1Size;
	unsigned short audioFormat;
	unsigned short channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;

};

