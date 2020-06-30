#include "myPanel.h"

wxBEGIN_EVENT_TABLE(myPanel, wxPanel)
EVT_PAINT(myPanel::paintEvent)
END_EVENT_TABLE()

myPanel::myPanel(wxFrame* parent, const wxString filepath) 
	: wxPanel(parent, wxID_ANY, wxPoint(0,0), parent->GetSize())
{
	wavFile = new myWaveFile(filepath);
	if(wavFile == NULL) {
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
	delete wavFile;
}

void myPanel::paintEvent(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	PrepareDC(dc);
	render(dc);
	
	drawTest(dc);
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

	// Read wave file
	if (wavFile->IsOpened()) {
		if (!wavFile->readHeader()) {
			wxMessageBox("Error: Unable to parse header");
		}
		else {
			wavFile->readSubChunk1();
			wavFile->readSubChunk2();
			wavFile->constrainWidth(maxWidth);
		}
	}
	
	if (!wavFile->IsOpened()) {
		for (int i = 0; i < GetSize().x; i++) {
			int amplitude = wavFile->getDataAmplitude(i) / 2;
			dc.DrawLine(wxPoint(i, midHeight - amplitude), wxPoint(i, midHeight + amplitude));
		}
		drawMidline(dc);
		displayInfo(dc);
	}

}

void myPanel::displayInfo(wxDC& dc)
{
	wxString displayText;
	wxString format = (wavFile->getAudioFormat() == 1 ? "PCM" : "Unknown");
	wxString channels = (wavFile->getChannels() == 1 ? "Mono" : "Stereo");
	wxString sampleRate = wxString::Format(wxT("%i"), wavFile->getSampleRate());
	wxString byteRate = wxString::Format(wxT("%i"), wavFile->getByteRate());
	wxString bitsPerSample = wxString::Format(wxT("%i"), wavFile->getBitsPerSample());
	wxString numberOfSamples = wxString::Format(wxT("%i"), wavFile->getNumberOfSamples());

	//Draw mini-window
	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();
	pen.SetWidth(5);
	pen.SetColour(wxColour(100, 150, 200));
	brush.SetColour(wxColour(190, 210, 255));
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, 250, 175);

	//Display wave file information
	dc.DrawText(wavFile->GetName(), wxPoint(20, 20));
	displayText = "Audio Format: " + format;
	dc.DrawText(displayText, wxPoint(20, 40));
	displayText = "Channels: " + channels;
	dc.DrawText(displayText, wxPoint(20, 60));
	displayText = "Sample Rate: " + sampleRate + "Hz";
	dc.DrawText(displayText, wxPoint(20, 80));
	displayText = "Avg. Byte Rate: " + byteRate;
	dc.DrawText(displayText, wxPoint(20, 100));
	displayText = "Bits Per Sample: " + bitsPerSample + " bits";
	dc.DrawText(displayText, wxPoint(20, 120));
	displayText = "# of Samples: " + numberOfSamples;
	dc.DrawText(displayText, wxPoint(20, 140));

	return;
}

void myPanel::drawMidline(wxDC& dc)
{
	wxPen pen = dc.GetPen();
	pen.SetColour(wxColour(255, 125, 150));
	dc.SetPen(pen);
	dc.DrawLine(wxPoint(0, midHeight), wxPoint(maxWidth, midHeight));
	return;
}

myWaveFile* myPanel::getFile()
{
	return wavFile;
}