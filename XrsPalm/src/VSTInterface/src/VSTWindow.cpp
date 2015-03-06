#include "VSTWindow.h"
#include "MyList.h"
#include "MySource.h"
#include "ListItem.h"
#include "ScrollView.h"
#include "Configurator.h"
#include "VSTFilter.h"
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "Entry.h"
#include "Juice.h"
#include "Box.h"
#include "PlugWindow.h"
#define DRAG_X 1978

#include "TabView.h"
#include "MyItem.h"

BTabView	*tab_view;
BTab		*tab;
BPath		vstpath;

#define	WIN_POSX	300
#define	WIN_POSY	300
#define	GLOBAL_LINE	2

float hs;

VSTWindow::VSTWindow() :	PlugWindow()
{
	ResizeTo(380,135);
	
	SetTitle("VST Audio Output");
	//B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS)
	//SetType(B_FLOATING_WINDOW); //, B_NOT_RESIZABLE)
	//SetLook(B_FLOATING_WINDOW_LOOK);
	//SetFeel(B_NORMAL_WINDOW_FEEL);
	//BWindow(BRect(WIN_POSX,WIN_POSY,WIN_POSX+380,WIN_POSY+135),"VST Audio Output", B_DOCUMENT_WINDOW, |B_ASYNCHRONOUS_CONTROLS
	SetFlags(B_AVOID_FOCUS|B_WILL_ACCEPT_FIRST_CLICK|B_NOT_ZOOMABLE|B_NOT_RESIZABLE);
	succo=NULL;
	 	
	list[0]=new MyList(BRect(5,5,105,95));
	list[0]->SetInvocationMessage(new BMessage('invk'));
	list[1]=new MyList(BRect(5,5,105,95));
	list[1]->SetInvocationMessage(new BMessage('invk'));
	list[2]=new MyList(BRect(5,5,105,95));
	list[2]->SetInvocationMessage(new BMessage('invk'));
	BView* empty=new BView(Bounds(),"Pippo",B_FOLLOW_ALL,B_WILL_DRAW);
	empty->SetViewColor(216,216,216,0);
	//empty->AddChild(new BScrollView("_scroolSource",s,B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,false,true,B_FANCY_BORDER));

	BRect r = Bounds();
	r.InsetBy(5,5);
	r.right-=220;
	tab_view = new BTabView(r, "tab_view",B_WIDTH_FROM_LABEL,B_FOLLOW_NONE);
	tab_view->SetViewColor(216,216,216,0);
	r = tab_view->Bounds();
	r.InsetBy(5,5);
	r.bottom -= tab_view->TabHeight();
	tab = new BTab();
	tab_view->AddTab(new BScrollView("_scroolView2",list[0],B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,false,true,B_FANCY_BORDER), tab);
	tab->SetLabel("Fx1");
	tab = new BTab();
	tab_view->AddTab(new BScrollView("_scroolView3",list[1],B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,false,true,B_FANCY_BORDER), tab);
	tab->SetLabel("Fx2");	
	tab = new BTab();
	tab_view->AddTab(new BScrollView("_scroolView1",list[2],B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,false,true,B_FANCY_BORDER), tab);
	tab->SetLabel("Global");
	empty->AddChild(tab_view);
	//BRect(200,40,280,130)
	r=(tab_view->ContainerView())->Frame();
	r.top+=10;
	r.bottom+=5;
	r.left+=200;
	r.right+=200;
	s=new MySource(r);
	
		
	empty->AddChild(new BScrollView("_scroolSource",s,B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,false,true,B_FANCY_BORDER));

	AddChild(empty);
	

	win_vst[0]=(new BList(0));
	win_vst[1]=(new BList(0));
	win_vst[2]=(new BList(0));
	
	list[0]->AddItem(new MyItem("dump!"));
	hs=list[0]->ItemFrame(0).bottom-list[0]->ItemFrame(0).top;
	list[0]->RemoveItem((int32)0);

}
void
VSTWindow::getVstName(const char* d,int line,int pos)
{
	if(pos >= CountVst(line)) return;
	strncpy((char*)d,((BStringItem*)(list[line]->ItemAt(pos)))->Text(),100);
}
int
VSTWindow::CountVst(int i)
{
	return list[i]->CountItems();	
}

void
VSTWindow::CloseVst()
{
	for(int j=0; j<3;j++)
	{
	
		for(int i=0;i<win_vst[j]->CountItems();i++)
		{
			BWindow	*dv=(BWindow*)win_vst[j]->ItemAt(i);
			if(dv->Lock())	dv->Quit();
		}
	
		win_vst[j]->MakeEmpty();
		list[j]->MakeEmpty();
	}
	
	
}
VSTWindow::~VSTWindow()
{
	CloseVst();
		
	
}
void
VSTWindow::MessageReceived(BMessage* msg)
{
	if(msg->what==DRAG_X){
	
	int k=msg->FindInt16("from");
	
		if(k>=0)	
		{
			list[tab_view->Selection()]->RemoveItem(k);
			RemoveVst(0,k);
		}
	}
	else
	if(msg->what=='invk') 
	{
		int line=tab_view->Selection();
		BWindow* nw=(BWindow*)win_vst[line]->ItemAt(list[tab_view->Selection()]->CurrentSelection());
		if(nw->Lock())
		{
			if(nw->IsHidden())	nw->Show();
			else
			nw->Activate();
			
			nw->Unlock();
			
		}
	}
	else 
	BWindow::MessageReceived(msg);
}

void
VSTWindow::Init(Juice* sj)
{
	succo=sj;
	
	BEntry e;
	entry_ref rif;
	
	status_t err=Ref("vstfolder",&rif,"/boot/xeD/");
	
	if(err==B_OK)
	{
		printf("found vstfolder : %s\n",rif.name);
		e.SetTo(&rif);
		e.GetPath(&vstpath);
		VSTFilterPlugin::load_plug_ins (vstpath.Path(), (BListView*)s,1);
	}
	
	
	list[0]->Init(hs); list[2]->Init(hs); list[1]->Init(hs); s->Init();
	
}
bool
VSTWindow::AddVst(BMessage* msg,int line,int pos,bool g=false)
{
	//printf("VSTWindow :: AddVst name : %s, line: %d.\n",line);
	
	BPath p(vstpath);
	p.Append(msg->FindString("xrsname"));
	
	BEntry	e(p.Path());
	if(!e.Exists()){return false;	}
			
	if(g) line=GLOBAL_LINE;		// INSERIMENTO SULLA LINEA GLOBALE!
		
		
	printf("Line accepted!\n");
	
	if(Lock()){
	
		
		succo->Pause();
		
		
		VSTFilterPlugin* vst2=new  VSTFilterPlugin(p.Path());
		
		
		
		MyItem *st=new MyItem(msg->FindString("xrsname"));
		

	
		list[line]->AddItem(st,pos);
		
		 
		if(succo->vst[line]->AddItem((void*)vst2,pos))
			
			{
				vst2->SetConfig(msg);
				st->SetStatus(vst2->GetStatus());		
				succo->ResetVstTempo(vst2);
			}
		else
				{
					Unlock();
					return false;
				}
		vstWindow(vst2,msg->FindString("xrsname"),line,pos,msg);
		
		//((PlugWindow*)win_vst[line]->ItemAt(pos))->LoadPref(msg);
			
				
		succo->Pause();
		
		Unlock();
		
	}
	
	return true;
}
void
VSTWindow::AddVst(const char* n,int line,int pos)
{
	line=tab_view->Selection();
	
	if(succo==NULL || line < 0) return;
	
	
	BPath p(vstpath);
	p.Append(n);
	
	BEntry	e(p.Path());
		
	if(!e.Exists()){
			return;
	}
		
	if(Lock()){
	
		
		succo->Pause();
		VSTFilterPlugin* vst2=new  VSTFilterPlugin(p.Path());
		vstWindow(vst2,n,line,pos);
		 
		if(succo->vst[line]->AddItem((void*)vst2,pos))
			
			succo->ResetVstTempo(vst2);
		
		else
			printf("error while putting a vst in line %d\n",line);
		
		succo->Pause();
		
		Unlock();
		
	}
	
}

void
VSTWindow::SetVstState(int pos,bool s)
{
	int line=tab_view->Selection();
	
	if(line < 0 || pos<0) return;
	
	VSTFilterPlugin* vst2=(VSTFilterPlugin*)(succo->vst[line]->ItemAt(pos)); 
	vst2->SetStatus(s);
}

void
VSTWindow::RemoveVst(int line,int pos)
{
	line=tab_view->Selection();
	
	if(succo==NULL || line < 0 || pos<0) return;
		
	if(Lock()){
			
		succo->Pause();
		
		BWindow* nw=(BWindow*)(win_vst[line]->ItemAt(pos));
		win_vst[line]->RemoveItem(pos);
		if(nw->Lock())
		{
			nw->Quit();
			//delete nw;
		}
		
		VSTFilterPlugin* vst2=(VSTFilterPlugin*)(succo->vst[line]->ItemAt(pos) ); 
		succo->vst[line]->RemoveItem(pos);
		delete vst2;			
		
		succo->Pause();
		Unlock();
		
	}
	
}
void
VSTWindow::MoveVst(int from,int to)
{
	int line=tab_view->Selection();
	
	if(from==to || line < 0 ||succo==NULL) return;
	
			
	if(Lock()){
			
		succo->Pause();
		
		BWindow*	 nw=(BWindow*)(win_vst[line]->ItemAt(from));
		VSTFilterPlugin* vst2=(VSTFilterPlugin*)(succo->vst[line]->ItemAt(from));
		
		
		win_vst[line]->RemoveItem(from);
		succo->vst[line]->RemoveItem(from);
		
		win_vst[line]->AddItem((void*)nw,to);
		succo->vst[line]->AddItem((void*)vst2,to);
		
		succo->Pause();
		Unlock();
		
	}
}

void
VSTWindow::vstWindow(VSTFilterPlugin *vst,const char* name,int line,int pos,BMessage* msg=NULL)
{

	//B_ASYNCHRONOUS_CONTROLS
	PlugWindow	*nw=	new PlugWindow();

	win_vst[line]->AddItem((void*)nw,pos);
	
	nw->SetControls(vst->Configure(),0);

	if(msg!=NULL)
		nw->LoadPref(msg);

	if(line==0)
	nw->SetTitle("Fx1");
	else
	if(line==1)
	nw->SetTitle("Fx2");
	else
	nw->SetTitle("Global");
	
	nw->Show();
}
void
VSTWindow::AddVstWindowInfo(BMessage* m,int line,int pos)
{
	((PlugWindow*)win_vst[line]->ItemAt(pos))->SavePref(m);
}