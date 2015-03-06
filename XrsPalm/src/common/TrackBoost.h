#ifndef _TRACK_BOOST_H_
#define _TRACK_BOOST_H_


#include "View.h"
#include "Entry.h"


class Track;
class JTrack;
class BFile;
class TrackManager;

class TrackBoost 
{
	public:
	
		TrackBoost();
	virtual	~TrackBoost() {};
	
	char	name[100];	//Better a BString!
	int16	id;		// Ask Me to assign
	
	virtual	Track*		getTrack() { return NULL;}
	virtual	JTrack*	getJTrack(BRect r,int16 n,BView* tv) { return NULL;}
	virtual	bool		Load(Track* cur,int t,BFile* file,int32 va,int32 rt) {return true;}
	virtual	void		Save(Track *cur,int t,BFile* file,int mea) {if(cur==NULL) return; return ;}
	virtual	status_t	RefReceived(entry_ref,Track*,BMessage *m=NULL){};
			void		WriteFixed(int64 code ,int64 data,BFile* file );
			void		WriteVar(int64 code ,void* data,int64 size,BFile* file);
			int64		ReadFixed(int64*,BFile* file);
			
	virtual	void		Init(TrackManager* t);  //only one time, when the boost is created 
	virtual	void		Reset(){};		//each time a new song is ready
	virtual	void		Restart() {};		//EACH TIME A song is going to be close.
			void		RefreshGraphics();
			void		RefreshSelected();
			void		Refresh(JTrack* t);
	
	
			TrackManager*		tm;
			
};
#endif