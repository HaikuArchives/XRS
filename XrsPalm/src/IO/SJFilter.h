#include <FilePanel.h>
#include "string.h"

class SJFilter:public BRefFilter
{
public:
		SJFilter();
	virtual bool Filter(const entry_ref *erf,BNode *node,struct stat *st,const char *filetype);
};
