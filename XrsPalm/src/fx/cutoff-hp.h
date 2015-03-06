// butterwo.h


#ifndef _SPKIT_BUTTERWORTH_H_
#define _SPKIT_BUTTERWORTH_H_

#include "SupportDefs.h"


class CutFilter  {
public:
    	CutFilter();
    ~CutFilter();
    
        int16 getSample(int16);
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
    	int16 x[2];
    	int16 y[2];
    };

    BWBuffer* chBuf[256];
};

#endif
