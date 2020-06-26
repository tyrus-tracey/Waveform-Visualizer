#include "myWaveFile.h"
#include "wx/wx.h"
#include "intrin.h"

myWaveFile::myWaveFile(wxString filepath)
	: wxFFile(filepath, "r")
{
	filename = filepath;
	filesize = 0; 

	chunk1Size = 0;
	audioFormat = 0;
	channels = 0;
	sampleRate = 0;
	byteRate = 0;
	blockAlign = 0; 
	bitsPerSample = 0;

	chunk2Size = 0;
}

myWaveFile::~myWaveFile() {

}

bool myWaveFile::readHeader() {
	char buffer4B[4];

	Read(buffer4B, sizeof(buffer4B));
	if (!strcmp(buffer4B, "RIFF")) {
		return false;
	}

	Read(buffer4B, sizeof(buffer4B));
	filesize = *buffer4B;

	Read(buffer4B, sizeof(buffer4B));
	if (!strcmp(buffer4B, "WAVE")) {
		return false;
	}

	return true;
}

void myWaveFile::readSubChunk1() {
	//verify fmt
	char buffer[4];

	Read(buffer, sizeof(buffer));
	if (!strcmp(buffer, "fmt ")) {
		wxMessageBox("Error: fmt missing");
		return;
	}

	uint32_t* buffer4B = new uint32_t;
	unsigned char buffer2B[2];

	//Subchunk 1 Size
	Read(buffer4B, sizeof(buffer4B));
	chunk1Size = *buffer4B;

	//Audio Format (PCM == 1)
	Read(buffer2B, sizeof(buffer2B));
	audioFormat = *buffer2B;

	//Channels 
	Read(buffer2B, sizeof(buffer2B));
	channels = *buffer2B;

	//Sample Rate
	Read(buffer4B, sizeof(buffer4B));
	sampleRate = *buffer4B;

	//Byte Rate (Hz)
	Read(buffer4B, sizeof(buffer4B));
	byteRate = *buffer4B;

	//Block Align (bytes per sample)
	Read(buffer2B, sizeof(buffer2B));
	blockAlign = *buffer2B;

	//Bits per sample
	Read(buffer2B, sizeof(buffer2B));
	bitsPerSample = *buffer2B;

	delete buffer4B;
}

void myWaveFile::readSubChunk2() {
	char buffer1B[1];
	char buffer2B[2];
	char buffer4B[4];
	uint32_t* intBuffer4B = new uint32_t;

	Read(buffer4B, sizeof(buffer4B));
	if (!strcmp(buffer4B, "data")) {
		wxMessageBox("Error: Unable to locate data subchunk");
		return;
	}

	Read(intBuffer4B, sizeof(intBuffer4B));
	chunk2Size = *intBuffer4B;

	numberOfSamples = chunk2Size / bitsPerSample * 8;


	//Begin reading audio data
	if (bitsPerSample == 8) {
		dataArray8b = new unsigned int[numberOfSamples];
		int i = 0;
		while (!Eof()) {
			Read(buffer1B, sizeof(buffer1B));
			dataArray8b[i++] = *buffer1B;
		}
	}
	else if (bitsPerSample == 16) {
		dataArray16b = new short[numberOfSamples];
		int i = 0;
		while (!Eof()) {
			Read(buffer2B, sizeof(buffer2B));
			dataArray16b[i++] = *buffer2B;
		}
	}

}

int myWaveFile::getSampleCount()
{
	return numberOfSamples;
}

short myWaveFile::getDataAmplitude(int index)
{
	if (0 <= index && index <= numberOfSamples) {
		if (bitsPerSample == 8) {
			return dataArray8b[index];
		}
		else if (bitsPerSample == 16) {
			return dataArray16b[index];
		}
	}
	return -1;
}

