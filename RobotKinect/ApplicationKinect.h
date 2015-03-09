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

class ApplicationKinect : public Device
{
public:
    ApplicationKinect(const char* deviceName, const char* deviceIP);
    virtual ~ApplicationKinect();
    virtual void connectGesCom();
    virtual void disconnectGesCom();
    virtual void sendMsg();
    virtual void receiveMsg();
    void runApp();
    int selectRobot(Kinect &myKinect, std::vector<std::string> robotList);
    int selectAction();
    
};
