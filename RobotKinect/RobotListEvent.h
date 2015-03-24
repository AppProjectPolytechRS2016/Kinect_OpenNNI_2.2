/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__RobotListEvent__
#define __RobotKinect__RobotListEvent__

#include <stdio.h>
#include <string>
#include <vector>
#include "Event.h"
#include "EventSource.h"

class RobotListEvent : public Event
{

private:
    std::vector<std::string>robotList;
    EventSource* source;

public:
    RobotListEvent(EventSource* source, std::vector<std::string>robotList);
    std::vector<std::string> getRobotList();
    
};

#endif /* defined(__RobotKinect__RobotListEvent__) */
