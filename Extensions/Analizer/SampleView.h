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
	void Init(float** buffer,int32 samples_num,int spiaz);
	//virtual void Pulse();
			
	private:
	
		int32	sampleCount;
		
		float *data[2];
		int	spiaz;
		bool clip;
		
		
};

