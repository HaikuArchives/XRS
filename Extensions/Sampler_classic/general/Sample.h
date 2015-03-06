#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "GlobalDef.h"
#include "SupportKit.h"

class Sample 
{
	public:
			Sample();
			~Sample();
		
		void*	data;
		char   name[SAMPLE_NAME_L];
			
		uint32		totalbytes;
		int		modifier;
		int		type;	// 0 is BANK Samples
					// 1 is External Sample //
		char*		path_name;
};

#endif