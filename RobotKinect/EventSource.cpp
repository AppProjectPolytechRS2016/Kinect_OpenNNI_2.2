/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "EventSource.h"

void EventSource::addObserver(EventObserver *o){
    listObservers.insert(o);
}

void EventSource::removeObserver(EventObserver *o){
    listObservers.erase(o);
}

void EventSource::notify(rapidjson::Document& d){
    for (std::set<EventObserver*>::const_iterator it = listObservers.begin();it != listObservers.end(); ++it){
        (*it)->update(d);
    }
}