#include "Sequence.h"
#include "stdio.h"
#include "String.h"

#include "List.h"
//#define	MAX_SEQUENCE 99

Sequence::Sequence()
{
	MaxSeq=-1;
	MaxPat=0;
	names=new BList(0);
	matrix2=NULL;
	
}
void
Sequence::AddMeasure(const char* name){

	names->AddItem((void*)(new BString(name) ));
	MaxPat++;

}
BString*
Sequence::GetMeasureName(int pos){

	if(pos<0 || pos >= names->CountItems()) return NULL;
	return (BString*)(names->ItemAt(pos));
}
void
Sequence::SetMeasureName(const char* s,int pos)
{
	if(pos<0 || pos >= names->CountItems()) return;
	 ((BString*)(names->ItemAt(pos)))->SetTo(s);
}
int
Sequence::ItemAt(int d)
{
	//printf("M Item At : %d\n",d);
	
	//printf("m2 where : %lld %ld \n",(bigtime_t)d*10000,matrix2->EventCount()); 
	const media_timed_event *mev=matrix2->FindFirstMatch((bigtime_t)d*10000,BTimedEventQueue::B_AFTER_TIME,true);
	
	
	if(mev==NULL) return -1;
	
	if(mev->event_time >= d*10000+10000) return -1;
	//printf("M2 Item At : %d %lld\n",(int)(mev->event_time - d*10000),mev->event_time);
	return (int)(mev->event_time - d*10000);
	
	
//	if(d<MAX_SEQUENCE)
//	return matrix[d];
//		else
//	return -1;
	
}


void
Sequence::setItemAt(int d,int val,bool c=false)
{
	//if(d<MAX_SEQUENCE)
//		matrix[d]=val;
	
	//printf("M SetItemAt : %d %d \n",d,val);
	
	
		
	const media_timed_event *mev=matrix2->FindFirstMatch(d*10000,BTimedEventQueue::B_AFTER_TIME,true);
	if(mev!=NULL && (mev->event_time < d*10000+10000)) 
	{
		matrix2->RemoveEvent(mev);
		if(val==-1) return;
	}
	media_timed_event nmev(d*10000+val,BTimedEventQueue::B_USER_EVENT);
	matrix2->AddEvent(nmev);
	
	//printf("M2 SetItemAt : %lld\n",nmev.event_time);
	
		
	if(c && val>=0 && d>MaxSeq) {
	
	//if(matrix2->EventCount()==0) return -1;
	//printf("GetMaxSeq %lld\n",
	MaxSeq= (int)(matrix2->LastEventTime()/10000);
	
	}
}

int
Sequence::Reset()
{
	
		if(matrix2!=NULL) delete matrix2;
		matrix2=new BTimedEventQueue();
		//name->MakeEmpty();
		for(int i=0;i<names->CountItems();i++)
		{
			BString* s=(BString*)names->ItemAt(i);
			if(s!=NULL) delete s;
			names->RemoveItem(i);
		}
		MaxSeq=0;
		return 0;
	/*
	for(int x=0;x<MAX_SEQUENCE;x++)
				if(x<MaxPat) matrix[x]=x;
				else
					matrix[x]=-1;
					
	MaxSeq=MaxPat-1;
	
	
	return 0;*/
					
}
int
Sequence::getMaxPat() { return MaxPat; }

void
Sequence::RemoveMeasure(int pos)
{
	int ms=-1;
	int mat;
	
	for(int x=0;x<MaxSeq+1;x++)
	{
		mat=ItemAt(x);
		
		if(x>ms && mat>=0 && mat!=pos) ms=x;
		
		if(mat!=-1 && mat>pos){
				setItemAt(x,mat-1,true);
				
				}
		else
		if( mat==pos)
			setItemAt(x,-1,true);
	}
	
	BString* s=(BString*)names->ItemAt(pos);
	if(s!=NULL) delete s;
	names->RemoveItem(pos);

	MaxSeq=ms;
}

int
Sequence::getMaxSeq() {return MaxSeq; }
void
Sequence::setMaxPat(int d) { MaxPat=d; }
void
Sequence::setMaxSeq(int d) { MaxSeq=d; }
