/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__SendOrderEvent__
#define __RobotKinect__SendOrderEvent__

#include <stdio.h>
#include <string>
#include "Event.h"
#include "EventSource.h"

class SendOrderEvent : public Event {
private:
    EventSource* source;
    std::string robot;
    std::string feature;
    
public:
    SendOrderEvent(EventSource* source, std::string robot);
    std::string getRobot();
    std::string getFeature();
};

#endif /* defined(__RobotKinect__SendOrderEvent__) */
