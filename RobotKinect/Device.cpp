/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include <stdio.h>
#include "Device.h"
#include "String.h"

Device::Device(const char* deviceName, const char* deviceIP){
    strncpy(myDeviceName, deviceName, NAME_MAX_LENGTH);
    strncpy(myDeviceIP, deviceIP, NAME_MAX_LENGTH);
}

Device::~Device(){}
