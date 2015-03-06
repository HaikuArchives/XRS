#ifndef _SAMPLER_TRACK_BOOST_H_
#define _SAMPLER_TRACK_BOOST_H_

#include "PopUpMenu.h"

#include "TrackBoost.h"

class BMidiLocalProducer;


class MidiOutTrackBoost : public TrackBoost
{
	public:
			MidiOutTrackBoost();
	virtual	~MidiOutTrackBoost() {};
	virtual 	Track*		getTrack();
	virtual 	JTrack*	getJTrack(BRect r,int16 n,BView* tv);
	virtual	bool		Load(Track* cur,int t,BFile* file,int32 va,int32 rt) ;
	//virtual	void		Save(Track *cur,int t,BFile* file,int mea) ;
	virtual	void		MessageReceived(BMessage* message);
	virtual	bool		NeedMessage();

	private:
			void GetIcons(BBitmap* largeIcon, BBitmap* miniIcon);
			void AddIcons(BMessage* msg, BBitmap* largeIcon, BBitmap* miniIcon) const;
	
			BPopUpMenu*	popup;
			void ShowMenu();
			
			BMidiLocalProducer *mider;
};
#endif