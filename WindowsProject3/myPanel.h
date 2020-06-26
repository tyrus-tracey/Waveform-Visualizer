#pragma once
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/wfstream.h"
#include "myWaveFile.h"

class myPanel : public wxPanel
{
public:
	myPanel(wxFrame* parent, myWaveFile* audioFile);
	~myPanel();

	void paintEvent(wxPaintEvent& event);
	void render(wxDC& dc);
	bool isLoaded();
	void nowLoaded();
	void drawTest(wxDC& dc);
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

