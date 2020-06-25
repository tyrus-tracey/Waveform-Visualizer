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
	else {
		dc.DrawText("Success.", wxPoint(100, 100));
	}
	
}

wxFFile* myPanel::getFile()
{
	return wavFile;
}

bool myPanel::readHeader()
{
	char buffer[4];
	filesize = new unsigned long;
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

bool myPanel::isLoaded()
{
	return loaded;
}

void myPanel::nowLoaded()
{
	loaded = true;
	return;
}
