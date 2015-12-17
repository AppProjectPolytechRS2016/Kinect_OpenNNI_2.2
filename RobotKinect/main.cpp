/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/


#include <stdio.h>
#include "ApplicationKinect.h"
#include "Kinect.h"
#include "KinectDisplay.h"
#include "ComBridge.h"

#define PAD_WIDTH 800
#define PAD_HEIGHT 720


int main(int argc, char** argv)
{
    
    std::vector<jointPositions> jointsPositions;
    std::vector<jointPositions> jointsPositionsDisplay = vector<jointPositions>();
    int resolutionX, resolutionY;
    bool exit,  aUserTracked;
    
    KinectDisplay mKinectDisplay;
    
    /*Setting the size of the displayPad*/
    mKinectDisplay.setPadX(PAD_WIDTH);
    mKinectDisplay.setPadY(PAD_HEIGHT);
    
    mKinectDisplay.initWindows();
    
    Kinect kinect_1(0,jointsPositions, jointsPositionsDisplay, resolutionX, resolutionY, exit, aUserTracked);
    
    kinect_1.start();
    
    /*ApplicationKinect appKinect("AppKinect1","169.254.100.170", &kinect_1,jointsPositions, jointsPositionsDisplay, resolutionX, resolutionY, exit, aUserTracked,mKinectDisplay);
    
    ComBridge comBridge("169.254.193.193",6020);
    
    appKinect.addObserver(&comBridge);
    comBridge.addObserver(&appKinect);
    
    appKinect.logInCM();
        
    appKinect.logOutCM();*/
    
    kinect_1.join();
    
}
