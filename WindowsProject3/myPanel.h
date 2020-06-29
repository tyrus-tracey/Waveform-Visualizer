#pragma once
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/wfstream.h"
#include "myWaveFile.h"
#include "wx/panel.h"

class myPanel : public wxPanel
{
public:
	myPanel(wxFrame* parent, const wxString filepath);
	~myPanel();

	void paintEvent(wxPaintEvent& event);
	void render(wxDC& dc);
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

