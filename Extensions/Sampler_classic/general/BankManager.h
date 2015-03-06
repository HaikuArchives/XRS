#include "GlobalDef.h"
#include "List.h"
#include <MediaFile.h>
#include <MediaTrack.h>
#include <Entry.h>

class Sample;

class BFile;
class BankManager 
{
	public:
					BankManager(const char*);
					~BankManager();
			Sample*	getSampleAt(int);	
			BList*		getSampleList();
			int32		CountItems();
			status_t	InitCheck();
	
		static status_t		LoadDefaultBank(const char*,BList*);

		entry_ref	the_ref;
	private:
	
		
		static int64		ReadFixed(int64 *data,BFile*);
		static char 		space;
			BList*		samples_list;
		status_t		lastStatus;
			

};