#include <View.h>

class VStringView:public BView
{
	public:
			VStringView(BRect,char*);
			char	*text;
	virtual void Draw(BRect);
	void SetStringPos(BPoint x);
	
	private:
		BPoint	spec;
};