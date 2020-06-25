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

private:
	//Header
	wxString filename;
	uint32_t filesize;

	//SubChunk1
	uint32_t chunk1Size;
	unsigned short audioFormat;
	unsigned short channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;

};

