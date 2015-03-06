#include "SamplerTrack.h"
#include "Pattern.h"
#include "Note.h"
#include "Sample.h"

#include "aflibConverter.h"

#include "math.h"


#define THUMB	50;


   
SamplerTrack::SamplerTrack ():Track()
{
	reversed=false;
	curSample=NULL;
	virtualposition=0;
	samplesperbeat=0;
	converter=new aflibConverter(false,true,false);
	myBufferino=malloc( FRAMES_NUM*4+200);
	numNotes=4;
	res_enable=false;
	need_res=false;
	amp=1.;
}

const char*
SamplerTrack::getPreferredName()
{
	if(curSample!=NULL) return curSample->name;
	else return NULL;
}
SamplerTrack::~SamplerTrack(){}

int
SamplerTrack::getModel(){return 0;}


bool
SamplerTrack::isReversed()
{
	return reversed;
}
void
SamplerTrack::setReversed(bool val)
{
	reversed=val;
}

Sample*
SamplerTrack::getSample()
{
	return curSample;
}
void
SamplerTrack::setSample(Sample* s)
{
	
	curSample=s;
	posSample=0;
	if(s==NULL) return;
	
	type=s->type;
	virtualbytes=s->totalbytes;
	
	if(res_enable){
	need_res=true;
	note_status=0;
	virtualposition=0;}
}

void
SamplerTrack::setResample(int val)
{
	numNotes=(float)val/4.;	
	need_res=true;
	if(res_enable){
	note_status=0;
	virtualposition=0;}
}

void
SamplerTrack::setResampleEnable(bool val)
{
	res_enable=val;
	need_res=true;
	virtualposition=0;
	note_status=0;
}

bool
SamplerTrack::isResampleEnable() { return res_enable; }

int
SamplerTrack::getResample(){ return (int)(numNotes*4);}
void
SamplerTrack::getSampleData(void* dest,size_t pos,size_t size)
{
	memcpy(dest,(char*)myBufferino+pos,size);
}

size_t
SamplerTrack::mix2(float** dest, size_t spiaz , size_t pos , size_t framecount,float note_vol)
{
	// Mix2
		
	int16 data[2];
	
	
		int virtualPos=0;
		if(!reversed)	virtualPos=posSample;
		else
			{
				virtualPos=curSample->totalbytes/4-posSample-framecount;
				if(virtualPos<0) virtualPos=0;
			}
	if(res_enable)
	{
		int d=(int)ceil((float)(framecount)*(1./coef));
		int count=(int)ceil((float)(1024)*(1./coef))+3;
		//int ret=
		converter->resample(count,1024,(HWORD[])((char*)curSample->data+(virtualPos*4)),(HWORD[])myBufferino);
		posSample +=d ; //count;
	}
	else
	{
		memcpy(myBufferino,(char*)curSample->data+(virtualPos*4),framecount*4);
		posSample +=framecount ; //count;
	}
	
	//printf("B) req %5ld ; ret %5d ; count %5d  ; d %d\n",framecount,ret,count,d);
		
	for(size_t x=0 ; x<framecount ; x++)
	{
		if(!reversed){
		
			getSampleData((char*)&data[0],x*4,2);
			getSampleData((char*)&data[1],x*4+2,2);
		}
		else
		{
			getSampleData((char*)&data[0],(framecount-1-x)*4,2);
			getSampleData((char*)&data[1],(framecount-1-x)*4+2,2);
		}
				
		
		dest[0][spiaz+x] +=(float)data[0]/32767.*vol_left*note_vol*amp;
		dest[1][spiaz+x] +=(float)data[1]/32767.*vol_right*note_vol*amp;
						
	}
	
	//dest[0][framecount-1]=1;
	//dest[1][framecount-1]=1;
	
	return pos;
}

void
SamplerTrack::Reset()
{
	
	note_status=-1;
	virtualposition=0;
	posSample=0;
	curNote=NULL;
}

void
SamplerTrack::NoteReady(int notenumber,int patternnumber,float samplesperbeat)
{
	if(patternnumber!=EMPTY_PATTERN)
	{
	
	Note *n=getPatternAt(patternnumber)->getNoteAt(notenumber);
	//printf("New Note! : \n" );
	if(n->getValue())
	{
			curNote=n;
			note_status=1;
			virtualposition=0;
			posSample=0;
			
			if(curSample!=NULL && (need_res || samplesperbeat!=this->samplesperbeat)  )
			{
				this->samplesperbeat=samplesperbeat;
				if(res_enable)
				{
					coef=(float)( (float)(samplesperbeat*numNotes) /  (float)((float)curSample->totalbytes/4.));
					//printf("Coef : %f \n",coef );
					converter->initialize(coef,2);
					virtualbytes=(uint32)floor((float)((float)curSample->totalbytes/8.)*coef)*8+1;
				}
				else
				{
					virtualbytes=curSample->totalbytes;
				}
				need_res=false;
			}
	}
	
	else
		{
			note_status=0;		
		}
	}
	else
	{
		note_status=0;		
	}
	
	
}
void
SamplerTrack::Process(float ** buffer ,int32 sample_num,int spiaz)
{
	if(type!=UNSET_TYPE && curNote!=NULL)
	{
		int poz=SamplerTrack::min(sample_num,(virtualbytes/4-virtualposition));
		
		if(note_status==1)	// c'e' una nuova nota on oppure devo continuare con l'ultima valida?
		{
			// Nuova nota attiva!
			
			if(poz>0)
			{
				mix2(buffer,spiaz ,virtualposition*4 , poz,curNote->getVolume());
				virtualposition +=poz;
			}
		
		}
		else
		if(note_status==0)
		{
			if(poz>0 && virtualposition>0)
			{
			
				mix2(buffer,spiaz ,virtualposition*4 , poz,curNote->getVolume());
				virtualposition +=poz;
			}
		}
	}
}
void
SamplerTrack::goOn(){

	Reset();

}