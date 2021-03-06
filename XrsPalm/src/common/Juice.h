#ifndef JUICE_H
#define JUICE_H

#include <Window.h>

#include "MediaDefs.h"

#include "Track.h"


#include "struct_ev.h"
#include "Song.h"
#include "Window.h"
#include "Path.h"

class BSoundPlayer;
class TileView;
class BMenu;
#ifdef ENABLE_VST
class VSTFilterPlugin;
class VCF;
class VSTWindow;
#endif
class BBox;
class BPath;
class MatrixWindow;
class JFileManager;

class Juice:public BLooper
{


 public:

					Juice(MatrixWindow*);
					~Juice();
			status_t 	Init(Song*,BWindow*,int32);
			void		Start(bool,int);
			void		Stop();
			void		Pause();
			bool		isPlay();
			void		Reset(bool allpat,int);
	virtual 	void		MessageReceived(BMessage* message);
	virtual	bool	 	QuitRequested();

#ifdef ENABLE_VST
			void		ShowVstManager();
			void		ResetVstTempo(VSTFilterPlugin*);

			BMessage	*getVSTData(int,int);
			void		setVSTData(int);
#endif

			int		getTempo();

		//bisogna renderlo privato..
			 int post_roll;
			static	void ProcessBuffer(
				void * cookie,
				void * data,
				size_t size);

			static	void PlayFileX(
				void * cookie,
				void * data,
				size_t size,
				const media_raw_audio_format & format);



			int		beatpos;
			int		row;
			static void 	process_row(Juice *);
			static int	min(int a, int b) { return a<b ? a:b; }


			size_t			last_buf_good;
			Song*			curSong;
			void			setSong(Song*);
			size_t			note_size;




		int		nextPattern(int,size_t,size_t);
		int		getPattern();
		void		setPattern(int);
		void		setPosition(int);
		void		setPatternMode(bool);


protected:

#ifdef ENABLE_VST
			friend VSTWindow;
			VSTWindow*		vstw;
			BList*			vst[3];
			void			CloseVst();
#endif

friend JFileManager;


private:


			MatrixWindow		*mw;
#ifdef ENABLE_VST
			BList*			vst1;
			BList*			vst2;
#endif
			BList*			global;

			int 			pospat;
			bool			isp;
			bool			isPlaying;

			TileView*		 	osk;
			BSoundPlayer*			player;

			media_raw_audio_format	fmt;

			BMessage 			*ck_mes;
			BMessage			*buf_mes;
			BMessage			*meterl_mes;

			void*			preroll[2];
			int			preroll_pos;
			size_t			maxbuf;



			// Variabili per l'Oscy..
			//size_t			last_count;
			//void*			last_data;
			//

			void			AddEv(size_t,Track*,int);
			void			SubEv(size_t,ev*);
			void			FreeEv(ev*,Track*);
			ev*			GetEv(int,Track*);
			size_t			mix2(void*,Track*,size_t,size_t,float,float);
			size_t			mix(void*,void*,size_t,float,float);
			void			mixEv(Track*,void*,size_t);
			void			FreeAllEv(Track*);
			void			mixDown(float**,void* ,size_t,float*);
			void			melt(float**,float**,size_t);
			int16			default_data;

			bool			all_patterns;
			size_t			rikko[16];

			int			render;


			BWindow	*look;


#ifdef ENABLE_VST
			VCF		*xVcf;
#endif

			//size_t note_size;

			float inputsl[1];
			float inputsr[1];
			float *ptr[2];
			float outputsl[1];
			float outputsr[1];
			float *optr[2];

			float *stream_normal[2];
			float *stream_normal_left;
			float *stream_normal_right;

			float *stream_fx1[2];
			float  *stream_fx1_left;
			float  *stream_fx1_right;

			float *stream_fx2[2];
			float  *stream_fx2_left;
			float  *stream_fx2_right;

			float *stream_service[2];
			float *stream_service_left;
			float *stream_service_right;

			/*float *stream_service2[2];
			float *stream_service_left2;
			float *stream_service_right2;*/

		            BPath	vstpath;

		            float vcf_cutoff;
		            float vcf_resonance;
		            float top[2];

};
#endif
