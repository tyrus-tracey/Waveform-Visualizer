#include "myWaveFile.h"
#include "wx/wx.h"
#include "intrin.h"

myWaveFile::myWaveFile(wxString filepath)
	: wxFFile(filepath, "rb")
{
	filesize = 0; 

	chunk1Size = 0;
	audioFormat = 0;
	channels = 0;
	sampleRate = 0;
	byteRate = 0;
	blockAlign = 0; 
	bitsPerSample = 0;
	numberOfSamples = 0;
	chunk2Size = 0;
}

myWaveFile::~myWaveFile() {
	delete[] sampleData;
}

bool myWaveFile::readHeader() {
	char buffer4B[5];

	Read(buffer4B, 4);
	buffer4B[4] = '\0';
	if (strcmp(buffer4B, "RIFF")) {
		return false;
	}

	Read(&filesize, sizeof(filesize));

	Read(buffer4B, 4);
	buffer4B[4] = '\0';
	if (strcmp(buffer4B, "WAVE")) {
		return false;
	}

	return true;
}

void myWaveFile::readSubChunk1() {
	//verify fmt
	char charBuffer[5];

	Read(charBuffer, 4);
	charBuffer[4] = '\0';
	if (strcmp(charBuffer, "fmt ")) {
		wxMessageBox("Error: fmt missing");
		return;
	}

	Read(&chunk1Size, 4);
	Read(&audioFormat, 2);
	Read(&channels, 2);
	Read(&sampleRate, 4);
	Read(&byteRate, 4);
	Read(&blockAlign, 2);
	Read(&bitsPerSample, 2);
}

void myWaveFile::readSubChunk2() {
	char buffer1B[1];
	char buffer4B[4];
	char charBuffer[5];

	Read(charBuffer, 4);
	charBuffer[4] = '\0';
	if (strcmp(charBuffer, "data")) {
		wxMessageBox("Error: Unable to locate data subchunk");
		return;
	}

	Read(&chunk2Size, sizeof(chunk2Size));

	numberOfSamples = (chunk2Size / channels) / (bitsPerSample / 8);

	int i = 0;
	//Begin reading audio data
	if (bitsPerSample == 8) {
		dataArray8b = new unsigned int[numberOfSamples];
		while (!Eof()) {
			Read(buffer1B, sizeof(buffer1B));
			dataArray8b[i++] = *buffer1B;
		}
	}
	else if (bitsPerSample == 16) {
		dataArray16b = new short[numberOfSamples];

		for (i = 0; i < numberOfSamples; i++) {
			Read(buffer4B, sizeof(buffer4B));
			dataArray16b[i] = *buffer4B;
		}
		
	}

	// Average out data points to fit on screen (1000px)
	int residue = numberOfSamples % 1000;
	int scaleFactor = numberOfSamples / 1000;
	long sum = 0;
	int j = 0;
	sampleData = new long[1000];
	int count = 0;

	for (i = 0; i < numberOfSamples-residue-scaleFactor; i+=scaleFactor) {
		for (j = 0; j < scaleFactor; j++) {
			sum += dataArray16b[i + j];
		}
		sum /= j;
		sampleData[count] = sum;
		count++;
	}
	
	delete[] dataArray8b;
	delete[] dataArray16b;
	Close();
}

int myWaveFile::getSampleCount()
{
	return numberOfSamples;
}

long myWaveFile::getDataAmplitude(int index)
{
	if (0 <= index && index <= numberOfSamples) {
		return sampleData[index];
	}
	return -1;
}

