#include "MListItem.h"
#include "string.h"
#include "malloc.h"
#include "Message.h"

#define	XRS_BANK_SAMPLE	'xbks'

MListItem::MListItem(entry_ref *r,const char*name,bool isFolder,int32 spiaz,int32 size):BStringItem(name)
{
	if(spiaz==-1 || size==-1)
	{
		if(!isFolder){
		msg=new BMessage(B_SIMPLE_DATA);
		msg->AddRef("refs",r);
		}
		else
		{
			msg=NULL;
		}
	}
	else
	{
		msg=new BMessage(XRS_BANK_SAMPLE);
		msg->AddRef("refs",r);
		msg->AddString("name",name);
		msg->AddInt32("spiaz",spiaz);
		msg->AddInt32("size",size);
	}
}

BMessage*
MListItem::getMessage()
{
	return msg;	
}
