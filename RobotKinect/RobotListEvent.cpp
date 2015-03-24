/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "RobotListEvent.h"

RobotListEvent::RobotListEvent(EventSource* source, std::vector<std::string>robotList){
    this->source = source;
    this->robotList = robotList;
}

std::vector<std::string> RobotListEvent::getRobotList(){
    return robotList;
}