/*******************************************************************************
*                                                                              *
*   APP RS 2016 - ApplicationKinect                                            *
*   Polytech Annecy Chambery                                                   *
*                                                                              *
*******************************************************************************/

#include "ApplicationKinect.h"

using namespace std;

ApplicationKinect::ApplicationKinect(const char* deviceName, const char* deviceIP) : Device(deviceName,deviceIP){}
ApplicationKinect::~ApplicationKinect(){}

void ApplicationKinect::connectGesCom(){}

void ApplicationKinect::disconnectGesCom(){}

void ApplicationKinect::sendMsg(){}

void ApplicationKinect::receiveMsg(){}

int ApplicationKinect::selectRobot(Kinect &myKinect, vector<string> robotList){
    int robotSelected=-1;
    nite::Status checkResult = nite::STATUS_OK;
    
    checkResult = myKinect.initHandTracker();
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }
    
    while (robotSelected==-1) {
        checkResult = myKinect.trackHand(robotSelected, robotList);
        if (checkResult!=nite::STATUS_OK) {
            cout<<"Error in : "<<checkResult<<endl;
            exit(1);
        }
    }
    
    myKinect.stopHandTracker();
    
    
    return robotSelected;
}

int ApplicationKinect::selectRobotSkeleton(Kinect &myKinect, std::vector<std::string> robotList){
    int robotSelected=-1;
    
    
    
    return robotSelected;
}


int ApplicationKinect::selectAction(){
    int actionSelected;
    
    return actionSelected;
}

void ApplicationKinect::runApp(){
    openni::Status checkResult = openni::STATUS_OK;
    Kinect kinect1(0);
    checkResult = kinect1.initKinect();
    if (checkResult != openni::STATUS_OK)
    {
        cout<<"Error : "<<checkResult<<endl;
        exit(1);
    }
    
    int robotSelected;
    std::vector<std::string> listeCas;
    listeCas.push_back("test1");
    listeCas.push_back("test2");
    listeCas.push_back("test3");
    listeCas.push_back("test4");
    listeCas.push_back("test5");
    //listeCas.push_back("test6");
    //listeCas.push_back("test7");
    robotSelected = selectRobot(kinect1, listeCas);
    cout<<"Robot selected : "<<robotSelected<<endl;
    /*kinect1.initSkeletonTracker();
    while (true) {
        kinect1.trackSkeleton();
    }*/
    
    nite::NiTE::shutdown();
    
}


int main(int argc, char** argv)
{
    ApplicationKinect appKinect("AppKinect1","127.0.0.1");
    
    /*appKinect.connectGesCom();
    
    
    
    appKinect.disconnectGesCom();*/
    
    appKinect.runApp();
    
    std::vector<std::string> listeCas;
    listeCas.push_back("test1");
    listeCas.push_back("test2");
    listeCas.push_back("test3");
    listeCas.push_back("test4");
    listeCas.push_back("test5");
    //listeCas.push_back("test6");
    //listeCas.push_back("test7");

    KinectDisplay myKinectDisplay;
    myKinectDisplay.setPadX(900);
    myKinectDisplay.setPadY(600);
    while (true) {
        myKinectDisplay.displayPad(700,250,listeCas);
        


    }
    
    
    
    //SampleViewer sampleViewer("User Viewer");
    //checkResult = sampleViewer.Init(argc, argv);
    
        //sampleViewer.Run();
}
