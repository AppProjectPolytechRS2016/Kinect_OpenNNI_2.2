/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "IdentToComManagerEvent.h"

IdentToComManagerEvent::IdentToComManagerEvent(EventSource* source, std::string deviceIP){
    this->source=source;
    this->deviceIP=deviceIP;
}

std::string IdentToComManagerEvent::getDeviceIP(){
    return deviceIP;
}