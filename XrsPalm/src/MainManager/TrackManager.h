#ifndef _TRACKMANAGER_
#define _TRACKMANAGER_

//#include "List.h"
#include "View.h"

class BMenu;
class Track;
class JTrack;
class BFile;
class TrackBoost;
class MBWindow;
class Song;

#define	MAX_PLUG	16

class	TracksPanel;

class TrackManager
{
public:
	
	TrackManager();
	~TrackManager();
	
	Track*		getTrack(int);
	JTrack*	MakeJTrack(Track*,BRect rect,int16 n,BView* tv);		
	JTrack*	getJTrack();
	void		Save(Track*,int,BFile*,int);
	bool		Load(Track*,int,BFile*,int32,int32);
	Track*		SendRef(entry_ref,int,BMessage *m=NULL);	
	bool		isBoosterValid(int);
	status_t	SelectTrack(JTrack* t);
	JTrack*	getCurrentJTrack(){ return curJTrack; };
	void		getAllMyTrack(BList *l,int id);
	void		getAllMyJTrack(BList *l,int id);
	
	Track*		getCurrentTrack();
	
	void		Reset(Song*);
	void		Init();
	void		Restart();
	void		Close();
	
	void		GetXRSDirectoryEntry(entry_ref*,const char*);
	void		ErrorMsg(const char *msg,const char *txt);
	BMenu*	getMenu() {return myMenu;}
	
	void	RefreshSelected();
	void	Refresh(JTrack* t);
	protected:
	friend TracksPanel;	
		TracksPanel*	curPanel;
	
	
	private:
		
		void		LoadPlugin(const char *name,int i);
		
		BMenu		*myMenu;
		JTrack		*j;
		Track		*t;
		TrackBoost	*list[MAX_PLUG];	//Only up to 16 plug_ins?
		JTrack		*curJTrack;
		MBWindow 	*mb;
		Song*		curSong;
		
		
};



#endif