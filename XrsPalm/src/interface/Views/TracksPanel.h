#ifndef _TRACKS_PANEL_
#define  _TRACKS_PANEL_

#include "View.h"
#include "GlobalDef.h"
#include "List.h"

class Song;
class Pattern;
class JTrack;
class Track;
class TrackManager;

class TracksPanel : public BView
{
	public:
						TracksPanel (BRect,BView*);
						
		virtual 	void		MessageReceived(BMessage* message);
		virtual 	void		MouseDown(BPoint);
		
				status_t	Init(TrackManager*);
				void		Reset(Song *,int);
				int		getNumberJTrack();			
				void		RemoveTrackAt(int id);
				void		RemoveTrack(int);
				Track*		getCurrentTrack();
				void		SelectTrack(int);
				void		AddTrack(int h);
				
				void		setPattern(int);
				int		getCurrentPattern();
								
				void		muteTrack(Track*);
				void		soloTrack(Track*);	
				
	protected:
	friend TrackManager;
						JTrack*	getJTrackAt(int h);		
						void		RefreshGraphics();	
						void		RefreshSelected();
						void		Refresh(JTrack* t);
	private:
	
		
		
		BList		*xnv;
		BView		*w;
		Song		*curSong;
		TrackManager	*tm;
		int		selPattern;
		
};

#endif

