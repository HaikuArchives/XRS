
#include "SamplerTrackBoost.h"	// *change*



#define main main_plugin
extern "C" __declspec(dllexport) TrackBoost *main_plugin ();


TrackBoost *main ()
{
	return (TrackBoost*)(new SamplerTrackBoost);
}
