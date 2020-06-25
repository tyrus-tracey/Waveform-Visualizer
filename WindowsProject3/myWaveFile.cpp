#include "myWaveFile.h"
#include "wx/wx.h"

myWaveFile::myWaveFile(wxString filepath)
	: filename(filepath)
{
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
