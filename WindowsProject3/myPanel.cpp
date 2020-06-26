#include "myPanel.h"

wxBEGIN_EVENT_TABLE(myPanel, wxPanel)
EVT_PAINT(myPanel::paintEvent)
END_EVENT_TABLE()

myPanel::myPanel(wxFrame* parent, myWaveFile* audioFile) 
	: wxPanel(parent, wxID_ANY, wxPoint(0,0), wxSize(500,500))
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

	dc.DrawRectangle(wxRect(maxSize));
	
}

void myPanel::drawTest(wxDC& dc) {
	dc.DrawText(wavFile->GetName(), wxPoint(40, 60));

	// Read wave file
	if (!wavFile->readHeader()) {
		wxMessageBox("Error: Unable to parse header");
	}

	wavFile->readSubChunk1();
	wavFile->readSubChunk2();


	//Display waveform
	for (int i = 0; i < wavFile->getSampleCount(); i++) {
		int amplitude = wavFile->getDataAmplitude(i) / 2;
		dc.DrawLine(wxPoint(i, midHeight-amplitude), wxPoint(i, midHeight+amplitude));
	}

	//Draw cutoff after last sample
	wxPen pen = dc.GetPen();
	pen.SetStyle(wxPENSTYLE_DOT);
	pen.SetColour(wxT("RED"));
	dc.SetPen(pen);

}

myWaveFile* myPanel::getFile()
{
	return wavFile;
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
