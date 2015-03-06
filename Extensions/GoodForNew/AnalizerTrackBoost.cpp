#include "AnalizerTrackBoost.h"
#include "JAnalizerTrack.h"
#include "AnalizerTrack.h"
#include "defined.h"
/*#include "Application.h"
#include "JTrack.h"


#include "filemaker_msg.h"
#include "Message.h"
#include "File.h"
#include "GfxMsg.h"*/




AnalizerTrackBoost::AnalizerTrackBoost():TrackBoost()
{
	name[0]='A';
	name[1]='n';
	name[2]='a';
	name[3]='l';
	name[4]='i';
	name[5]='z';
	name[6]='e';
	name[7]='r';
	name[8]='\0';
	id=MY_MODEL_ID;
}

Track*
AnalizerTrackBoost::getTrack()
{
	return (Track*)(new AnalizerTrack());
}
JTrack*
AnalizerTrackBoost::getJTrack(BRect r,int16 n,BView* tv)
{
	JTrack *jt=new JAnalizerTrack(r,n,tv);

 	return jt;
 }

bool
AnalizerTrackBoost::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	return false;
}

bool
AnalizerTrackBoost::NeedMessage(){return true;}


