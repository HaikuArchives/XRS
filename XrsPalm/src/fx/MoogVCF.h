
//Ciao!

class MoogVCF 
{
public:
	
	MoogVCF();

	
		float Work(float );
		void	setPar(float,float);
	
	float y1,y2,y3,y4,oldx,oldy1,oldy2,oldy3;
	float cutoff ;
	float fs ; //(e.g. 44100Hz)
	float res; //(minimum - maximum)
	float f ; //[0 - 1]
	float k; //(Empirical tunning)
	float p;
	float scale;
	float r ;
	
};