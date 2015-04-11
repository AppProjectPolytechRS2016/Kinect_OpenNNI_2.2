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
#include <vector>
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "NiTE.h"
#include "KinectMath.h"

using namespace rapidjson;
using namespace std;

class JsonHandler {
public:
    Document createJsonConnectToRobot(string robot, string deviceIP);
    Document createJsonSendOrder(string robot, string feature, string deviceIP);
    Document createJsonIdentToComManager(string deviceIP);
    Document createJsonLogOutFromComManager(string deviceIP);
    Document createJsonAskForUpdateList(string deviceIP);
    Document createJsonErrorRecv();
    Document createJsonMime(string robot, string deviceIP, vector<nite::Quaternion> jointOrientationVector);
    
    void addComManagerIPToDocument(Document &doc, string comManagerIP);
    
    string convertJsonToString(Document &doc);
    string extractMessageType(Document &doc);
    string extractOrderName(Document &doc);
    string extractAckType(Document &doc);
    
    vector<string> extractList(string listType,Document &doc);
};

#endif /* defined(__RobotKinect__jsonHandler__) */
