//
//  main.cpp
//  RobotKinect
//
//  Created by Mikael on 23/03/2015.
//  Copyright (c) 2015 mikael. All rights reserved.
//

#include <stdio.h>
#include "ApplicationKinect.h"
#include "ComBridge.h"

int main(int argc, char** argv)
{
    Kinect kinect_1(0);
    
    ApplicationKinect appKinect("AppKinect1","169.254.136.91", &kinect_1);
    
    ComBridge comBridge("172.26.101.10",6020);
    
    appKinect.addObserver(&comBridge);
    comBridge.addObserver(&appKinect);
    
    appKinect.logInCM();
    
    appKinect.sendOrder("193.48.125.67");
    
    appKinect.logOutCM();
    
    
    
}
