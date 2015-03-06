#ifndef _CONFIGURATOR_H_
#define _CONFIGURATOR_H_

#include "Message.h"
#include "File.h"
#include "Path.h"

// 	~Configurator();
 	
 void	cf_Init(const char*);
 void    cf_Close();
 float	FloatKey(const char*,float);
 void	PutFloatKey(const char*,float);
 void	PutString(const char*,const char*);
 void	String(const char*,const char*,const char*,int);
status_t	Ref(const char *key,entry_ref *ref,const char *def);
 void	PutRef(const char *key,entry_ref *ref);
 void	ReplaceRef(const char *key,entry_ref *ref);

 static BMessage*	cf_in_msg;
 static BMessage*	cf_out_msg;
 static BFile*		cf_file;
 static BPath		 cf_path;


#endif