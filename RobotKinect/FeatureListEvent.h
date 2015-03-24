/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__FeatureListEvent__
#define __RobotKinect__FeatureListEvent__

#include <stdio.h>
#include <string>
#include <vector>
#include "Event.h"
#include "EventSource.h"

class FeatureListEvent : public Event {
private:
    std::vector<std::string>featureList;
    EventSource* source;
    
public:
    FeatureListEvent(EventSource* source, std::vector<std::string>featureList);
    std::vector<std::string> getFeatureList();
};
#endif /* defined(__RobotKinect__FeatureListEvent__) */
