#ifndef RTK_ROVER_CONFIG_H
#ifndef MANAGER_CONFIG_H
#define MANAGER_CONFIG_H
#include <Arduino.h>

/* 
THIS WILL BE REPLACED BY THE RTKRoverConfig.h after including this project 
as submodule into the RTKRover project under lib/
*/

/******************************************************************************/
//                       Default Serial settings
/******************************************************************************/
//set to true for debug output, false for no debug output
#define DEBUGGING true 
#define DBG \
  if (DEBUGGING) Serial

#define BAUD                          115200

#endif  /*** MANAGER_CONFIG_H ***/
#endif