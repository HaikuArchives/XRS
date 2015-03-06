#ifndef _SAMPLERTRACK_H_
#define _SAMPLERTRACK_H_

#include "Track.h"

class Sample;

#define	 FRAMES_NUM	1024

class aflibConverter;

class SamplerTrack: public Track
{
	public:
	
						SamplerTrack();
		virtual			~SamplerTrack();
			
				bool		isReversed();
				void		setReversed(bool);
	
			
		virtual	void		Reset();
		virtual 	int		getModel();	
		virtual	void		NoteReady(int,int,float);
		virtual	void		Process(float**,int32,int);
		virtual	const char*	getPreferredName();
		virtual	void		goOn();
		
				Sample*	getSample();
				void		setSample(Sample*);
				aflibConverter	*converter;

		//protected??
		
		void	setResample(int);
		int	getResample();
		void	setResampleEnable(bool);
		bool	isResampleEnable();
		
		/*RAW*/
		float	amp;
		
	
private:
	
		static 		int		min(int a, int b) { return a<b ? a:b; }
				size_t 		mix2(float** dest,size_t, size_t pos , size_t size,float);
				void		getSampleData(void*,size_t,size_t);
				
		int		note_status;
		Sample*	curSample;
		bool		reversed;
		uint32		virtualbytes;
		int32		virtualposition;	 // misurati in SAMPLES!
		
		void*		myBufferino;
		int32		posSample;
		float		coef;
		float		samplesperbeat;
		float		numNotes;
		bool		res_enable;
		bool		need_res;
		
		};
#endif