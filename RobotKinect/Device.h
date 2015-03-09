/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/


#define NAME_MAX_LENGTH 30

class Device {
public :
    Device(const char* deviceName, const char* deviceIP);
    virtual ~Device();
    
    virtual void connectGesCom()=0;
    virtual void disconnectGesCom()=0;
    virtual void sendMsg()=0;
    virtual void receiveMsg()=0;
    
protected :
    
    char        myDeviceName[NAME_MAX_LENGTH];
    char        myDeviceIP[NAME_MAX_LENGTH];
    
};

