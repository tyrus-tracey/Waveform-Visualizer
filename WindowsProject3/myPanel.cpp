#include "myPanel.h"

wxBEGIN_EVENT_TABLE(myPanel, wxPanel)
EVT_PAINT(myPanel::paintEvent)
END_EVENT_TABLE()

myPanel::myPanel(wxFrame* parent, wxFFile* audioFile) : wxPanel(parent), wavFile(audioFile)
{
	wavFile = audioFile;
	if (wavFile != NULL) {
		loaded = true;
	}
	else {
		wxMessageBox("Error: Could not find associated WAVE file.");
	}
	maxSize = parent->GetSize();
	SetSize(maxSize);
	midHeight	= maxSize.y / 2;
	maxHeight	= maxSize.y;
	midWidth	= maxSize.x / 2;
	maxWidth	= maxSize.x;
	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

myPanel::~myPanel()
{
	delete filesize;
}

void myPanel::paintEvent(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	PrepareDC(dc);
	render(dc);
	if (loaded) {
		drawTest(dc);
	}
}

void myPanel::render(wxDC& dc)
{
	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();
	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(wxColour(100,150,200));
	dc.SetPen(pen);
	dc.SetBrush(brush);

	//dc.DrawText(wxT("Testing"), 40, 60);
	dc.DrawRectangle(wxRect(maxSize));

	//draw a crosshair
	dc.DrawLine(wxPoint(midWidth - 10, midHeight), wxPoint(midWidth + 10, midHeight));
	dc.DrawLine(wxPoint(midWidth, midHeight - 10), wxPoint(midWidth, midHeight + 10));
	
}

void myPanel::drawTest(wxDC& dc) {
	dc.DrawText(wavFile->GetName(), wxPoint(40, 60));

	if (!readHeader()) {
		wxMessageBox("Error: Unable to parse header");
	}
	
	readChunk1();

}

wxFFile* myPanel::getFile()
{
	return wavFile;
}

bool myPanel::readHeader()
{
	char buffer[4];
	filesize = new uint32_t;
	wavFile->Read(buffer, sizeof(buffer));
	if (!strcmp(buffer, "RIFF")) {
		return false;
	}

	wavFile->Read(filesize, sizeof(filesize));

	wavFile->Read(buffer, sizeof(buffer));
	if (!strcmp(buffer, "WAVE")) {
		return false;
	}

	return true;
}

void myPanel::readChunk1() {
	//verify fmt
	char buffer[4];

	wavFile->Read(buffer, sizeof(buffer));
	if (!strcmp(buffer, "fmt ")) {
		wxMessageBox("Error: fmt missing");
		return;
	}

	uint32_t* buffer4B = new uint32_t;
	unsigned char buffer2B[2];

	//Subchunk 1 Size
	wavFile->Read(buffer4B, sizeof(buffer4B));
	uint32_t chunk1Size = *buffer4B;

	//Audio Format (PCM == 1)
	wavFile->Read(buffer2B, sizeof(buffer2B));
	unsigned short audioFormat = *buffer2B;

	//Channels 
	wavFile->Read(buffer2B, sizeof(buffer2B));
	unsigned short channelNo = *buffer2B;

	//Sample Rate
	wavFile->Read(buffer4B, sizeof(buffer4B));
	uint32_t sampleRate = *buffer4B;

	//Byte Rate (Hz)
	wavFile->Read(buffer4B, sizeof(buffer4B));
	uint32_t byteRate = *buffer4B;

	//Block Align (bytes per sample)
	wavFile->Read(buffer2B, sizeof(buffer2B));
	unsigned short blockAlign = *buffer2B;

	//Bits per sample
	wavFile->Read(buffer2B, sizeof(buffer2B));
	unsigned short bitsPerSample = *buffer2B;


	delete buffer4B, buffer2B;
}

bool myPanel::isLoaded()
{
	return loaded;
}

void myPanel::nowLoaded()
{
	loaded = true;
	return;
}
