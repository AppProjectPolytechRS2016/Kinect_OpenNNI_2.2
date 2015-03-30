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

void ApplicationKinect::receiveMsg(){}

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
    
    
}

void ApplicationKinect::runApp(){
    openni::Status checkResult = openni::STATUS_OK;
    /*Kinect kinect1(0);
    checkResult = kinect1.initKinect();
    if (checkResult != openni::STATUS_OK)
    {
        cout<<"Error : "<<checkResult<<endl;
        exit(1);
    }*/
    
    int caseSelected;
    std::vector<std::string> listeCas;
    listeCas.push_back("test1");
    listeCas.push_back("test2");
    listeCas.push_back("test3");
    listeCas.push_back("test4");
    listeCas.push_back("test5");
    listeCas.push_back("test6");
    listeCas.push_back("test7");
    
    //robotSelected = selectRobot(kinect1, listeCas);
    
    caseSelected = selectCaseSkeleton(myKinect, listeCas);
    cout<<"Robot selected : "<<caseSelected<<endl;
    
    /*kinect1.initSkeletonTracker();
    while (true) {
        kinect1.trackSkeleton();
    }*/
    
    nite::NiTE::shutdown();
    
    
    
}

