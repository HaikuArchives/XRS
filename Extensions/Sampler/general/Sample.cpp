#include "Sample.h"


Sample::Sample()
{	
	path_name=NULL;
	
}
Sample::~Sample()
{
	if(data!=NULL) free(data);
	if(path_name!=NULL) free(path_name);
}