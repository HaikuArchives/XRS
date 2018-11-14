#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include "TimedEventQueue.h"

/*
#include "String.h"
#include "List.h"
*/

//#define	MAX_SEQUENCE 99

class BList;
class BString;

class Sequence
{
	public:
	
			Sequence();
		
		int	ItemAt(int);
		int	Reset();
			void	setItemAt(int,int,bool f=false);
		
			int	getMaxSeq();
			int	getMaxPat();
			void	setMaxPat(int);
			void	setMaxSeq(int);
			void	RemoveMeasure(int pos);
			void	AddMeasure(const char*);
		BString*	GetMeasureName(int);
		void		SetMeasureName(const char*,int);
		
	private:

		//int 	matrix[MAX_SEQUENCE];
		
		BTimedEventQueue*	matrix2;
		
		BList*	names;
		int	MaxSeq,MaxPat;
};

#endif