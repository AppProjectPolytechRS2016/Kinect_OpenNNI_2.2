/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__ConnectToRobotEvent__
#define __RobotKinect__ConnectToRobotEvent__

#include <stdio.h>
#include <string>
#include "Event.h"
#include "EventSource.h"

class ConnectToRobotEvent : public Event {
private:
    EventSource* source;
    std::string robot;
    
public:
    ConnectToRobotEvent(EventSource* source, std::string robot);
    std::string getRobot();
};

#endif /* defined(__RobotKinect__ConnectToRobotEvent__) */
