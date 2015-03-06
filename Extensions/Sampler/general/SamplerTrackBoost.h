#ifndef _SAMPLER_TRACK_BOOST_H_
#define _SAMPLER_TRACK_BOOST_H_

#include "PopUpMenu.h"

#include "TrackBoost.h"

class Sample;
class ExternalManager;
class BankManager;
class PopUpMenu;
class JSamplerTrack;
class SamplerTrack;
class FxWindow;

class SamplerTrackBoost : public TrackBoost
{
	public:
					SamplerTrackBoost();
	virtual			~SamplerTrackBoost();
	virtual 	Track*		getTrack();
	virtual 	JTrack*	getJTrack(BRect r,int16 n,BView* tv);
	virtual	bool		Load(Track* cur,int t,BFile* file,int32 va,int32 rt) ;
	virtual	void		Save(Track *cur,int t,BFile* file,int mea) ;
	virtual	status_t	RefReceived(entry_ref,Track*,BMessage *m=NULL);
	virtual	bool		NeedMessage();
	virtual	void		Init(TrackManager*);
	virtual	void		Reset();	//on a new song!
	virtual	void		Restart();
			void		RemoveAll();
			void		RemoveSample(Sample*);
		
	ExternalManager*	getEXTM();
	void			ShowMenu(BPoint,JSamplerTrack*);
	//void			setSample(Sample*);
	void			SetTrack(SamplerTrack* tr);
	void			MakeMenu();
	void			ChangeSample(int id);
				void	LoadSample();
				void	ShowPanel();
	private:

			BPopUpMenu*	popup;
			ExternalManager*	extm;
			BankManager*		banker;
			JSamplerTrack*	lastTrack;
			BFilePanel*		openpanel;
			entry_ref		sample_ref;
					
			status_t		locateEXTMSample(SamplerTrack*);
			status_t		CheckIsBank(entry_ref *ref);
			Sample*		FindSample(const char*);
			FxWindow*		fw;
};
#endif