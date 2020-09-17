#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>


int main() 
{
	Fl_Window win(400, 200, "Testing");

	Fl_Button but(10, 150, 70, 30, "Click me");


	win.show();

	return Fl::run();

}