#include "myApp.h"

wxIMPLEMENT_APP(myApp);

myApp::myApp() {

}

myApp::~myApp() {
}

wxSize myApp::getDimensions() {
	return dimensions;
}

bool myApp::OnInit() {
	frame = new myFrame(dimensions);

	return true;
}
