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
#include "document.h"
#include "JsonHandler.h"
#include <set>


class EventSource{
public:
    void notify(rapidjson::Document& d);
    void addObserver(EventObserver* o);
    void removeObserver(EventObserver* o);
    
protected:
    std::set<EventObserver*>listObservers;
    rapidjson::Document jsonDocument;
    JsonHandler myJsonHandler;
};

#endif