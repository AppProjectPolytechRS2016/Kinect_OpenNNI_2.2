/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include <string>
#define NAME_MAX_LENGTH 30

class Device {
public :
    Device(const char* deviceName, const char* deviceIP);
    virtual ~Device();
    
    virtual void logInCM()=0;
    virtual void logOutCM()=0;
    virtual void sendOrder(std::string targetIP)=0;
    virtual void receiveMsg()=0;
    
protected :
    
    char        myDeviceName[NAME_MAX_LENGTH];
    char        myDeviceIP[NAME_MAX_LENGTH];
    
};


