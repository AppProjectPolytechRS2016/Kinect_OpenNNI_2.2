/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__OrderAcceptedEvent__
#define __RobotKinect__OrderAcceptedEvent__

#include <stdio.h>
#include "Event.h"
#include "EventSource.h"

class OrderAcceptedEvent : public Event {
private:
    bool accepted;
    EventSource* source;
    
public:
    OrderAcceptedEvent(EventSource* source, bool accepted);
    bool getAccepted();
};

#endif /* defined(__RobotKinect__OrderAcceptedEvent__) */
