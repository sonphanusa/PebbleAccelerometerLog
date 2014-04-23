#include <pebble.h>

#include "pebble_math.h"
//#include "accel.h"

static const int sampling_rate = 25;
	
static float last[3];
static float filter[3];

int16_t Kalman_Filter(float X, float Y, float Z) {
    // high pass filter
    float updateFreq = 1.0/sampling_rate; // match this to your update speed
    float cutOffFreq = 1.0;
    float RC = 1.0 / cutOffFreq;
    float dt = 1.0 / updateFreq;
    float filterConstant = RC / (dt + RC);
    float alpha = filterConstant; 
    float kAccelerometerMinStep = 0.033;
    float kAccelerometerNoiseAttenuation = 3.0;

  
    float d = clamp(fabs(norm(filter[0], filter[1], filter[2]) - norm(X, Y, Z)) / kAccelerometerMinStep - 1.0, 0.0, 1.0);
    alpha = d * filterConstant / kAccelerometerNoiseAttenuation + (1.0 - d) * filterConstant;
  
    filter[0] = (float) (alpha * (filter[0] + X - last[0]));
    filter[1] = (float) (alpha * (filter[1] + Y - last[1]));
    filter[2] = (float) (alpha * (filter[2] + Z - last[2]));

    last[0] = X;
    last[1] = Y;
    last[2] = Z;
		
	float mag = sqrt((filter[0]*filter[0])+ (filter[1]*filter[1])+  (filter[2]*filter[2]));
    return (mag*1000000);
}