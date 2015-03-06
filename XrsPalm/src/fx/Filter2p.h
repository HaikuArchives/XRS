
//Ciao!

class Filter2p 
{
public:
	int _cutoff;
	int _resonance;
	int _lfoSpeed;
	int _lfoAmp;
	int _lfoPhase;
	int _filterMode;

	Filter2p();
	
	void setPar(float,float);
	void setCutoff(float);
	void setResonance(float);
	void	changeMode();
	
	virtual void Init(void);
	virtual void Work(float **, int numSamples);
	void Update(void);

protected:
	static char* _psName;
	float _bufferL0;
	float _bufferL1;
	float _bufferR0;
	float _bufferR1;
	float _fCutoff;
	float _fResonance;
	float _fLfoSpeed;
	float _fLfoAmp;
	float _fLfoPhase;
	float _fLfoDegree;

	float WorkL(float input, float f, float q, float fa, float fb);
	float WorkR(float input, float f, float q, float fa, float fb);
};