#pragma once
#include "wx/wx.h"
#include "myFrame.h"

class myApp : public wxApp
{
public:
	myApp();
	~myApp();
	virtual bool OnInit();
	wxSize getDimensions();

private:
	myFrame* frame = NULL;
	wxSize dimensions = wxSize(800, 600);
	
};

