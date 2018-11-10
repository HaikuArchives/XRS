#ifndef _PATCONTROL_H_
#define _PATCONTROL_H_

#include <InterfaceKit.h>

class PatControl : public BView
{
	public:
		PatControl(BRect,BPicture*,BPicture*,BPicture*,BPicture*,BPicture*,int ,int ,int,int32,BBitmap *base);
	
	virtual void 		AttachedToWindow();	
	virtual void 		MessageReceived(BMessage*);
		int 		getValue();
		void 		setValue(int);
		void		setMax(int);
		void		setEnabled(bool);
		bool		isEnabled();
	virtual void		Draw(BRect);		
	private:
	
		BPictureButton 	*up;
		BPictureButton	*down;
		BBitmap		*pad;
		//BStringView		*text;
		//int	value;
		int 			min,max,value;
		char 			str[3];
		//int32 			to_msg;
		bool 			is_en;
		BMessage		*the_msg;
		
};

#endif
