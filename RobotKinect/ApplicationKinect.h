/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "Viewer.h"
#include "Kinect.h"
#include "Device.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include "EventObserver.h"
#include "EventSource.h"
#include "JsonHandler.h"
/*#include "ConnectToRobotEvent.h"
#include "SendOrderEvent.h"
#include "IdentToComManagerEvent.h"
#include "LogOutFromComManagerEvent.h"*/

class ApplicationKinect : public Device, public EventObserver, public EventSource
{
public:
    ApplicationKinect(const char* deviceName, const char* deviceIP, Kinect* mKinect);
    virtual ~ApplicationKinect();
    virtual void logInCM();
    virtual void logOutCM();
    virtual void sendOrder();
    virtual void receiveMsg();
    
    virtual void update(rapidjson::Document& d);
    
    void runApp();
    void selectRobot(std::vector<std::string> robotList);
    void selectFeature(std::vector<std::string> featureList);
    int selectCaseSkeleton(Kinect* myKinect, std::vector<std::string> caseList);
    
    std::vector<std::string> getRobotList();
    void setRobotList(std::vector<std::string> robotList);
    
private:
    Kinect* myKinect;
    std::vector<std::string> robotList; /*IP address of the robots*/
    JsonHandler myJsonHandler;
    
};
