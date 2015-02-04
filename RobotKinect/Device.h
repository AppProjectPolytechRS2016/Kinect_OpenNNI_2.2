//
//  Device.h
//  RobotKinect
//
//  Created by Mikael on 04/02/2015.
//  Copyright (c) 2015 mikael. All rights reserved.
//


class Device {
public :
    Device(const char* deviceName, const char* deviceIP);
    ~Device();
    
    virtual void connectGesCom()=0;
    virtual void disconnectGesCom()=0;
    virtual void sendOrder()=0;
    virtual void receiveMsg()=0;
    
    
private :
    
    char        myDeviceName;
    char        myDeviceIP;
    
};


