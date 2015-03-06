#include "TrackBoost.h"
#include "Application.h"
#include "File.h"
#include "TrackManager.h"

TrackBoost::TrackBoost()	
{
	
}
void
TrackBoost::WriteFixed(int64 code ,int64 data,BFile* file )
{
	file->Write(&code,sizeof(int64));
	file->Write(&data,sizeof(int64));
	return;
}
void
TrackBoost::WriteVar(int64 code ,void* data,int64 size,BFile* file)
{
	WriteFixed(code,size,file);
	file->Write(data,size);		
}

int64
TrackBoost::ReadFixed(int64 *data,BFile* file)
{
	int64 code;
	file->Read(&code,sizeof(int64));
	file->Read(data,sizeof(int64));
	return code;	
}

void
TrackBoost::Init(TrackManager*t) {tm=t;}
void
TrackBoost::RefreshGraphics(){tm->getAllMyJTrack(NULL,0);}
void
TrackBoost::RefreshSelected(){tm->RefreshSelected();}
void
TrackBoost::Refresh(JTrack* t){tm->Refresh(t);}