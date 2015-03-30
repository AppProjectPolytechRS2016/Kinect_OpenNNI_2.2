/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__ComBridge__
#define __RobotKinect__ComBridge__

#include <stdio.h>
#include <iostream>
#include <string>
#include "EventObserver.h"
#include "EventSource.h"
#include "JsonHandler.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

class ComBridge : public EventSource, public EventObserver {
public:
    ComBridge(std::string comManagerIP, int portNumber);
    ~ComBridge();
    
    virtual void update(rapidjson::Document& d);
    
    void readMessage();

private:
    void initComBridge();
    
    
    void writeMessage(rapidjson::Document& d);
    
    string myComManagerIP;
    
    struct sockaddr_in server_addr;
    struct hostent* server;
    int socketFD, portNum;
    string buffer;
    char bufferIn[256];
    
};

#endif /* defined(__RobotKinect__ComBridge__) */
