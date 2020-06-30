#include "myWaveFile.h"
#include "wx/wx.h"
#include "intrin.h"
#include "math.h"

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
//BUG: does not properly handle stereo files at the moment, but outside assignment scope
//TODO: average amplitudes to a range within screen (-250...250)?
void myWaveFile::readSubChunk2() {
	char buffer1B[1];
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
		short* ptr = dataArray16b;
		for (i = 0; i < numberOfSamples; i++) {
			Read(ptr, 2);
			ptr++;
		}
	}
	Close();
}

void myWaveFile::constrainWidth(const int screenWidth)
{
	int residue = numberOfSamples % screenWidth;
	int scaleFactor = numberOfSamples / screenWidth;
	int maxIndex = numberOfSamples - residue - scaleFactor;
	long sum = 0;
	int j = 0;
	sampleData = new long[screenWidth];
	int count = 0;

	if (screenWidth >= 2) {
		for (int i = 0; i < maxIndex; i += scaleFactor) {
			for (j = 0; j < scaleFactor; j++) {
				sum += dataArray16b[i + j];
			}
			sum /= j;
			sampleData[count] = sum;
			count++;
		}
	}
	else {
		wxMessageBox("Error: Panel size cannot support waveform display.");
	}

	delete[] dataArray8b;
	delete[] dataArray16b;
	return;
}

void myWaveFile::constrainHeight(const int screenWidth, const int screenHeight)
{
	double divisor = getMaxAmplitude(screenWidth) / (double(screenHeight/2) + 1);
	for (int i = 0; i < screenWidth; i++) {
		long test = sampleData[i];
		sampleData[i] /= divisor;
		long test2 = sampleData[i];
	}
	return;
}

long myWaveFile::getDataAmplitude(const int index) const
{
	if (0 <= index && index <= numberOfSamples) {
		return sampleData[index];
	}
	return -1;
}

long myWaveFile::getMaxAmplitude(const int screenWidth) const
{
	long max = 0;
	for (int i = 0; i < screenWidth; i++) {
		if (abs(sampleData[i] > max)) {
			max = sampleData[i];
		}
	}
	return max;
}

unsigned short myWaveFile::getAudioFormat() const
{
	return audioFormat;
}

unsigned short myWaveFile::getChannels() const
{
	return channels;
}

uint32_t myWaveFile::getSampleRate() const
{
	return sampleRate;
}

uint32_t myWaveFile::getByteRate() const
{
	return byteRate;
}

unsigned short myWaveFile::getBitsPerSample() const
{
	return bitsPerSample;
}

int myWaveFile::getNumberOfSamples() const
{
	return numberOfSamples;
}

