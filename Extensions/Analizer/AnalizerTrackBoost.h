#ifndef _ANALIZER_TRACK_BOOST_H_
#define _ANALIZER_TRACK_BOOST_H_

#include "PopUpMenu.h"

#include "TrackBoost.h"




class AnalizerTrackBoost : public TrackBoost
{
	public:
			AnalizerTrackBoost();
	virtual	~AnalizerTrackBoost() {};
	virtual 	Track*		getTrack();
	virtual 	JTrack*	getJTrack(BRect r,int16 n,BView* tv);
	virtual	bool		Load(Track* cur,int t,BFile* file,int32 va,int32 rt) ;
	virtual	bool		NeedMessage();

		
			
};
#endif