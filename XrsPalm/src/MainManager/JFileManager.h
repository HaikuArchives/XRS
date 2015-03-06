#ifndef _JFILEMANAGER_H_
#define _JFILEMANAGER_H_
// JFileManager v0.0.0.0.0.1

#include  <File.h>
#include "NodeInfo.h"

class Juice;
class Song;
class SJFilter; 
class TrackManager;

class JFileManager
{
	public:
		JFileManager();
		~JFileManager();
	
	status_t	SaveFile(entry_ref,Song*); 	//The name	??
	status_t	LoadFile(entry_ref,Song*);	//The name    ??
	
	
	Song*		EmptySong();
	
	
	void		CloseSong(Song*,bool);	
	
	void		Init(Juice*,TrackManager*);
	
	status_t	Load();
	//status_t	LoadSample(BMessage*);
	//void		setRef(entry_ref);
	
	void		Save(Song*,bool);
	
	void		SaveReq(entry_ref,const char*,Song*);
	
	status_t	RenderAsWave(entry_ref);
	
	void		ExportWave(bool,int);
	
	void		AddMime(BFile*);
	
	int		save_type;
	int		open_type;
		
	status_t	AskForClose(Song*);
	
	private:
	
		
		void	ErrorMsg(const char *msg,const char *txt);
		void	WriteFixed(int64,int64);
		int64	ReadFixed(int64*);
		void	WriteVar(int64,void*,int64);
		int64	ReadVar(void*);
		
		BFile*			file;
		char*			x;
		Juice 		*succo;
		
		BFilePanel*	filepanel;
		BFilePanel*	openpanel;


		SJFilter 	*filtro;
		TrackManager*	tm;
		BDirectory 	*dir;

		BCheckBox	*chek;
		bool		allpat;
		int		poz;
		
		entry_ref sl;

};
#endif