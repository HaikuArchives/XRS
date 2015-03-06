#include "VstHost.h"

#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "AEffect.h"
#include "AudioEffect.hpp"

VstHost::VstHost()
{
	
	status_t err;
	char	name[100];
	int32 l;
	int32 type;
	void	*u;
	ae=NULL;	
	
	AEffect* 	(*main_plugin)(audioMasterCallback); 
	
	id=load_add_on("/boot/Freeverb3");
	
	printf("Image id :%d \n",(int32)id);
	
	
	err=get_image_symbol(id,"main_plugin",B_SYMBOL_TYPE_TEXT,(void**)&main_plugin);
	
	if(err!=B_OK) {
	
	printf("Error! %s\n",strerror(err));
	unload_add_on(id);
	}
	
	else
	{	
	
	ae=(*main_plugin)(d);
	
	printf("PlugIn Caricato !!  \n");
	
	if(ae==NULL) printf("Main: effect==NULL!\n");
		else
			{
			 printf("Main: effect numParams = %ld !\n",ae->numParams);
			printf("Main: effect version = %ld !\n",ae->version);
			
			if(ae->magic==kEffectMagic) printf("PlugIn Certificato!\n");
			
			char f[100];
			void *		chunk;
			ssize_t		size;
			long rt;
			
			
			printf("Effect: parametro 1 valwe :%f\n",ae->getParameter(ae,1));
			
			//ae->setParameter(ae,1,0.2);
			
			AudioEffect *fx=(AudioEffect*)ae->object;	// for class access (see AudioEffect
			fx->getParameterLabel(1,f);
			printf("One label : %s\n",f);
			
			printf("getBlockSize : %ld\n",fx->getBlockSize());
			printf("Sample rate : %f\n",fx->getSampleRate());
			
			rt=ae->dispatcher(ae,effOpen        ,  0, 0, NULL, 0.f);
			printf("effOpen %ld\n",rt);
			rt=ae->dispatcher(ae,effSetProgram  ,  0, 0, NULL, 0.f);
			printf("effSetProgram %ld\n",rt);
			rt=ae->dispatcher(ae,effMainsChanged,  0, 1, NULL, 0.f);
			printf("effMainsChanged %ld\n",rt);
			rt=ae->dispatcher(ae,effSetSampleRate, 0, 0, 0, 44100);
			printf("effSetSampleRate %ld\n",rt);
			rt=ae->dispatcher(ae,effSetBlockSize,  0, 4, NULL, 0.f);
			printf("effSetBlockSize %ld\n",rt);
			size = ae->dispatcher (ae, effGetChunk, 0, 0, &chunk, 0.f);
			
			printf("chunkSize : %ld\n",size);
						
			inputsl[0]=5;
			inputsl[1]=6;
								
			inputsr[0]=7;
			inputsr[1]=8;
			
						
			ptr[0]=(float*)&inputsl;
			ptr[1]=(float*)&inputsr;
			
			// OutPuts 
			
			
			outputsl[0]=outputsl[1]=outputsr[0]=outputsr[1]=0;
						
						
			optr[0]=(float*)&outputsl;
			optr[1]=(float*)&outputsr;
						
			//float outputs[2][2];
			long sampleframes=2;
			
			ae->processReplacing(ae, (float**)ptr, (float**)optr, sampleframes);
						
    	      		 printf("OutPut at 0  : %f  %f !\n",outputsl[1],outputsr[1]);
		}
	//unload_add_on(id);
	}
}

void
VstHost::process(float x,float y)
{
	inputsl[0]=x;
	inputsr[0]=y;
	ae->processReplacing(ae, (float**)ptr, (float**)optr, 1);
}
void
VstHost::direct(float** in,float** out ,int32 num)
{
	ae->processReplacing(ae, (float**)in, (float**)out, num);
}
float
VstHost::lastLeft()
{
	return outputsl[0];
}
float
VstHost::lastRight()
{
	return outputsr[0];
}
long
VstHost::d(AEffect *effect, long opcode, long index,long value, void *ptr, float opt)
{
	printf("**OpCode %ld \n",opcode);
	if(effect==NULL) printf("d: effect==NULL!\n");
	
	return 1;
}

VstHost::~VstHost()
{
	unload_add_on(id);
}