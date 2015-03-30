/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "ComBridge.h"

ComBridge::ComBridge(std::string comManagerIP, int portNumber){
    this->myComManagerIP = comManagerIP;
    this->portNum = portNumber;
    
    initComBridge();
}

ComBridge::~ComBridge(){
    close(socketFD);
}

void ComBridge::initComBridge(){
    /*Opening the socket*/
    socketFD = socket(AF_INET, SOCK_STREAM, 0); //PF_INET ???
    if (socketFD<0) {
        cout<<"Error opening socket !"<<endl;
        exit(1);
    }
    
    /*Defining the server address*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    /*bcopy(myComManagerIP.c_str(),
          (char *)&server_addr.sin_addr.s_addr,
          myComManagerIP.size());*/
    inet_pton(AF_INET,myComManagerIP.c_str(),&server_addr.sin_addr);
    server_addr.sin_port = htons(portNum);
    
    cout<<"ip server store : "<<myComManagerIP.c_str()<<endl;
        
    /*Connecting to ComManager*/
    int rcon = connect(socketFD, (struct sockaddr*)&server_addr, sizeof(server_addr));
    cout<<"back num for connection : "<<rcon<<endl;
    if (rcon<0) {
        cout<<"Failed connection to ComManager !"<<endl;
    }
}

void ComBridge::update(rapidjson::Document& d){
    
    /*If the message is for the ComManager*/
    if (!d.HasMember("To")) {
        rapidjson::Value myVal;
        /*Adding the comManager IP*/
        myVal.SetString(myComManagerIP.c_str(), d.GetAllocator());
        d.AddMember("To", myVal, d.GetAllocator());
    }
    
    /*Sending message*/
    writeMessage(d);
    
    /*Waiting for answer*/
    readMessage();

}

void ComBridge::readMessage(){
    bzero(bufferIn,256);
    
    /*Waiting for data from socket*/
    ssize_t numOfCharRecv = recv(socketFD, &bufferIn, 255,0);
    //ssize_t numOfCharRecv = read(socketFD,&bufferIn,255);
    
    if (numOfCharRecv < 0)
        cout<<"ERROR reading socket"<<endl;
    
    /*Storing the string received in a jsonDocument*/
    jsonDocument.Parse(bufferIn);
    cout<<"message : "<<bufferIn<<endl;
    /*Notifying observers of event*/
    //notify(jsonDocument);
}

void ComBridge::writeMessage(rapidjson::Document& d){
    buffer = myJsonHandler.convertJsonToString(d);
    buffer+="\n\r";
    cout<<buffer<<endl;

    ssize_t writeRes = send(socketFD,buffer.c_str(),strlen(buffer.c_str()),0);
    if (writeRes < 0)
        cout<<"ERROR writing to socket"<<endl;
    else cout<<"Message sent ! "<<writeRes<<endl;
}





