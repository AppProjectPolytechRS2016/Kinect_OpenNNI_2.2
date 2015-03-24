/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__IdentToComManagerEvent__
#define __RobotKinect__IdentToComManagerEvent__

#include <stdio.h>
#include <string>
#include "Event.h"
#include "EventSource.h"

class IdentToComManagerEvent : public Event {
private:
    EventSource* source;
    std::string deviceIP;
    
public:
    IdentToComManagerEvent(EventSource* source, std::string deviceIP);
    std::string getDeviceIP();
};

#endif /* defined(__RobotKinect__IdentToComManagerEvent__) */
