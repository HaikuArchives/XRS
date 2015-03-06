#include "GlobalDef.h"
#include "List.h"
#include <MediaFile.h>
#include <MediaTrack.h>
#include <Entry.h>

class Sample;

class BFile;
class ExternalManager 
{
	public:
					ExternalManager();
					~ExternalManager();
			
			Sample*	getSampleAt(int);	
			BList*		getSampleList();
			status_t	AddSampleList(BList*);
			status_t	FindSample(const char*);
			int32		CountItems();
			status_t	InitCheck();
			void		Empty();
			status_t	AddSample(entry_ref ref);
			
			status_t	AddBankSample(BMessage*);
			
		static status_t	LoadFile(entry_ref,BList*);
		static status_t	ExtractSample(entry_ref,int32,int32,const char*,BList *sample_list);
		
			entry_ref	the_ref;

	private:
	
		
		
	
			BList*		samples_list;
			status_t	lastStatus;
};