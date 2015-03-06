#include "Window.h"

class BStringView;

class AboutBox: public BWindow
{
	public:
	 	AboutBox(bool);
	 	void setText(const char*);
	 private:
	 	BStringView *text;
		 	
};