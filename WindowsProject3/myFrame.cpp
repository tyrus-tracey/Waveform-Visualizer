#include "myFrame.h"
#include "myWaveFile.h"
#include <intrin.h>

myFrame::myFrame(wxSize& appDimensions) 
	: wxFrame(NULL, wxID_ANY, "Waveform Visualizer", wxPoint(0,0), appDimensions), panel(NULL)
{
	createMenuBar();
		
	frameSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(frameSizer);
	SetAutoLayout(true);
	Show(true);
	
}

myFrame::~myFrame() {
}

void myFrame::createMenuBar() {
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_Open, "&Open a Wave File");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("This is the Status Bar.");

	Bind(wxEVT_MENU, &myFrame::OnOpen, this, ID_Open);
	Bind(wxEVT_MENU, &myFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &myFrame::OnExit, this, wxID_EXIT);
}

myPanel* myFrame::getPanel()
{
	return panel;
}

void myFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("This is a wxWidgets Hello World example",
		"About Hello World", wxOK | wxICON_INFORMATION);
}

void myFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

void myFrame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog openDialog(this, ("Open a Wave file"), "", "", "Wave files (*.wav)|*.wav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (openDialog.ShowModal() == wxID_CANCEL) {
		wxMessageBox("No file was chosen.");
		return;
	}
	
	myWaveFile* inputFile = new myWaveFile(openDialog.GetPath());
	
	if (!inputFile->IsOpened()) {
		wxMessageBox("File could not open. Please try again.");
	}
	
	panel = new myPanel(this, inputFile);
	GetSizer()->Add(panel, 1, wxEXPAND);

	Refresh();
	if (panel->isLoaded()) {
		wxMessageBox(panel->getFile()->GetName(), "File Loaded");
	}
	
}
