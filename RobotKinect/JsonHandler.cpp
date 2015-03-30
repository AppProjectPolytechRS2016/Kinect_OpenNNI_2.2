/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "JsonHandler.h"

#define IBUFSIZE 65536

Document JsonHandler::createJsonConnectToRobot(string robot, string deviceIP){
    Document d;
    
    /*Parse an empty JSON objext to initialize the document*/
    char json[IBUFSIZE] = "{  }";
    if (d.Parse<0>(json).HasParseError())
    {
        // Oops, something went wrong
    }
    else
    {
        Value myVal;
        
        /*Adding the device IP*/
        myVal.SetString(deviceIP.c_str(), d.GetAllocator());
        d.AddMember("From", myVal, d.GetAllocator());
        
        /*Adding the robot IP*/
        myVal.SetString(robot.c_str(), d.GetAllocator());
        d.AddMember("To", myVal, d.GetAllocator());
    
        /*Adding the msg type*/
        myVal.SetString("Order", d.GetAllocator());
        d.AddMember("MsgType", myVal, d.GetAllocator());
        
        /*Adding the order*/
        myVal.SetString("ConnectTo", d.GetAllocator());
        d.AddMember("OrderName", myVal, d.GetAllocator());
    }

    return d;
}

Document JsonHandler::createJsonSendOrder(string robot, string feature, string deviceIP){
    Document d;
    
    /*Parse an empty JSON objext to initialize the document*/
    char json[IBUFSIZE] = "{  }";
    if (d.Parse<0>(json).HasParseError())
    {
        // Oops, something went wrong
    }
    else
    {
        Value myVal;
        
        /*Adding the device IP*/
        myVal.SetString(deviceIP.c_str(), d.GetAllocator());
        d.AddMember("From", myVal, d.GetAllocator());
        
        /*Adding the robot IP*/
        myVal.SetString(robot.c_str(), d.GetAllocator());
        d.AddMember("To", myVal, d.GetAllocator());
        
        /*Adding the msg type*/
        myVal.SetString("Order", d.GetAllocator());
        d.AddMember("MsgType", myVal, d.GetAllocator());
        
        /*Adding the order*/
        myVal.SetString(feature.c_str(), d.GetAllocator());
        d.AddMember("OrderName", myVal, d.GetAllocator());
    }

    return d;
}

Document JsonHandler::createJsonIdentToComManager(string deviceIP){
    Document d;
    
    /*Parse an empty JSON objext to initialize the document*/
    char json[IBUFSIZE] = "{  }";
    if (d.Parse<0>(json).HasParseError())
    {
        // Oops, something went wrong
    }
    else
    {
        Value myVal;
        
        /*Adding the device IP*/
        myVal.SetString(deviceIP.c_str(), d.GetAllocator());
        d.AddMember("From", myVal, d.GetAllocator());
        
        /*Adding the msg type*/
        myVal.SetString("Ident", d.GetAllocator());
        d.AddMember("MsgType", myVal, d.GetAllocator());
        
    }

    return d;
}

Document JsonHandler::createJsonLogOutFromComManager(string deviceIP){
    Document d;
    
    /*Parse an empty JSON objext to initialize the document*/
    char json[IBUFSIZE] = "{  }";
    if (d.Parse<0>(json).HasParseError())
    {
        // Oops, something went wrong
    }
    else
    {
        Value myVal;
        
        /*Adding the device IP*/
        myVal.SetString(deviceIP.c_str(), d.GetAllocator());
        d.AddMember("From", myVal, d.GetAllocator());
        
        /*Adding the msg type*/
        myVal.SetString("LogOut", d.GetAllocator());
        d.AddMember("MsgType", myVal, d.GetAllocator());
        
    }

    return d;
}

void JsonHandler::addComManagerIPToDocument(Document &doc, string comManagerIP){
    Value myVal;
    
    /*Adding the device IP*/
    myVal.SetString(comManagerIP.c_str(), doc.GetAllocator());
    doc.AddMember("From", myVal, doc.GetAllocator());
}

string JsonHandler::convertJsonToString(Document &doc){
    string s;
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    s = buffer.GetString();
    
    return s;
}




