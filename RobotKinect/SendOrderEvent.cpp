/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "SendOrderEvent.h"

SendOrderEvent::SendOrderEvent(EventSource* source, std::string robot){
    this->source=source;
    this->robot=robot;
    this->feature=feature;
}

std::string SendOrderEvent::getRobot(){
    return robot;
}

std::string SendOrderEvent::getFeature(){
    return feature;
}