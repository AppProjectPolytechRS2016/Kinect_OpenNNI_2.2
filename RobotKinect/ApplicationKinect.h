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
#include <unistd.h>
#include "EventObserver.h"
#include "EventSource.h"
#include "JsonHandler.h"

class ApplicationKinect : public Device, public EventObserver, public EventSource
{
public:
    ApplicationKinect(const char* deviceName, const char* deviceIP, Kinect* mKinect);
    virtual ~ApplicationKinect();
    virtual void logInCM();
    virtual void logOutCM();
    virtual void sendOrder(string targetIP);
    
    virtual void update(rapidjson::Document& d);
    
    void selectRobot(std::vector<std::string> robotList);
    void selectFeature(std::vector<std::string> featureList, string robot);
    int selectCaseSkeleton(Kinect* myKinect, std::vector<std::string> caseList,const std::string what);
    void mimeHumanArms(string robot);
    
    std::vector<std::string> getRobotList();
    void setRobotList(std::vector<std::string> robotList);
    
private:
    Kinect* myKinect;
    std::vector<std::string> robotList; /*IP address of the robots*/
    int64 startMimeTime;
};
