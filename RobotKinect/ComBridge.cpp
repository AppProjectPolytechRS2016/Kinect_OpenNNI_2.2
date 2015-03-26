/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "ComBridge.h"

ComBridge::ComBridge(std::string comManagerIP){
    this->myComManagerIP = comManagerIP;
}

void ComBridge::update(rapidjson::Document& d){
    
    /*If the message is for the ComManager*/
    if (!d.HasMember("To")) {
        rapidjson::Value myVal;
        /*Adding the comManager IP*/
        myVal.SetString(myComManagerIP.c_str(), d.GetAllocator());
        d.AddMember("To", myVal, d.GetAllocator());
    }

}

std::string ComBridge::readMessage(){
    std::string ret="ha";
    return ret;
}
void ComBridge::writeMessage(std::string msg){
    
}