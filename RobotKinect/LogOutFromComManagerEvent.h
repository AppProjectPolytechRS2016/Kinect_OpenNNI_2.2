/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__LogOutFromComManagerEvent__
#define __RobotKinect__LogOutFromComManagerEvent__

#include <stdio.h>
#include "Event.h"
#include "EventSource.h"

class LogOutFromComManagerEvent : public Event {
private:
    EventSource* source;
public:
    LogOutFromComManagerEvent(EventSource* source);
};

#endif /* defined(__RobotKinect__LogOutFromComManagerEvent__) */
