#ifndef _VSTHOST_H_
#define _VSTHOST_H_

#include "KernelKit.h"

class AEffect;

class VstHost
{
	public:
	
			VstHost();
		         ~VstHost();
				static long d(AEffect *effect, long opcode, long index,long value, void *ptr, float opt);
		
		void 	process(float,float);
		void    direct(float**,float** ,int32);
	
		float lastLeft();
		float lastRight();
		
	private:
	

		
		
		float inputsl[2];
		float inputsr[2];
		float *ptr[2];
		float outputsl[2];
		float outputsr[2];
		float *optr[2];
		image_id id;
		AEffect*	ae;
		bool	value;
		float   volume;
};

#endif