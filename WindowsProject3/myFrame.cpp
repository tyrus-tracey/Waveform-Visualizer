#include "myFrame.h"
#include "myWaveFile.h"
#include <intrin.h>

// Using base class constructor, create a frame of a specified size in the app.
myFrame::myFrame(wxSize& appDimensions) 
	: wxFrame(NULL, wxID_ANY, "Waveform Visualizer", wxPoint(0,0), appDimensions), panel(NULL)
{
	createMenuBar();
		
	frameSizer = new wxBoxSizer(wxHORIZONTAL); //Sizers handle spacing of elements within frame
	SetSizer(frameSizer);
	SetAutoLayout(true);
	Show(true); // Display this frame
}

myFrame::~myFrame() {
}

// Create the menu bar and bind some events to menu options
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
	if (panel != NULL) {
		panel->Close(true); // Stop displaying
	}
	Close(true); // Stop displaying
}

// If user selects a wave file, a new panel will be created using that file.
// Then, that panel will be attached to the frame using the frame's sizer.
void myFrame::OnOpen(wxCommandEvent& event)
{
	delete panel; // Delete panel if one exists
	wxFileDialog openDialog(this, ("Open a Wave file"), "", "", "Wave files (*.wav)|*.wav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (openDialog.ShowModal() == wxID_CANCEL) { // If user cancels the dialog
		wxMessageBox("No file was chosen.");
		return;
	}
	
	panel = new myPanel(this, openDialog.GetPath()); // Create a new panel with given wave file
	GetSizer()->Add(panel, 1, wxEXPAND); // Add the panel to the frame's sizer

	Refresh(); //Redraw the frame
}
