
#include <Application.h>

#include "MainWindow.h"
#include "Song.h"
#include "JFileManager.h"

class Juice;
class Pattern;
class BList;
class TrackManager;

class TheApp : public BApplication {

public:

		
				TheApp(const char * signature);
	virtual	void	MessageReceived(BMessage* message);	
	virtual	 bool 	QuitRequested();
	virtual	void 	RefsReceived(BMessage *);
	virtual	void	AboutRequested();
		
			Song	*getSong();
		
		

private:

		void				CopyPattern(Pattern*,Pattern*);
		void				ClearPattern(Pattern*);
		/* questi lavorano su DefaultSong : */
		status_t			LoadRequest(entry_ref);
		void				EmptySong();
		void				DefaultSong();
			
		MainWindow*			S;
		Song*				currentSong;	
		BHandler*			look;
		Juice*				succo;	
		JFileManager			*jfm;
		TrackManager 		*tm;
		/* ClipBoardz */
		Pattern				*clipboard;
		BList				*dinamic_clip;						
		/**/		
};
