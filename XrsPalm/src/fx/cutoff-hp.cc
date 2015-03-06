// butterwo.cc
/*
    Sound Processing Kit - A C++ Class Library for Audio Signal Processing
    Copyright (C) 1995-1998 Kai Lassfolk

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "cutoff-hp.h"
#include <math.h>

CutFilter::CutFilter()
{
    channelIndex = 0;

    for (int i = 0; i <2; i++) {
    	chBuf[i] = new BWBuffer;
    	chBuf[i]->x[0] = chBuf[i]->x[1] = 0.0;
	chBuf[i]->y[0] = chBuf[i]->y[1] = 0.0;
    }


}

int16 CutFilter::getSample(int16 inputSample)
{
    int16 outputSample;

//    if (!input->getSample(inputSample))
 //   	return 0;

    BWBuffer* buf = chBuf[channelIndex++];

    outputSample = a[0] * inputSample + a[1] * buf->x[0] + a[2] * buf->x[1]
    		   - b[0] * buf->y[0] - b[1] * buf->y[1];

    buf->x[1] = buf->x[0];
    buf->x[0] = inputSample;
    buf->y[1] = buf->y[0];
    buf->y[0] = outputSample;

    channelIndex %=2;

    return outputSample;
}

CutFilter::~CutFilter()
{
    for (int i = 0; i <2; i++)
    	delete chBuf[i];
}

void CutFilter::setCutOffFreq(float f)
{
    cutOffFreq = f;
    
    C = tan(M_PI * cutOffFreq / 44100);

    a[0] = 1.0 / (1.0 + sqrt(2.0) * C + C * C);
    a[1] = -2.0 * a[0];
    a[2] = a[0];

    b[0] = 2 * (C * C - 1.0) * a[0];
    b[1] = (1.0 - sqrt(2.0) * C + C * C) * a[0];
}

