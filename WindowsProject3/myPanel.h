#pragma once
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/wfstream.h"

class myPanel : public wxPanel
{
public:
	myPanel(wxFrame* parent, wxFFile* wavFile);
	~myPanel();

	void paintEvent(wxPaintEvent& event);
	void render(wxDC& dc);
	bool isLoaded();
	void nowLoaded();
	void drawTest(wxDC& dc);
	wxFFile* getFile();

private:
	bool readHeader();
	void readChunk1();

	wxFFile* wavFile;
	uint32_t* filesize;
	wxSize maxSize;
	int midHeight;
	int	maxHeight;
	int midWidth;
	int maxWidth;
	bool loaded = false;

	DECLARE_EVENT_TABLE();
};

