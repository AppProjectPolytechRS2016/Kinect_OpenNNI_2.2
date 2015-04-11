/*******************************************************************************
*                                                                              *
*   APP RS 2016 - ApplicationKinect                                            *
*   Polytech Annecy Chambery                                                   *
*                                                                              *
*******************************************************************************/

#include "ApplicationKinect.h"

using namespace std;

ApplicationKinect::ApplicationKinect(const char* deviceName, const char* deviceIP, Kinect* mKinect) : Device(deviceName,deviceIP){

    myKinect = mKinect;
    
    openni::Status checkResult = openni::STATUS_OK;
    checkResult = myKinect->initKinect();
    if (checkResult != openni::STATUS_OK)
    {
        cout<<"Error : "<<checkResult<<endl;
        exit(1);
    }
    else{
        cout<<"kinect initialized !"<<endl;
    }

}

ApplicationKinect::~ApplicationKinect(){}

void ApplicationKinect::setRobotList(std::vector<std::string> robotList){
    this->robotList = robotList;
}

std::vector<std::string> ApplicationKinect::getRobotList(){
    return robotList;
}

void ApplicationKinect::logInCM(){
    jsonDocument = myJsonHandler.createJsonIdentToComManager(myDeviceIP);
    notify(jsonDocument);
}

void ApplicationKinect::logOutCM(){
    jsonDocument = myJsonHandler.createJsonLogOutFromComManager(myDeviceIP);
    notify(jsonDocument);
}

/*Test method for communication*/
void ApplicationKinect::sendOrder(string targetIP){
    jsonDocument = myJsonHandler.createJsonConnectToRobot(targetIP, myDeviceIP);
    notify(jsonDocument);
}

void ApplicationKinect::selectRobot(vector<string> robotList){
    int robot=-1;
    
    robot = selectCaseSkeleton(myKinect, robotList);
    
    if (robot==-2) {
        cout<<"Fin du programme "<<endl;
        //exit(0);
    }
    else if (robot!=-1){
        jsonDocument = myJsonHandler.createJsonConnectToRobot(robotList[robot], myDeviceIP);
        notify(jsonDocument);
    }
}

void ApplicationKinect::selectFeature(vector<std::string> featureList, string robot){
    int feature=-1;
    
    feature = selectCaseSkeleton(myKinect, featureList);
    
    if (feature==-2) {
        jsonDocument = myJsonHandler.createJsonSendOrder(robot, "Disconnect", myDeviceIP);
        notify(jsonDocument);
    }
    else if (feature!=-1){
        jsonDocument = myJsonHandler.createJsonSendOrder(robot, featureList[feature], myDeviceIP);
        notify(jsonDocument);
    }

}

int ApplicationKinect::selectCaseSkeleton(Kinect* myKinect, std::vector<std::string> caseList){
    int caseSelected=-1;
    
    nite::Status checkResult = nite::STATUS_OK;
    
    checkResult = myKinect->initSkeletonTracker();
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }
    
    while (caseSelected==-1) {
        checkResult = myKinect->trackSkeleton(caseSelected, caseList);
        if (checkResult!=nite::STATUS_OK) {
            cout<<"Error in : "<<checkResult<<endl;
            exit(1);
        }
    }
    
    myKinect->stopSkeletonTracker();

    return caseSelected;
}

void ApplicationKinect::mimeHumanArms(string robot){
    nite::Status checkResult = nite::STATUS_OK;
    vector<nite::Quaternion> jointOrientation;
    
    checkResult = myKinect->initSkeletonTracker();
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }
    
    do {
        jointOrientation = myKinect->trackSkeletonMime();
        if (jointOrientation[0].w!=0 || jointOrientation[0].x!=0 || jointOrientation[0].y!=0 || jointOrientation[0].z!=0) {
            jsonDocument = myJsonHandler.createJsonMime(robot, myDeviceIP, jointOrientation);
            notify(jsonDocument);
        }
    }
    while (!jointOrientation.empty());
    
    myKinect->stopSkeletonTracker();

}


void ApplicationKinect::update(rapidjson::Document& d){
    string msgType;
    
    msgType = myJsonHandler.extractMessageType(d);
    
    if(msgType=="ErrorReceiving"){
        cout<<"Error receiving message ! Asking for UpdateList"<<endl;
        jsonDocument = myJsonHandler.createJsonAskForUpdateList(myDeviceIP);
        notify(jsonDocument);
    }
    else if (msgType=="Order"){
        if (myJsonHandler.extractOrderName(d)=="UpdateList") {
            robotList = myJsonHandler.extractList("RobotList", d);
            if (robotList[0]=="null") {
                sleep(10.0f);
                jsonDocument = myJsonHandler.createJsonAskForUpdateList(myDeviceIP);
                notify(jsonDocument);
            }
            selectRobot(robotList);
        }
        else{
            cout<<"Unknown order !"<<endl;
        }
    }
    else if (msgType=="Ack"){
        string ackType = myJsonHandler.extractAckType(d);
        if (ackType=="FeatureList") {
            selectFeature(myJsonHandler.extractList("FeatureList", d), d["From"].GetString());
        }
        else if (ackType=="OrderAccepted"){
            if (d["OrderAccepted"].GetBool()==true) {
                cout<<"Order accepted by the robot"<<endl;
                jsonDocument = myJsonHandler.createJsonConnectToRobot(d["From"].GetString(), myDeviceIP);
                notify(jsonDocument);
            }
            else{
                cout<<"Robot denied order !"<<endl;
                jsonDocument = myJsonHandler.createJsonAskForUpdateList(myDeviceIP);
                notify(jsonDocument);
            }
        }
        else if ((ackType=="End" && d["End"].GetBool()==true) || (ackType=="Disconnected" && d["Disconnected"].GetBool()==true)){
            sleep(5.0f);
            jsonDocument = myJsonHandler.createJsonAskForUpdateList(myDeviceIP);
            notify(jsonDocument);
        }
        else if (ackType=="MimeAccepted"){
            if (d["MimeAccepted"].GetBool()==true) {
                mimeHumanArms(d["From"].GetString());
            }
            else{
                cout<<"Robot denied order !"<<endl;
                jsonDocument = myJsonHandler.createJsonAskForUpdateList(myDeviceIP);
                notify(jsonDocument);
            }
        }
    }
    else{
        cout<<"Message type unknown !"<<endl;
    }
    
}



