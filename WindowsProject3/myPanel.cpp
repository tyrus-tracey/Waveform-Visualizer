#include "myPanel.h"

//Bind paint events to this panel
wxBEGIN_EVENT_TABLE(myPanel, wxPanel)
EVT_PAINT(myPanel::paintEvent)
END_EVENT_TABLE()

//Using the base class constructor, create a panel and an associated myWaveFile as a child
//The parent/child relationship allows for multiple panels to open with each one 
//	having it's own waveform. However, currently this is not implemented.
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
	delete wavFile; //required to delete manually allocated elements of myWaveFile
}

// 'dc' is a device context (the screen) that is used for drawing on the panel
void myPanel::paintEvent(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	PrepareDC(dc);
	drawBackground(dc);
	drawWaveform(dc);
}

// Prepare a clean background to draw the waveform on.
void myPanel::drawBackground(wxDC& dc)
{
	wxBrush brush = dc.GetBrush();	// Rectangle body 
	wxPen pen = dc.GetPen();		// Rectangle border
	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(wxColour(100,150,200));
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.DrawRectangle(wxRect(maxSize));	//	Since this is purely a visual element,
										//	I should be allowed this convenience and not have to 
										//	manually draw a rectangle.

	return;
}

/*
	Step 1: Read data from inputted wave file.
	Step 2: Resize wave file data to within the screen limits.
	Step 3: Display waveform.
*/
void myPanel::drawWaveform(wxDC& dc) {

	// Read wave file
	if (wavFile->IsOpened()) {
		if (!wavFile->readHeader()) {
			wxMessageBox("Error: Unable to parse header");
		}
		else {
			wavFile->readSubChunk1();
			wavFile->readSubChunk2();
			wavFile->constrainToScreen(maxWidth, maxHeight);
			wavFile->fadeIn(maxWidth, maxHeight);
			wavFile->fadeOut(maxWidth, maxHeight);
		}
	}
	
	// Ensure wave file has been read successfully before drawing.
	// For each pixel across the width of the panel, display amplitude of wave file data.
	if (!wavFile->IsOpened()) {
		for (int i = 0; i < GetSize().x; i++) {
			int amplitude = wavFile->getDataAmplitude(i) / 2; // Center waveform to panel midline
			dc.DrawLine(wxPoint(i, midHeight - amplitude), wxPoint(i, midHeight + amplitude));
		}
		drawMidline(dc);
		displayInfo(dc);
	}

}

// Display wave file parameters.
void myPanel::displayInfo(wxDC& dc)
{
	wxString displayText;
	wxString format = (wavFile->getAudioFormat() == 1 ? "PCM" : "Unknown");
	wxString channels = (wavFile->getChannels() == 1 ? "Mono" : "Stereo");
	wxString sampleRate = wxString::Format(wxT("%i"), wavFile->getSampleRate());
	wxString byteRate = wxString::Format(wxT("%i"), wavFile->getByteRate());
	wxString bitsPerSample = wxString::Format(wxT("%i"), wavFile->getBitsPerSample());
	wxString numberOfSamples = wxString::Format(wxT("%i"), wavFile->getNumberOfSamples());
	wxString maxAmplitude = wxString::Format(wxT("%i"), wavFile->getMaxAmplitude());

	//Draw mini-window
	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();
	pen.SetWidth(5);
	pen.SetColour(wxColour(100, 150, 200));
	brush.SetColour(wxColour(190, 210, 255));
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, 250, 155);

	//Display wave file information
	dc.DrawText(wavFile->GetName(), wxPoint(10, 5));
	displayText = "Audio Format: " + format;
	dc.DrawText(displayText, wxPoint(10, 23));
	displayText = "Channels: " + channels;
	dc.DrawText(displayText, wxPoint(10, 41));
	displayText = "Sample Rate: " + sampleRate + "Hz";
	dc.DrawText(displayText, wxPoint(10, 59));
	displayText = "Avg. Byte Rate: " + byteRate;
	dc.DrawText(displayText, wxPoint(10, 77));
	displayText = "Bits Per Sample: " + bitsPerSample + " bits";
	dc.DrawText(displayText, wxPoint(10, 95));
	displayText = "# of Samples: " + numberOfSamples;
	dc.DrawText(displayText, wxPoint(10, 113));
	displayText = "Max Amplitude: " + maxAmplitude;
	dc.DrawText(displayText, wxPoint(10, 131));

	return;
}

// Draw a line representing 0 amplitude.
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