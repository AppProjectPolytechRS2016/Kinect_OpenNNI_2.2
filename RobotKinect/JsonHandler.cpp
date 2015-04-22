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

Document JsonHandler::createJsonAskForUpdateList(string deviceIP){
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
        myVal.SetString("UpdateList", d.GetAllocator());
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
        myVal.SetString("Logout", d.GetAllocator());
        d.AddMember("MsgType", myVal, d.GetAllocator());
        
    }

    return d;
}

Document JsonHandler::createJsonMime(string robot, string deviceIP, vector<nite::Quaternion> jointOrientationVector){
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
        
        /*Adding the target*/
        myVal.SetString(robot.c_str(), d.GetAllocator());
        d.AddMember("To", myVal, d.GetAllocator());
        
        /*Adding the msg type*/
        myVal.SetString("Order", d.GetAllocator());
        d.AddMember("MsgType", myVal, d.GetAllocator());
        
        /*Adding the order*/
        myVal.SetString("Mime", d.GetAllocator());
        d.AddMember("OrderName", myVal, d.GetAllocator());

        /*Adding the joints orientation*/
        std::vector<float> jointsOrientation;
        KMath::rotation3DFromQuaternion(jointOrientationVector, jointsOrientation);
        cout<<"taille du vecteur orientation : "<<jointsOrientation.size()<<endl;
        Value jOVal(kArrayType);
        for (int i = 0; i<jointsOrientation.size(); i++) {
            jOVal.PushBack(jointsOrientation[i], d.GetAllocator());
        }
        cout<<"taille tableau orientation json : "<<jOVal.Size()<<endl;
        d.AddMember("JointOrientation", jOVal, d.GetAllocator());
        
    }
    
    return d;

}

void JsonHandler::addComManagerIPToDocument(Document &doc, string comManagerIP){
    Value myVal;
    
    /*Adding the device IP*/
    myVal.SetString(comManagerIP.c_str(), doc.GetAllocator());
    doc.AddMember("To", myVal, doc.GetAllocator());
}

string JsonHandler::convertJsonToString(Document &doc){
    string s;
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    s = buffer.GetString();
    
    return s;
}

string JsonHandler::extractMessageType(Document &doc){
    string s;
    
    if(doc.HasMember("MsgType")){
        s = doc["MsgType"].GetString();
    }
    else{
        s = "error";
    }
    
    return s;
}

string JsonHandler::extractOrderName(Document &doc){
    string s;
    
    if(doc.HasMember("OrderName")){
        s = doc["OrderName"].GetString();
    }
    else{
        s = "error";
    }
    
    return s;
    
}

string JsonHandler::extractAckType(Document &doc){
    string s;
    
    if(doc.HasMember("Received")){
        s = "Received";
    }
    else if (doc.HasMember("FeatureList")){
        s = "FeatureList";
    }
    else if (doc.HasMember("OrderAccepted") && !doc.HasMember("End") && !doc.HasMember("Disconnected") && !doc.HasMember("MimeAccepted")){
        s = "OrderAccepted";
    }
    else if (doc.HasMember("Disconnected")){
        s = "Disconnected";
    }
    else if (doc.HasMember("End")){
        s = "End";
    }
    else if (doc.HasMember("MimeAccepted")){
        s = "MimeAccepted";
    }
    else{
        s = "error";
    }
    
    return s;
    
}

vector<string> JsonHandler::extractList(string listType,Document &doc){
    vector<string> list;
    
    if (doc.HasMember(listType.c_str())) {
        const Value& val = doc[listType.c_str()];
        if(val.IsArray()){
            for (SizeType i = 0; i < val.Size(); i++) // Uses SizeType instead of size_t
                list.push_back(val[i].GetString());
        }
        else{
            list.push_back(val.GetString());
        }
    }
    
    return list;
}

Document JsonHandler::createJsonErrorRecv(){
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
        
        /*Adding the msg type*/
        myVal.SetString("ErrorReceiving", d.GetAllocator());
        d.AddMember("MsgType", myVal, d.GetAllocator());
        
    }
    
    return d;
}

