#ifndef _TRACK_H_
#define _TRACK_H_

#include "GlobalDef.h"
#include "SupportDefs.h"
#include "List.h"
#include "synel.h"
#include "String.h"

class Pattern;
class Note;

class Track
{
	public:
	
				   Track();
		virtual	~Track();
		
	
		virtual	Pattern*	getCurrentPattern();		
		virtual 	void		goOn(){}
		virtual 	void		goOff(){}
		virtual	void		Reset();
		virtual	void 		Message(SynthMessage msg, float data);
		virtual	void		NoteReady(int,int,float);
		virtual	void		Process(float**,int32,int);
		virtual 	int		getModel();	
		virtual 	const char*	getPreferredName();	
		

		//Measures
		int			getType();
		void			setType(int);
		Pattern*		getPatternAt(int);
		void			moveToPattern(int);
		float			getVolume();
		void			setVolume(float);
		float			getPan();
		void			setPan(float);
		bool			isOn();
		void			setOn(bool);
		bool 			isFX1On();
		void			setFX1On(bool);
		int			getFX1Line();
		void			setFX1Line(int);
		void			AddMeasure(int val,int);
		void			RemoveMeasure(int num);
		void			setName(const char*);
		void			LockName(bool b);
		const char*		getName();
		void			ResetName();
		bool			isNameLocked();
		void			setNumberNotes(int);
		
		
protected:
		float		vol_left;
		float  		vol_right;
		Note*		curNote;
		Pattern*	currentPattern;
		BList*		patternList;
		int		type;			/* Track Type */
		void		CalcVols();
		
private:

	
		BString		name;
		bool		name_set;
		bool		on;
		bool		fx1;
		int		vst_line;
		float		volume;
		float		pan;	
			
		
};
#endif