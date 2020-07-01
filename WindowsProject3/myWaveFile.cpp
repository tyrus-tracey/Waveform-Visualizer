#include "myWaveFile.h"
#include "wx/wx.h"
#include "intrin.h"

// Using base class constructor, open the wave file in binary mode
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
	maxAmplitude = 0;
}

// Since this was allocated manually this must also be deleted manually.
myWaveFile::~myWaveFile() {
	delete[] sampleData;
}

// Check that selected file is indeed a wave file.
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

// Read subchunk 1 to myWaveFile member variables
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
// Read subchunk 2 to myWaveFile member variables.
void myWaveFile::readSubChunk2() {
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
	long max = 0;
	//Begin reading audio data
	if (bitsPerSample == 8) {
		dataArray8b = new uint8_t[numberOfSamples];
		uint8_t* ptr = dataArray8b;
		for (i = 0; i < numberOfSamples; i++) {
			Read(ptr, 1);
			if (abs(*ptr) > max) { max = *ptr; }
			ptr++;
		}
	}
	else if (bitsPerSample == 16) {
		dataArray16b = new short[numberOfSamples];
		short* ptr = dataArray16b;
		for (i = 0; i < numberOfSamples; i++) {
			Read(ptr, 2);
			if (abs(*ptr) > max) { max = *ptr; }
			ptr++;
		}
	}
	maxAmplitude = max;
	Close(); // Indicate successful read
}

// Compress raw samples to fit within screen width, by averaging values.
// Residue samples are ignored, but this is only a small percentage of samples.
// Raw samples are grouped into bins of size scaleFactor, with the number of bins equal to screenWidth
// Means are taken from each bin and then saved to sampleData array.
void myWaveFile::constrainWidth(const int screenWidth)
{
	int residue = numberOfSamples % screenWidth; //Allows clean division of samples by number of bins
	int scaleFactor = numberOfSamples / screenWidth;
	int maxIndex = numberOfSamples - residue - scaleFactor;
	sampleData = new long[screenWidth];
	int count = 0; //Keep track of what index of sampleData we've written to
	long sum = 0; //Avoid potential overflows by using long
	long binMean = 0;

	if (screenWidth >= 2) { // Avoid potential memory overwrites
		int i = 0;
		for (int bin = 0; bin < maxIndex; bin += scaleFactor) {
			if (bitsPerSample == 8) {
				for (i = 0; i < scaleFactor; i++) {
					sum += long(dataArray8b[bin + i]);
				}
			}
			else if (bitsPerSample == 16) {
				for (i = 0; i < scaleFactor; i++) {
					sum += long(dataArray16b[bin + i]);
				}
			} 
			binMean = sum /= long(i);
			sampleData[count] = binMean;
			count++;
		}
	}
	else {
		wxMessageBox("Error: Panel size cannot support waveform display.");
	}

	// No longer require raw sample data
	delete[] dataArray8b; 
	delete[] dataArray16b;
	return;
}

// Constrain amplitudes of waveform to a range half the height of the screen
void myWaveFile::constrainHeight(const int screenWidth, const int screenHeight)
{
	double divisor = getMaxBinnedAmplitude(screenWidth) / (double(screenHeight/2) + 1);
	for (int i = 0; i < screenWidth; i++) {
		sampleData[i] /= divisor;
	}
	return;
}

// High-level function calls both contraining functions.
void myWaveFile::constrainToScreen(const int screenWidth, const int screenHeight)
{
	constrainWidth(screenWidth);
	constrainHeight(screenWidth, screenHeight);
	return;
}

// Returns sampleData array element from an index
long myWaveFile::getDataAmplitude(const int index) const
{
	if (0 <= index && index <= numberOfSamples) {
		return sampleData[index];
	}
	return -1;
}

// Finds the largest amplitude in order to resize amplitude range
long myWaveFile::getMaxBinnedAmplitude(const int screenWidth) const
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

long myWaveFile::getMaxAmplitude() const
{
	return maxAmplitude;
}

