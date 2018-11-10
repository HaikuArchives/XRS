
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
 
#include <Window.h>

class Song;
class XPanel;
class Juice;
class BMenu;
class JFileManager;
class TracksPanel;

class Pattern;
class MatrixWindow;
class TickView;
class TrackManager;
class Track;

class MainWindow:public BWindow
{



	 public:
					MainWindow();
				void	Close();
			
		virtual	void	MessageReceived(BMessage* message);
				void	Reset(Song*,bool juicereset);
				void	Init(JFileManager*,TrackManager*);
		virtual bool		QuitRequested();
		virtual void WindowActivated(bool active);
				void	Saved();
				//void	RefreshPopUp(int h); //nome inappropriato
		//BHandler*		getHandler();
				
				
				Track* 	getCurrentTrack();
				int	getCurrentPattern();
				void		setTickValue(float,int);
			
	private:
			//void 		MakeBanksMenu();
			status_t	AskRemove(const char*);
			
			
			MatrixWindow	*mw;
			
			Song*		curSong;
			XPanel 		*panel;
			TracksPanel	*tp;						
			Juice*		succo;
			JFileManager	*jfm;
			TrackManager	*tm;
			TickView	*ticks;
			//BMenu		*menuBanks;
			
			/*Up Menu */
			
			BMenuItem	*saveas_item;
			BMenuItem	*save_item;	
			BMenuBar	*menuBar;
			BMenu		*menuFile;
			BMenuItem	*copy;
			BMenuItem	*cut;
			BMenuItem	*paste;
			BMenuItem	*mea_copy;
			BMenuItem	*mea_cut;
			BMenuItem	*mea_paste;
			//BMenuItem	*none_bank;
			
			
};

#endif
