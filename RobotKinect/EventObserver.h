/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __EventObserver__
#define __EventObserver__

#include <stdio.h>
#include "Event.h"
#include "document.h"

class EventObserver{
public:
    virtual void update(rapidjson::Document& d)=0;
};

#endif /* defined(__RobotKinect__EventObserver__) */
