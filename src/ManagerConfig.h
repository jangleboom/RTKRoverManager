#ifndef RTK_ROVER_CONFIG_H
#ifndef MANAGER_CONFIG_H
#define MANAGER_CONFIG_H
#include <Arduino.h>

// THIS WILL BE REPLACED BY THE RTKRoverConfig.h after merge

/******************************************************************************/
//                       Default Serial settings
/******************************************************************************/
//set to true for debug output, false for no debug output
#define DEBUGGING true 
#define DEBUG_SERIAL \
  if (DEBUGGING) Serial

#define BAUD                          115200

/**
 * @brief Get the Chip Id (part of the MAC address)
 * 
 * @return uint32_t Chip Id
 */
uint32_t getChipId(void);
/**
 * @brief Get the unique Device Name 
 * 
 * @param prefix Device name e. g. rtkrover
 * @return String Name + ID
 */
String getDeviceName(const String &prefix);

#endif  /*** MANAGER_CONFIG_H ***/
#endif