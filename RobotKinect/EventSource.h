/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __EventSource__
#define __EventSource__

#include <stdio.h>
#include "Event.h"
#include "EventObserver.h"
#include <set>


class EventSource{
public:
    void notify(Event* e);
    void addObserver(EventObserver* o);
    void removeObserver(EventObserver* o);
    
protected:
    std::set<EventObserver*>listObservers;
    
};

#endif