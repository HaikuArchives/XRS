#include "View.h"
#include "async.h"

class StereoMeter:	public BView{
	typedef	BView  _inherited;
	
	public:
				StereoMeter(	BRect	frame);
		virtual void 	Draw(BRect r);
		virtual void 	MessageReceived(BMessage *message);
		
	private:
		float value_l;
		float value_r;
		float current;
};