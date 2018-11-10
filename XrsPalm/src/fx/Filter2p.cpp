#include "Filter2p.h"
#include <math.h>
Filter2p::Filter2p()
{
	Init();
	
}
void 
Filter2p::setPar(float a,float b)
{
	/*_lfoSpeed = a;
	_lfoAmp = b;
	Update();*/
}
void
Filter2p::changeMode()
{
	_cutoff=256-_cutoff;
	_fCutoff = _cutoff * 0.00390625f;

	if(_filterMode==0)  _filterMode=1;
	else _filterMode=0;
}
void
Filter2p::setCutoff(float val)
{
	if(_filterMode==0) val=256-val;
	_cutoff=val;
	_fCutoff = val * 0.00390625f;
}
void
Filter2p::setResonance(float val)
{
	_resonance=val;
	_fResonance = _resonance * 0.00390625f;
}
void Filter2p::Init(void)
{
	//Machine::Init();

	_cutoff = 400 ;  //128--
	_resonance = 0;	// MAX 256
	
	_lfoSpeed = 0;
	_lfoAmp = 0;
	_lfoPhase = 0;
	_filterMode = 0; //0 ->LP 1->HP
	_fLfoDegree = 0;
	
	_bufferL0 = 0;
	_bufferL1 = 0;
	_bufferR0 = 0;
	_bufferR1 = 0;

	Update();
}

void Filter2p::Work(float ** in ,  int numSamples)
{
	//Machine::Work(numSamples);
	//CPUCOST_INIT(cost);

	if (true) //Mute?
	{
		static float const TWOPI = 6.28318530717958647692528676655901f;
		
		float co1 = _fCutoff + (float)sin(_fLfoDegree) * _fLfoAmp;
		float co2 = _fCutoff + (float)sin(_fLfoDegree + _fLfoPhase) * _fLfoAmp;
		
		// Init filter intermediate vars
		if (co1 < 0.01f)
		{
			co1 = 0.01f;
		}
		if (co1 > 0.99f)
		{
			co1 = 0.99f;
		}
		float fa1 = float(1.0 - co1); 
		float fb1 = float(_fResonance * (1.0 + (1.0/fa1)));
		
		if (co2 < 0.01f)
		{
			co2 = 0.01f;
		}
		if (co2 > 0.99f)
		{
			co2 = 0.99f;
		}
		float fa2 = float(1.0 - co2); 
		float fb2 = float(_fResonance * (1.0 + (1.0/fa2)));
		
		float* pSamplesL =in[0];//= 
		float* pSamplesR =in[1];// = 
		//float*_pSamplesL =out[0];
		//float*_pSamplesR = out[1];
		
		int i = numSamples;
		do
		{
			*pSamplesL++ = WorkL(*pSamplesL, co1, _fResonance, fa1, fb1);
			*pSamplesR++ = WorkR(*pSamplesR, co2, _fResonance, fa2, fb2);
			
			_fLfoDegree += _fLfoSpeed;
			if (_fLfoDegree > TWOPI)
			{
				_fLfoDegree -= TWOPI;
			}
		}
		while (--i);
	}
	//Machine::SetVolumeCounter(numSamples);
	
	//_cpuCost += cost;
	//_worked = true;
}

float Filter2p::WorkL(
	float input,
	float f,
	float q,
	float fa,
	float fb)
{
	input += 1.0f;
	
	_bufferL0 = fa * _bufferL0 + f * (input + fb * (_bufferL0 - _bufferL1)); 
	_bufferL1 = fa * _bufferL1 + f * _bufferL0;
	
	switch(_filterMode)
	{
	case 0:
		return _bufferL1;
		break;
		
	case 1:
		return input-_bufferL1; 
		break;
	}
	return 0;
}

float Filter2p::WorkR(
	float input,
	float f,
	float q,
	float fa,
	float fb)
{
	input += 1.0f;
	
	_bufferR0 = fa * _bufferR0 + f * (input + fb * (_bufferR0 - _bufferR1));
	_bufferR1 = fa * _bufferR1 + f * _bufferR0;

	switch (_filterMode)
	{
	case 0:
		return _bufferR1; 
	case 1:
		return input-_bufferR1; 
	}
	return 0;
}

void Filter2p::Update(void)
{
	_fCutoff = _cutoff * 0.00390625f;
	_fResonance = _resonance * 0.00390625f;
	_fLfoSpeed = _lfoSpeed * 0.00000003f;
	_fLfoAmp = _lfoAmp * 0.00390625f;
	_fLfoPhase = _lfoPhase * 0.0122718f;
}


