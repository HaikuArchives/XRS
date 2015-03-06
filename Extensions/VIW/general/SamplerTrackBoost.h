#ifndef _SAMPLER_TRACK_BOOST_H_
#define _SAMPLER_TRACK_BOOST_H_


#include "TrackBoost.h"


class SamplerTrackBoost : public TrackBoost
{
	public:
			SamplerTrackBoost();
			virtual	~SamplerTrackBoost(){}
	virtual 	Track*		getTrack();
	virtual 	JTrack*	getJTrack(BRect r,int16 n,BView* tv);
	virtual	bool		Load(Track* cur,int t,BFile* file,int32 va,int32 rt) ;
	virtual	void		Save(Track *cur,int t,BFile* file,int mea) ;
	
};
#endif