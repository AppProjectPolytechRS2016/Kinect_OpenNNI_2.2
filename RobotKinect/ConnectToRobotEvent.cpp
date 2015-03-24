/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "ConnectToRobotEvent.h"

ConnectToRobotEvent::ConnectToRobotEvent(EventSource* source, std::string robot){
    this->source = source;
    this->robot = robot;
}

std::string ConnectToRobotEvent::getRobot(){
    return robot;
}