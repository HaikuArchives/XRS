// butterwo.h


#ifndef _SPKIT_BUTTERWORTH_H_
#define _SPKIT_BUTTERWORTH_H_

#include "SupportDefs.h"


class CutFilter  {
public:
    	CutFilter();
    ~CutFilter();
    
        float getSample(float);
	void setCutOffFreq(float);

protected:
   float cutOffFreq;

    // Temporary variables needed by derived classes:
   float C;
   float D;
    
    int channelIndex;

   float a[3];
   float b[2];

    struct BWBuffer {
    	float x[2];
    	float y[2];
    };

    BWBuffer* chBuf[256];
};

#endif
