
#ifndef _X_PANEL_
#define  _X_PANEL_

#include "View.h"
#include "GlobalDef.h"



class Song;
class BankManager;
class PatControl;
class Pattern;
class TracksPanel;

class XPanel : public BView
{
	public:
						XPanel (BRect,BView*);
		virtual 	void		MessageReceived(BMessage* message);
				status_t	Init();
				void		Reset(Song *,TracksPanel*);
				void		SetTimeText(int);
		virtual 	void		KeyDown(const char *bytes,int32);
		virtual 	void		KeyUp(const char *bytes,int32);
				bool		isAllPat();
				int		getCurrentPattern();
				Pattern*	currentPattern();
		virtual void			MouseDown(BPoint);
			void			ResetMeasureCount();
	
	private:
	
		void	SelectTrack(int);
	
	
		PatControl 		*curpat;
	
		BPictureButton 	*play_bt;
		BPictureButton 	*all_bt;
		BPictureButton		*edit;
		BPictureButton		*vst;
		
			
		BSlider			*tempo;
		BSlider			*sl_cut;
		BSlider			*sl_reso;
		BPictureButton		*filter_on;
		PatControl		*tempo_text;
		
		Song			*curSong;
		int			curPattern;
		
		char			ttext[4];
		bool 			key_rel;
		
		TracksPanel	*tp;
};
#endif

