/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "Kinect.h"

using namespace std;

#define PAD_WIDTH 1024
#define PAD_HEIGHT 720

Kinect::Kinect(int kinectID) : myPoseUser(0){
    myKinectID=kinectID;
}

Kinect::~Kinect(){}

openni::Status Kinect::initKinect(){
    openni::Status checkResult = openni::OpenNI::initialize();
    if (checkResult != openni::STATUS_OK)
    {
        printf("Failed to initialize OpenNI\n%s\n", openni::OpenNI::getExtendedError());
        return checkResult;
    }
    
    const char* deviceUri = openni::ANY_DEVICE;
    
    checkResult = myNiDevice.open(deviceUri);
    if (checkResult != openni::STATUS_OK)
    {
        printf("Failed to open device\n%s\n", openni::OpenNI::getExtendedError());
        return checkResult;
    }
    
    nite::Status checkResult2 = nite::NiTE::initialize();
    if (checkResult != nite::STATUS_OK)
    {
        cout<<"Failed to initialize NiTE !"<<endl;
        return checkResult;
    }
    
    /*if (myUserTracker.create(&myNiDevice) != nite::STATUS_OK)
    {
        return openni::STATUS_ERROR;
    }*/
    
    myKinectDisplay.setPadX(PAD_WIDTH);
    myKinectDisplay.setPadY(PAD_HEIGHT);
    
    return checkResult;
};



nite::Status Kinect::initHandTracker(){
    nite::Status checkResult = nite::STATUS_OK;
    
    checkResult = myHandTracker.create();
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }

    checkResult = myHandTracker.startGestureDetection(nite::GESTURE_WAVE);
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }
    checkResult = myHandTracker.startGestureDetection(nite::GESTURE_HAND_RAISE);
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }
    
    checkResult = myHandTracker.readFrame(&myHandTrackerFrame);
    
    cout<<"Wave or Click to start tracking your hand..."<<endl;
    
    return checkResult;
};


nite::Status Kinect::trackHand(int &robotSelected,std::vector<std::string> cases){
    nite::Status checkResult=nite::STATUS_OK;
    int caseSelected=-1;

    checkResult = myHandTracker.readFrame(&myHandTrackerFrame);
    openni::VideoFrameRef depthFrame = myHandTrackerFrame.getDepthFrame();
    
    int resolutionX = depthFrame.getVideoMode().getResolutionX();
    int resolutionY = depthFrame.getVideoMode().getResolutionY();
    
    if (checkResult != nite::STATUS_OK)
    {
        cout<<"Get next frame failed"<<endl;
        exit(1);
    }
    
    const nite::Array<nite::GestureData>& gestures = myHandTrackerFrame.getGestures();
    //cout<<"number of gest detected : "<<gestures.getSize()<<endl;
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        cout<<"gestures got"<<endl;
        if (gestures[i].isComplete() /*& (gestures[i].getType()==nite::GESTURE_WAVE)*/)
        {
            nite::HandId newId;
            myHandTracker.startHandTracking(gestures[i].getCurrentPosition(), &newId);
            cout<<"wave detected"<<endl;

        }
        if (gestures[i].isComplete() /*& (gestures[i].getType()==nite::GESTURE_CLICK)*/)
        {
            robotSelected = caseSelected;
            cout<<"robot selected : "<<robotSelected<<endl;
        }
    }
    
    const nite::Array<nite::HandData>& hands = myHandTrackerFrame.getHands();
    for (int i = 0; i < hands.getSize(); ++i)
    {
        const nite::HandData& hand = hands[i];
        if (hand.isTracking())
        {
            float handProjectivePostionX;
            float handProjectivePostionY;
            myHandTracker.convertHandCoordinatesToDepth(hand.getPosition().x, hand.getPosition().y, hand.getPosition().z, &handProjectivePostionX, &handProjectivePostionY);
            handProjectivePostionX = (resolutionX-handProjectivePostionX-resolutionX/3)*PAD_WIDTH/(resolutionX/3) ;
            handProjectivePostionY = (handProjectivePostionY-resolutionY/6)*PAD_HEIGHT/(resolutionY/6) ;
            caseSelected = myKinectDisplay.displayPad((int)(handProjectivePostionX), (int)(handProjectivePostionY), cases);
            cout<<"X : "<<handProjectivePostionX<<endl;
            cout<<"Y : "<<handProjectivePostionY<<endl;

        }
    }
    
    return checkResult;
};

nite::Status Kinect::stopHandTracker(){
    nite::Status checkResult=nite::STATUS_OK;

    myHandTracker.destroy();
    
    return checkResult;
};

nite::Status Kinect::initSkeletonTracker(){
    nite::Status checkResult=nite::STATUS_OK;
    
    myVideoStream.create(myNiDevice, openni::SENSOR_COLOR);
    myVideoStream.start();
    
    
    return checkResult;
};

nite::Status Kinect::trackSkeleton(){
    nite::Status checkResult=nite::STATUS_OK;

        myVideoStream.readFrame(&videoFrame);
        const openni::RGB888Pixel* imageBuffer = (const openni::RGB888Pixel*)videoFrame.getData();
        
        myKinectDisplay.displayFrame(imageBuffer, videoFrame);
    
    
    return checkResult;
};








