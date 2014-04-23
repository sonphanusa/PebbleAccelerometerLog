#ifndef __PEBBLE_MATH_H__
#define __PEBBLE_MATH_H__

/***********************************************************************
** Based on the PDOS written by Paul Edwards 
** 
** Header function to hold global math realted variables and functions
***********************************************************************/

/** DEFINES **/
#define DBL_MIN 1E-37

	
/** FUNCTION DECLARATIONS **/
double fabs(double x);
double sqrt(double x);


float clamp(float a, float b, float c);
float max(float a,float b);
float min(float a,float b);
float norm(float a, float b, float c);

#endif /* pebble_math.h */