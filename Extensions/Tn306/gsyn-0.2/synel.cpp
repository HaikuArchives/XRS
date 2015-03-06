
#include <stdlib.h>
#include <string.h>
#include "synel.h"
#include"stdio.h"

#ifndef NO_NAMESPACES
#include <algorithm>
using namespace std;
#else
#include <algo.h>
#endif

//////////////////////////////////////////////////////////
// Sample buffer
SampleBuf::~SampleBuf()
{
	if(buf) free(buf);
}

float *SampleBuf::GetBuf(int size)
{
	if(size>alloclen) {
		alloclen=size;
		buf=(float*)realloc(buf, alloclen*sizeof(float));
		Reset();
	}
	
	return buf;
}

void	SampleBuf::Reset()
{
	if(buf)
	{
		memset(buf, 0,alloclen*sizeof(float));
	}
}

//////////////////////////////////////////////////////////
// Synthesizer Element
Element::~Element()
{
	list<Element*>::iterator i;
	for(i=inputs.begin();i!=inputs.end();i++)
		(*i)->RemoveOutput();
	if(output) output->RemoveInput(this);
}

void Element::SetOutput(Element *out)
{
	if(output) output->RemoveInput(this);
	output=out; 
	output->AddInput(this);
}

void Element::AddInput(Element *in)
{
	inputs.push_back(in);
}

void Element::RemoveInput(Element *in)
{
	list<Element*>::iterator i;
	i=find(inputs.begin(), inputs.end(), in);
	if(i==inputs.end()) return;
	inputs.erase(i);
}

void Element::RemoveOutput(void)
{
	output = NULL;
}

void Element::Process(const float*, int, int)
{}

void Element::Flush(void)
{}

void Element::Message(SynthMessage type, float data)
{
	if(output) output->Message(type, data);
}

//////////////////////////////////////////////////////////
// Mixer Element
Mixer::~Mixer() {}


void Mixer::Reset()
{
	mixbuf.Reset();
}

void Mixer::Process(const float *buf, int len, int channels)
{
// This isn't bulletproof, but it should work as long as all channels are
// passing in the same buffer lengths and as long as all inputs are sending
// data.

// GetBuf always returns the same buffer; it resizes it larger if necessary.
	float *outbuf = mixbuf.GetBuf(index+len*channels);
	if(!inputcycle) 
		memcpy((void*)(outbuf+index), (void*)buf, len*channels*sizeof(float));
	else {
		int i;
		for(i=0;i<len*channels;i++) outbuf[index+i]+=buf[i];
	}
	index+=len*channels;
}

void Mixer::CycleInput(void)
{
	inputcycle++;
	if(inputcycle>=inputs.size()) {
		output->Process(mixbuf.GetBuf(), index, 1);
		inputcycle=0;
	}
	index=0;
}

