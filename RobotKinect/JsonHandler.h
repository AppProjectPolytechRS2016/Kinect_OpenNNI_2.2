/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__jsonHandler__
#define __RobotKinect__jsonHandler__

#include <stdio.h>
#include <iostream>
#include "document.h"
using namespace rapidjson;
using namespace std;

class JsonHandler {
public:
    Document createJsonConnectToRobot(string robot, string deviceIP);
    Document createJsonSendOrder(string robot, string feature, string deviceIP);
    Document createJsonIdentToComManager(string deviceIP);
    Document createJsonLogOutFromComManager(string deviceIP);
    
    void addComManagerIPToDocument(Document &doc, string comManagerIP);
    
    
};

#endif /* defined(__RobotKinect__jsonHandler__) */
