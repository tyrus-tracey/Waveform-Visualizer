#pragma once
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/wfstream.h"
#include "myWaveFile.h"
#include "wx/panel.h"

/*
	Panels are windows within frames that allow for drawing operations.
*/

class myPanel : public wxPanel
{
public:
	myPanel(wxFrame* parent, const wxString filepath);
	~myPanel();

	void paintEvent(wxPaintEvent& event);
	void drawBackground(wxDC& dc);
	void drawWaveform(wxDC& dc);
	void displayInfo(wxDC& dc);
	void drawMidline(wxDC& dc);
	myWaveFile* getFile();

private:

	myWaveFile* wavFile;

	wxSize maxSize;
	int midHeight;
	int	maxHeight;
	int midWidth;
	int maxWidth;
	bool loaded = false;

	DECLARE_EVENT_TABLE();
};

