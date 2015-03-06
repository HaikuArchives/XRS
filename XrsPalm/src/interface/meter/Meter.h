#include "View.h"
#include "async.h"

class Meter:	public BView{
	typedef	BView  _inherited;
	
	public:
				Meter(	BRect	frame);
		virtual void 	Draw(BRect r);
		virtual void 	MessageReceived(BMessage *message);
		
	private:
		float value;
		float current;
};