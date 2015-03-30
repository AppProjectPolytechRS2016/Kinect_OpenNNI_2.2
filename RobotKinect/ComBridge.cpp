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
        exit(1);
    }
}

void ComBridge::update(rapidjson::Document& d){
    
    string msgType = myJsonHandler.extractMessageType(d);
    /*If the message is for the ComManager*/
    if (msgType=="Ident") {
        myJsonHandler.addComManagerIPToDocument(d, myComManagerIP);
        
        /*Sending message*/
        writeMessage(d);
    }
    else{
    /*Sending message*/
    writeMessage(d);
    
    /*Waiting for answer*/
    readMessage();
    }

}

void ComBridge::readMessage(){
    bzero(bufferIn,256);
    
    /*Waiting for data from socket*/
    ssize_t numOfCharRecv = recv(socketFD, &bufferIn, 255,0);
    
    if (numOfCharRecv < 0)
        cout<<"ERROR reading socket"<<endl;
    else{
        /*Storing the string received in a jsonDocument*/
        jsonDocument.Parse(bufferIn);
        
        cout<<"message : "<<bufferIn<<endl;
        
        /*Notifying observers of event*/
        notify(jsonDocument);
    }
    
}

void ComBridge::writeMessage(rapidjson::Document& d){
    buffer = myJsonHandler.convertJsonToString(d);
    /*Adding character for the readline method of comManager*/
    buffer+="\n\r";
    
    cout<<buffer<<endl;

    /*Sending data*/
    ssize_t writeRes = send(socketFD,buffer.c_str(),strlen(buffer.c_str()),0);
    if (writeRes < 0)
        cout<<"ERROR writing to socket"<<endl;
    else cout<<"Message sent ! "<<writeRes<<endl;
}





