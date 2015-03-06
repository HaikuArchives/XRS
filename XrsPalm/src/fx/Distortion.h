class Distortion
{
public:
	int _posThreshold;
	int _posClamp;
	int _negThreshold;
	int _negClamp;

	Distortion();
	void Work(float**,int);
	void	Init();
};