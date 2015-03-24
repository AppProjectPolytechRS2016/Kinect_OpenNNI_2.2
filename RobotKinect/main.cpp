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
    ApplicationKinect appKinect("AppKinect1","127.0.0.1");
    
    ComBridge comBridge;
    
    appKinect.addObserver(&comBridge);
    comBridge.addObserver(&appKinect);
    
    /*appKinect.connectGesCom();
     
     
     
     appKinect.disconnectGesCom();*/
    
    appKinect.runApp();
    
}
