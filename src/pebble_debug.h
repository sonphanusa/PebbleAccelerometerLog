#ifndef PEBBLE_DEBUG_H 
#define PEBBLE_DEBUG_H

	
/*******************************************************************************
** Header function to hold global debug variables, defines/macros, structures,
** and function declarations for accelerometer access and usages.
** 
** Author: Jonathan Cardinale Date: 2014-01-27
*******************************************************************************/
	
/** Can be used to specifiy different debug prints in code **/
#define DEBUG_LEVEL 0 // debug output off

/** Helper macro for the helper macro **/
#define DEBUG(fmt, args...)\
	APP_LOG(APP_LOG_LEVEL_DEBUG, fmt, ##args )

/** Helper macro for the helper macro **/
#define ERROR(fmt, args...)\
	APP_LOG(APP_LOG_LEVEL_ERROR, fmt, ##args )

	
#endif /* pebble_debug.h */