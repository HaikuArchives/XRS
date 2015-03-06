//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#include "Distortion.h"
#include "stdio.h"

Distortion::Distortion()
{Init();}

void Distortion::Init(void)
{
	_posThreshold = 2528;
	_posClamp = 0;
	_negThreshold = 3232;
	_negClamp = 0;
}

void Distortion::Work(float **inp ,int numSamples)
{
	//Machine::Work(numSamples);
	//CPUCOST_INIT(cost);
	if (true)
	{
		float dpt = _posThreshold*128.0f;
		float dpc = _posClamp*128.0f;
		float dnt = -_negThreshold*128.0f;
		float dnc = -_negClamp*128.0f;
		float in;
		
		float* pSamplesL = inp[0];
		float* pSamplesR =inp[1];
		
		--pSamplesL;
		--pSamplesR;
		
		printf("prova : %f\n",inp[0][0]);
		
		int i = numSamples;
		do
		{
			in = *++pSamplesL;
			if (in > dpt)
			{
				in = dpc;
			}
			if (in < dnt)
			{
				in = dnc;
			}
			*pSamplesL = in;
			
			
			
			in = *++pSamplesR;
			if (in > dpt)
			{
				in = dpc;
			}
			if (in < dnt)
			{
				in = dnc;
			}
			*pSamplesR = in;
		}
		while (--i);
	}
	
	//Machine::SetVolumeCounter(numSamples);
	
}



