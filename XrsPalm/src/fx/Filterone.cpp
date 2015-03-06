#include "Filterone.h"
#include "math.h"

const double        scale = 100;
       // 0 <  Smoothness < 1. High is better, but may cause precision problems
   //    const double        smoothness = 0.999;
       const double        smoothness = .5;
        /* Precalc variables */
        double                a = 1.0-(2.4/scale); // Could also be set directly
        double                b = smoothness;      //         -"-
        double                acoef = a;
        double                bcoef = a*b;
        double                ccoef = a*b*b;
        double                mastergain = 1.0 / (-1.0/(log(a)+2.0*log(b))+2.0/(log(a)+log(b))-1.0/log(a));
        double                again = mastergain;
        double                bgain = mastergain * (log(a*b*b)*(log(a)-log(a*b)) /((log(a*b*b)-log(a*b))*log(a*b)) - log(a)/log(a*b));
        double                cgain = mastergain * (-(log(a)-log(a*b)) / (log(a*b*b)-log(a*b)));
        /* Runtime variables */
        double                areg = 0;
        double                breg = 0;
        double                creg = 0;
        
FilterOne::FilterOne()
{

	
}



float FilterOne::Work(float slow)
{
	areg = acoef * areg +  slow;
                breg = bcoef * breg + slow;
                creg = ccoef * creg +  slow;

             /* Combine filters in parallel */
	float  temp =   again * areg + bgain * breg+ cgain * creg;
            return temp;
}

