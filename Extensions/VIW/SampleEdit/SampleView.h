/***************************************
/
/	SimpleJuice beta 02
/
/   Something like a DrumMachine.
/	Original idea by Andrea Anzani
/	
/	I hope to find someone that can help me in get
/	this app better!
/	Please write two lines to me.
/
/	andrea.anzani@lib.disco.unimib.it
/
*****************************************/

#include <View.h>
#include <Synth.h>

class SampleView : public BView
{
	public:
		SampleView(BRect);
	
	virtual void Draw(BRect);
	void Init(void *data,int32,bool);
			
			
	private:
		int16	left;
		int16	right;
		int32	sampleCount;
		void	*data;
		bool clip;
		
};

