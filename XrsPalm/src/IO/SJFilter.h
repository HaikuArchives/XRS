#include <FilePanel.h>
#include <compat/sys/stat.h>
#include "string.h"

class SJFilter:public BRefFilter
{
public:
		SJFilter();
	virtual bool Filter(const entry_ref *erf,BNode *node,struct stat_beos *st,const char *filetype);
};
