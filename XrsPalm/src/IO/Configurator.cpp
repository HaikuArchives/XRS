#include "Configurator.h"
#include "stdio.h"
#include "FindDirectory.h"
#include "string.h"
#include "Entry.h"

void
cf_Init(const char *filename)
{
	// Carica il file se c'e' ,altrimenti lo crea,
	// se riesce a ciucciare il messaggio lo setta come il corrente
	// e ne prepara uno nuovo dove tutti possino scrivere i prori cazzi.


	find_directory (B_SYSTEM_SETTINGS_DIRECTORY, &cf_path, true);
	cf_path.Append (filename);

	status_t err;

	cf_in_msg=new BMessage();
	cf_out_msg=new BMessage();

	cf_file =new BFile (cf_path.Path (), B_READ_WRITE | B_CREATE_FILE);
	if (cf_file->InitCheck () == B_OK)
	{

		err=cf_in_msg->Unflatten(cf_file);
		if(err!=B_OK){
				delete cf_in_msg;
				cf_in_msg=NULL;
				}
			else
				cf_out_msg=cf_in_msg;

	}
}

void
cf_Close()
{
	status_t err;

	printf("Config!\n");

	cf_file->SetTo(cf_path.Path(),B_WRITE_ONLY|B_ERASE_FILE);

	err=cf_out_msg->Flatten(cf_file);



	if(err!=B_OK) printf("..errore nel mettere il msg!\n");
	delete cf_file;
}

float
FloatKey(const char *key,float def)
{
	status_t err;
	float val;

	if(cf_in_msg==NULL) 	return def;

	err=cf_in_msg->FindFloat(key,&val);




	if(err==B_OK) return val;

	return def;
}
void
PutFloatKey(const char *key,float def)
{
	status_t err;
	err=cf_out_msg->ReplaceFloat(key,def);

	if(err==B_NAME_NOT_FOUND)
		err=cf_out_msg->AddFloat(key,def);

	if(err!=B_OK) printf("can't write conf!\n");
	else printf("in conf : %s %f\n",key,def);
}
void
PutString(const char *key,const char *text)
{}
void
String(const char *key,const char* text,const char *def,int n)
{}
void
PutRef(const char *key,entry_ref *ref)
{
	status_t err;
	err=cf_out_msg->AddRef(key,ref);
	if(err!=B_OK) printf("can't write conf!\n");
	else printf("in conf : %s %s\n",key,ref->name);
}
void
ReplaceRef(const char *key,entry_ref *ref)
{
	status_t err;
	err=cf_out_msg->ReplaceRef(key,ref);

	if(err==B_NAME_NOT_FOUND)
		err=cf_out_msg->AddRef(key,ref);

	if(err!=B_OK) printf("can't repl conf!\n");
	else printf("in conf : %s %s\n",key,ref->name);
}
status_t
Ref(const char *key,entry_ref *ref,const char *def)
{
	status_t err;

	if(cf_in_msg==NULL) 	{get_ref_for_path(def,ref); return B_ERROR;}

	err=cf_in_msg->FindRef(key,ref);

	//printf("qui\n");

	if(err==B_OK) return B_OK;

	get_ref_for_path(def,ref); return B_ERROR;
}
