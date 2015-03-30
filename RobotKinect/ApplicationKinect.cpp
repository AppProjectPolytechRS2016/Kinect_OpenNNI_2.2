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
        selectRobot(robotList);
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

void ApplicationKinect::update(rapidjson::Document& d){
    string msgType;
    
    msgType = myJsonHandler.extractMessageType(d);
    
    if(msgType=="error"){
        cout<<"Error getting message type !"<<endl;
    }
    else if (msgType=="Order"){
        if (myJsonHandler.extractOrderName(d)=="UpdateList") {
            robotList = myJsonHandler.extractList("RobotList", d);
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
            }
            else{
                cout<<"Robot denied order !"<<endl;
            }
            selectRobot(robotList);
        }
    }
    
}



