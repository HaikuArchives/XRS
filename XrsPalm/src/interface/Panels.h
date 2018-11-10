
#include "Window.h"
#include "Entry.h"

class JFileManager;
class PatControl;
class BRadioButton;
class Song;
class BTextView;
class BCheckBox;
class BFilePanel;

class ExportPanel: public BWindow
{
	public:
		 	ExportPanel(int k,int m,JFileManager*);
	 virtual void	MessageReceived(BMessage*);
	 
	 private:
		BFilePanel *openpanel;
		JFileManager *jfm;
		PatControl *mea;
		BRadioButton *single;
	
};

class PrefsPanel: public BWindow
{
	public:
			PrefsPanel();
	virtual void	MessageReceived(BMessage*);
	
	private:
		BFilePanel *openpanel;
		entry_ref rif;
		
};
class SongSettings: public BWindow
{
	public:
			SongSettings();
	virtual void	MessageReceived(BMessage*);
	void Init(Song*);
	private:
		Song*	curSong;
		BCheckBox*	cb;
		BTextView* tw;
		PatControl *mea;
		PatControl *beat;		
};
