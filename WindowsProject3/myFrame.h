#pragma once
#include "wx/wx.h"
#include "wx/file.h"
#include "myPanel.h"

/*
	Frames are created to house GUI elements and controls, such as menus and panels.
	Handles most events between the program and user.
*/

class myFrame : public wxFrame
{
public:
	myFrame(wxSize& appDimensions);
	~myFrame();
	myPanel* getPanel();

private:
	void OnAbout(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void createMenuBar();

	myPanel* panel = NULL;
	wxBoxSizer* frameSizer = NULL;
	enum {ID_Open = 1};
};

