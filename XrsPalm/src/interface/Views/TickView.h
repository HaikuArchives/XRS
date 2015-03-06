// TickView 
//	23-7-00
//		verison 0.1

#include <View.h>

class TickView : public BView
{
	public:
		TickView(BRect,int d);
	
	void 	SetTick(int,int,int);
	void	SetNumberNotes(int);
	virtual void Draw(BRect);
	virtual void AttachedToWindow();
	private:
			float	xinc;
			int tick;
			float space,space2;
			int num_notes;
			
};