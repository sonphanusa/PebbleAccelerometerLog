/***********************************************************************
** Based on the PDOS written by Paul Edwards 
** 
** Source file to hold function implementations for the FallDetect 
** project.
***********************************************************************/

/** INCLUDES **/
#include "pebble_math.h"

	
/** FUNCTION IMPLEMENTATIONS **/
	
/* floating point absolute value function */
double fabs(double x)
{
    if (x < 0.0)
    {
        x = -x;
    }
    return (x);
}

/* sqrt function */
double sqrt(double x)
{
    double xs,yn,ynn;
    double pow1;
    int i;

    if (x < 0.0)
    {
        return(0.0);
    }
    if (x == 0.0) return (0.0);

/*

  Scale argument 1 <= x <= 4

*/

    xs=x;
    pow1=1;

    while(xs<1.0){xs=xs*4.0;pow1=pow1/2.0;}
    while(xs>=4.0){xs=xs/4.0;pow1=pow1*2.0;}

/*
  calculate using Newton raphson
  use x0 = x/2.0
*/

    i=0;
    yn = xs/2.0;
    ynn = 0;
    while(1)
    {
        ynn = (yn + xs/yn)*0.5;
        if ( fabs(ynn-yn) <= 10.0 * DBL_MIN ) break; else yn=ynn;
        if ( i > 10  ) break; else i++ ;
    }
    return (ynn*pow1);
}


float clamp(float a, float b, float c)
{
    return max(a, min(c, b));
}

float max(float a,float b)
{
     if (a > b)
	 {
	     return a;
	 }
	
	return b;
}

float min(float a,float b)
{
     if (a < b)
	 {
	     return a;
	 }
	
	return b;
}

float magnitude(float a, float b, float c)
{
	return sqrt(a*a + b*b + c*c);
}

float norm(float a, float b, float c)
{
   return a / magnitude(a, b, c);
}