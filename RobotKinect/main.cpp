/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/


#include <stdio.h>
#include "ApplicationKinect.h"
#include "ComBridge.h"

int main(int argc, char** argv)
{
    Kinect kinect_1(0);
    
    ApplicationKinect appKinect("AppKinect1","169.254.21.113", &kinect_1);
    
    ComBridge comBridge("169.254.193.193",6020);
    
    appKinect.addObserver(&comBridge);
    comBridge.addObserver(&appKinect);
    
    appKinect.logInCM();
        
    appKinect.logOutCM();
    
    /*bool startCount;
    int countDown=1;
    
    kinect_1.initSkeletonTracker();
    while (true) {
        kinect_1.trackSkeletonMime(startCount, countDown);
    }*/
    
    
}
