
#include "Juice.h"

#include "SoundPlayer.h"

#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "assert.h"

#include "List.h"
#include "Message.h"
#include "Pattern.h"
#include "GfxMsg.h"

#include "GlobalDef.h"
#include "Application.h"

#include "PopUpMenu.h"
#include "Box.h"
#include "Entry.h"

#include "Configurator.h"
#include "MenuItem.h"

#ifdef ENABLE_VST
#include "VSTFilter.h"
#include "VCF.h"
#endif
#include "Sequence.h"
#include "MatrixWindow.h"

#include "OS.h"
#include "async.h"
#ifdef ENABLE_VST
#include "VSTWindow.h"
#endif


#define	 PREROLL_NUM	 2
#define	 PREROLL_DIM  	16384
#define	 FRAMES_NUM	1024
#define	 BUF 			 'buf'

#define 	JUICE_PRIORITY	B_URGENT_PRIORITY
#include 	"synel.h"


sem_id		stresaforo;

Juice::Juice(MatrixWindow *matrix) :
	BLooper("JuiceKiller 1.1 Oz",JUICE_PRIORITY) //

{

#ifdef ENABLE_VST
	xVcf=new VCF();
	xVcf->Init(2,0);
#endif


	preroll_pos=0;
	preroll[0]=malloc( FRAMES_NUM*4);
	preroll[1]=malloc( FRAMES_NUM*4);


	buf_mes=new BMessage(BUF);
	ck_mes=new BMessage('Mtr');
	ck_mes->AddInt16("beat",0);
	ck_mes->AddInt16("pat",0);

	meterl_mes=new BMessage(ASYNC);
	meterl_mes->AddFloat("value_l",0);
	meterl_mes->AddFloat("value_r",0);

	default_data=0;
	isp=false;
	isPlaying=false;

	Run();


	ptr[0]=(float*)&inputsl;
	ptr[1]=(float*)&inputsr;
	optr[0]=(float*)&outputsl;
	optr[1]=(float*)&outputsr;

	stream_normal_left=new float[FRAMES_NUM];
	stream_normal_right=new float[FRAMES_NUM];

	stream_normal[0]=stream_normal_left;
	stream_normal[1]=stream_normal_right;

	stream_fx1_left=new float[FRAMES_NUM];
	stream_fx1_right=new float[FRAMES_NUM];

	stream_fx1[0]=stream_fx1_left;
	stream_fx1[1]=stream_fx1_right;

	stream_fx2_left=new float[FRAMES_NUM];
	stream_fx2_right=new float[FRAMES_NUM];

	stream_fx2[0]=stream_fx2_left;
	stream_fx2[1]=stream_fx2_right;

	stream_service_left=new float[FRAMES_NUM];
	stream_service_right=new float[FRAMES_NUM];

	stream_service[0]=stream_service_left;
	stream_service[1]=stream_service_right;

	top[0]=0;
	top[1]=0;

#ifdef ENABLE_VST
	vst1=new BList(0);
	vst2=new BList(0);
#endif
	global=new BList(0);

#ifdef ENABLE_VST
	vst[0]=vst1;//NULL;
	vst[1]=vst2; //NULL;
	vst[2]=global;
#endif


	mw=matrix;

	stresaforo=create_sem(1, "stresaforo");

	release_sem(stresaforo);

#ifdef ENABLE_VST
	vstw=new VSTWindow();
#endif

	curSong=NULL;


}

#ifdef ENABLE_VST
void
Juice::ShowVstManager()
{
	if(vstw->Lock())
	{
		if(vstw->IsHidden())
		vstw->Show();
			else
		vstw->Hide();

		vstw->Unlock();
	}
}
#endif

Juice::~Juice()
{

	player->Stop();

	delete_sem(stresaforo);

#ifdef ENABLE_VST
	CloseVst();
#endif

	delete player;

#ifdef ENABLE_VST
	if(vstw->Lock())	vstw->Quit();
#endif
}

#ifdef ENABLE_VST
BMessage*
Juice::getVSTData(int line,int pos)
{
	if(pos >= vst[line]->CountItems()) return NULL;

	BMessage* temp=new  BMessage();

	((VSTFilterPlugin*)vst[line]->ItemAt(pos))->GetConfig(temp);

	vstw->AddVstWindowInfo(temp,line,pos);

	return temp;
}
void
Juice::setVSTData(int line)
{
	/*
	if(curSong->vstdata[line]==NULL) return;

	((VSTFilterPlugin*)vst[line]->ItemAt(0))->SetConfig(curSong->vstdata[line]);
	float x,y;

	x=curSong->vstdata[line]->FindFloat("Xpos");
	y=curSong->vstdata[line]->FindFloat("Ypos");

	//vst_win[line]->MoveTo(x,y);
	*/
}
#endif

bool
Juice::QuitRequested()
{
	return BLooper::QuitRequested();
}
bool
Juice::isPlay()
{
	return isPlaying;
}
void
Juice::MessageReceived(BMessage* message)
{

	int z;


	switch(message->what)
	{

		case BUF:
			if(preroll_pos==1)
				preroll_pos=0;

			else
				preroll_pos=1;


		ProcessBuffer((void*)this,preroll[preroll_pos], FRAMES_NUM*4);

		break;

		case TEMPO_SET:
		//break;
		case TEMPO_MOV:

			z=(int)message->FindInt32("be:value");
			curSong->setTempo(z);
			note_size=curSong->getNoteSize();
#ifdef ENABLE_VST
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<vst[i]->CountItems();j++)
				{
					((VSTFilterPlugin*)vst[i]->ItemAt(j))->setBPM(z);
				}


			}
#endif
		break;
		default:
			BLooper::MessageReceived(message);
		break;
	}
}
int
Juice::getTempo()
{
	return curSong->getTempo();

}
status_t
Juice::Init(Song* s,BWindow* view,int32 msg)
{

	//xVcf->Init(2,0);

	status_t err;

	curSong=s;

	look=view;


	//ck_mes=new BMessage(CK_MESSAGE);

	fmt.format=media_raw_audio_format::B_AUDIO_SHORT;
	fmt.channel_count=2;
	fmt.frame_rate=44100;
	fmt.byte_order=B_MEDIA_LITTLE_ENDIAN;
	fmt.buffer_size= FRAMES_NUM*4;

	player=new BSoundPlayer(&fmt,"SupernaturalJuice",PlayFileX,NULL,this);

	err=player->InitCheck();

#ifdef ENABLE_VST
	vstw->Init(this);
	look->AddToSubset(vstw);
	vstw->Show();
	vstw->Hide();
#endif

	return err;
}

#ifdef ENABLE_VST
void
Juice::	ResetVstTempo(VSTFilterPlugin* p)
{
	//printf("succo tempo %d\n",getTempo());
	p->setBPM(getTempo());
}
#endif

void
Juice::setSong(Song *s)
{
	curSong=s;
}
int
Juice::nextPattern(int act,size_t now_pos,size_t allinea)
{
	if(all_patterns)
	{
		/* Case :  ALL PLAY */

		/* Please note : this if is works only when we are Exporting as Wave */
		if(mw->getStep()>=curSong->getSequence()->getMaxSeq())	last_buf_good=render;

		int r= mw->nextPattern(act);

		//printf("Current %d Next %d\n",pospat,r);

		return r;
		//return mw->nextPattern(act);


	}

	else
		{
			/* Case : Single pattern playing */
			last_buf_good=render;
			return act;
		 }


}
int
Juice::getPattern()
{
	if(all_patterns) return mw->getPattern();
	else return pospat;
}

void
Juice::setPattern(int val)
{
	if(all_patterns) mw->step();

}
void
Juice::setPatternMode(bool allpat)
{
	all_patterns=allpat;
}
void
Juice::setPosition(int val)
{
	pospat=val;
}
void
Juice::Reset(bool allpat,int poz)
{
	maxbuf=0;

	row=0;
	beatpos=0;

	all_patterns=allpat;
	pospat=poz;
	last_buf_good=0;

	note_size=curSong->getNoteSize();

	for(int y=0;y<curSong->getNumberTrack();y++)

			//curSong->getTrackAt(y)->Reset();
			curSong->getTrackAt(y)->Message(SystemReset,getPattern());



	printf("Juice::Reset * \n");
}
void
Juice::Start(bool allpat,int poz)
{
	if(isPlaying) { Stop(); return; }

	Reset(allpat,poz);

	process_row(this);

	ProcessBuffer((void*)this,preroll[0], FRAMES_NUM*4);

	printf("Juice::Started *\n");

	player->Start();
	isPlaying=true;
}
void
Juice::Pause()
{
	if(isPlaying)
		{
			player->Stop();
			isPlaying=false;
			isp=true;
			if(acquire_sem(stresaforo) != B_NO_ERROR)
			return;

		}
	else
		if(isp)	{
				release_sem(stresaforo);
				isPlaying=true;
				isp=false;
				player->Start();

			}


}
void
Juice::Stop()
{
	maxbuf=0;

	printf("Juice::Stopped [NodeLatency %lld]\n",player->Latency());
	player->Stop();
	isPlaying=false;
	preroll_pos=0;

	for(int y=0;y<curSong->getNumberTrack();y++)

					curSong->getTrackAt(y)->Message(SystemStop,getPattern());

	meterl_mes->ReplaceFloat("value_l",0);
	meterl_mes->ReplaceFloat("value_r",0);
	look->PostMessage(meterl_mes);

}

void
Juice::PlayFileX(
	void * theCookie,
	void * buffer,
	size_t size,
	const media_raw_audio_format & format)
{
	Juice *succo=(Juice*)theCookie;
	succo->post_roll=succo->preroll_pos;


	succo->look->PostMessage(succo->ck_mes);
	succo->look->PostMessage(succo->meterl_mes);

	succo->PostMessage(succo->buf_mes);
	memcpy(buffer,succo->preroll[succo->post_roll],size);

}

void
Juice::mixDown(float**optr,void* to,size_t size,float* top)
{
	int16 data[2];


	// Do you have a meter?
	top[0]=0;
	top[1]=0;

	for(uint32 x=0;x<size/4;x++)
	{
		memcpy((char*)&data[0],(char*)to+x*4,2);
		memcpy((char*)&data[1],(char*)to+x*4+2,2);

#ifdef ENABLE_VST
		if(curSong->isFilterOn()){
			optr[0][x]=(float)xVcf->Process(optr[0][x],0,curSong->getVCFCutoff(),curSong->getVCFResonance());
			optr[1][x]=(float)xVcf->Process(optr[1][x],0,curSong->getVCFCutoff(),curSong->getVCFResonance());
			}
#endif

		// Do you have a meter??
		if(fabs(optr[0][x])>top[0]) top[0]=fabs(optr[0][x]);
		if(fabs(optr[1][x])>top[1]) top[1]=fabs(optr[1][x]);


		optr[0][x]=optr[0][x]*32766. + data[0];
		optr[1][x]=optr[1][x]*32766. + data[1];

		//Limiter!

		if(fabs(optr[0][x])>=32766) optr[0][x]=(int32)copysign(32767,optr[0][x]);
		if(fabs(optr[1][x])>=32766) optr[1][x]=(int32)copysign(32767,optr[1][x]);

		data[0]= (int16)optr[0][x];
		data[1]= (int16)optr[1][x];

		memcpy((char*)to+x*4,&data,4);
	}

	meterl_mes->ReplaceFloat("value_l",top[1]);
	meterl_mes->ReplaceFloat("value_r",top[0]);
}

void
Juice::melt(float **a,float**b,size_t size){

	for(size_t i =0 ; i<size ; i++){

		b[0][i] += a[0][i];
		b[1][i] += a[1][i];


	}
}

void
Juice::ProcessBuffer(
	void * theCookie,
	void * buffer,
	size_t size)
{

	if (acquire_sem(stresaforo) != B_NO_ERROR) return;


	Juice *succo=(Juice*)theCookie;

	Song * song =succo->curSong;


	Track*	tmp;


	memset((char*)buffer, succo->default_data, size);


	int32 trip=succo->curSong->getNumberTrack();

	memset((char*)succo->stream_normal[0], 0, size);
	memset((char*)succo->stream_normal[1], 0, size);
	memset((char*)succo->stream_fx1[0], 0, size);
	memset((char*)succo->stream_fx1[1], 0, size);
	memset((char*)succo->stream_fx2[0], 0, size);
	memset((char*)succo->stream_fx2[1], 0, size);


	/* New Juice Engine 'G' version

		1) Dividere il buffer in "note_size"

		2) Per ogni traccia mandare il bufferino da riempire.

		3) Per ogni nuova nota che sta' del buffer chiamare i NoteReady(note_pos)
			delle traccie. e saltare al punto 2.

	*/



	int buflen;
	bool fx1=false;
	bool fx2=false;
	int len=size/4;
	int tot=len;
	int sampsperbeat=succo->note_size/4;


	/* Punto 1 : Divisione in sotto bufferini */

	while(len) {

		buflen=min(len, (int)ceil(sampsperbeat-succo->beatpos));
		succo->beatpos+=buflen;

		/* Punto 2 : Processiamo il bufferino di ogni traccia.. */

		if(buflen>0 && buflen<=tot)
			for(int x=0;x<trip;x++) // Manca anche tutto il grattacazzo dei VST!
			{
				tmp=(Track*)song->getTrackAt(x);
				if(!tmp->isFX1On())
						{//	NORMAL PROCESS
						if(tmp->isOn()) tmp->Process(succo->stream_normal, buflen, tot-len);

						}
				else
					{
						// VST PROCESS
					if(tmp->isOn()) {

								if(tmp->getFX1Line()==0)
										{
										  tmp->Process(succo->stream_fx1, buflen, tot-len);
					 					  fx1=true;
					 					}
					 			else	{
										 tmp->Process(succo->stream_fx2, buflen, tot-len);
					 					 fx2=true;
									}
					 		  }
					}
			}



		if(succo->beatpos>=sampsperbeat) {




			succo->render=-1*(tot-len-buflen);
			//printf("beatpos %d \n",succo->render);
			succo->beatpos-=sampsperbeat;
			/* Punto 3 */
			process_row(succo);
		}


		len-=buflen;
	//printf("____pos %d \n",tot-len);

	}


#ifdef ENABLE_VST
		if(fx1){

			for(int i=0;i<succo->vst[0]->CountItems();i++)
			{
				VSTFilterPlugin *vfp=( (VSTFilterPlugin*)succo->vst[0]->ItemAt(i) );
				vfp->FilterFloat(succo->stream_fx1,succo->stream_fx1,size/4,NULL);
			}
			succo->melt(succo->stream_fx1,succo->stream_normal,size/4);

			}

		if(fx2){
			for(int i=0;i<succo->vst[1]->CountItems();i++)
			{
				VSTFilterPlugin *vfp=( (VSTFilterPlugin*)succo->vst[1]->ItemAt(i) );
				vfp->FilterFloat(succo->stream_fx2,succo->stream_fx2,size/4,NULL);
			}
			succo->melt(succo->stream_fx2,succo->stream_normal,size/4);

		}

		// Global VST Line:
		for(int i=0;i<succo->vst[2]->CountItems();i++)
			{
				VSTFilterPlugin *vfp=( (VSTFilterPlugin*)succo->vst[2]->ItemAt(i) );
				vfp->FilterFloat(succo->stream_normal,succo->stream_normal,size/4,NULL);
				//
			}
#endif

		//printf("Audio stream %ld \n",succo->vst[2]->CountItems());
		succo->mixDown(succo->stream_normal,buffer,size,succo->top);


	 release_sem(stresaforo);
}
void
Juice::process_row(Juice* succo)
{
	Track*	tmp;
	int32 trip=succo->curSong->getNumberTrack();

	if(succo->getPattern()!=EMPTY_PATTERN)

		while(trip--)	//for(int x=0;x<trip;x++)
		{
			//tmp=(Track*)succo->curSong->getTrackAt(x);
			tmp=(Track*)succo->curSong->getTrackAt(trip);
			tmp->NoteReady(succo->row,succo->getPattern(),succo->note_size);
		}

	else
		while(trip--) //for(int x=0;x<trip;x++)
		{
			tmp=(Track*)succo->curSong->getTrackAt(trip);
			tmp->NoteReady(-1,EMPTY_PATTERN,succo->note_size);
		}

	/* Lucine !  */
	//succo->ck_mes=new BMessage('Mtr');
	succo->ck_mes->ReplaceInt16("beat",succo->row);
	succo->ck_mes->ReplaceInt16("pat",succo->getPattern());
	/*           */

	succo->row++;

	if(succo->row > succo->curSong->getNumberNotes()-1) {



		succo->row=0;
		succo->nextPattern(0,0,0);	// just do 'what is the next pattern ?'
		succo->setPattern(0);		// just do 'go to next pattern..'

	}
}

#ifdef ENABLE_VST
void
Juice::CloseVst()
{
	if(vstw->Lock())
	{
		vstw->CloseVst();
		vstw->Unlock();
	}
	for(int j=0; j<3;j++)
	{
		for(int i=0;i<vst[j]->CountItems();i++)
		{
			VSTFilterPlugin  *dv=(VSTFilterPlugin *)vst[j]->ItemAt(i);
			delete dv;
		}
		vst[j]->MakeEmpty();
	}

}
#endif
