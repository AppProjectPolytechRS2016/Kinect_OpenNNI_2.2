/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__ComBridge__
#define __RobotKinect__ComBridge__

#include <stdio.h>
#include <string>
#include "EventObserver.h"
#include "EventSource.h"

class ComBridge : public EventSource, public EventObserver {
public:
    virtual void update(Event* e);
    
    std::string readMessage();
    void writeMessage(std::string msg);

    
};

#endif /* defined(__RobotKinect__ComBridge__) */
