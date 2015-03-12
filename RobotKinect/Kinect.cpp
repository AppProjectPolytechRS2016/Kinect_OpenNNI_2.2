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

/*Initialisation of the Kinect*/
openni::Status Kinect::initKinect(){
    
    /*Openni init*/
    openni::Status checkResult = openni::OpenNI::initialize();
    if (checkResult != openni::STATUS_OK)
    {
        printf("Failed to initialize OpenNI\n%s\n", openni::OpenNI::getExtendedError());
        return checkResult;
    }
    
    const char* deviceUri = openni::ANY_DEVICE;
    
    /*Poening openni device*/
    checkResult = myNiDevice.open(deviceUri);
    if (checkResult != openni::STATUS_OK)
    {
        printf("Failed to open device\n%s\n", openni::OpenNI::getExtendedError());
        return checkResult;
    }
    
    /*NiTE init*/
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
    
    /*Setting the size of the displayPad*/
    myKinectDisplay.setPadX(PAD_WIDTH);
    myKinectDisplay.setPadY(PAD_HEIGHT);
    
    return checkResult;
};


/*Preparing hand tracker*/
nite::Status Kinect::initHandTracker(){
    nite::Status checkResult = nite::STATUS_OK;
    
    /*Creating the hand tracker node*/
    checkResult = myHandTracker.create();
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }

    /*Starting gesture detection : the gesture allows the hand tracking*/
    myHandTracker.setSmoothingFactor(0.2);
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
    checkResult = myHandTracker.startGestureDetection(nite::GESTURE_CLICK);
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Error in : "<<checkResult<<endl;
        exit(1);
    }
    
    cout<<"Wave or Click to start tracking your hand..."<<endl;
    
    return checkResult;
};

/*Tracking hand*/
nite::Status Kinect::trackHand(int &robotSelected,std::vector<std::string> cases){
    nite::Status checkResult=nite::STATUS_OK;
    int caseSelected=-1;

    /*Loading the frame data from the hand tracker*/
    checkResult = myHandTracker.readFrame(&myHandTrackerFrame);
    if (checkResult != nite::STATUS_OK)
    {
        cout<<"Get next frame failed"<<endl;
        exit(1);
    }

    /*Loading the depthFrame data from the frame : needed to define the correspondance between hand position and the position of the selected case on the pad*/
    openni::VideoFrameRef depthFrame = myHandTrackerFrame.getDepthFrame();
    int resolutionX = depthFrame.getVideoMode().getResolutionX();
    int resolutionY = depthFrame.getVideoMode().getResolutionY();
    
    /*Looking for gesture to start tracking*/
    const nite::Array<nite::GestureData>& gestures = myHandTrackerFrame.getGestures();
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        if (gestures[i].isComplete())
        {
            nite::HandId newId;
            myHandTracker.startHandTracking(gestures[i].getCurrentPosition(), &newId);
            //cout<<"hand : "<<newId<<" is tracked !"<<endl;

        }
        /*Selecting the case*/
        if (gestures[i].isComplete() & (gestures[i].getType()==nite::GESTURE_CLICK))
        {
            robotSelected = caseSelected;
            cout<<"robot selected : "<<robotSelected<<endl;
        }
    }
    /*Tracking hand*/
    const nite::Array<nite::HandData>& hands = myHandTrackerFrame.getHands();
    for (int i = 0; i < hands.getSize(); ++i)
    {
        const nite::HandData& hand = hands[i];
        if (hand.isTracking())
        {
            float handProjectivePostionX;
            float handProjectivePostionY;
            /*Converting coordinates*/
            myHandTracker.convertHandCoordinatesToDepth(hand.getPosition().x, hand.getPosition().y, hand.getPosition().z, &handProjectivePostionX, &handProjectivePostionY);
            handProjectivePostionX = (resolutionX-handProjectivePostionX-resolutionX/3)*PAD_WIDTH/(resolutionX/3) ;
            handProjectivePostionY = (handProjectivePostionY-resolutionY/6)*PAD_HEIGHT/(resolutionY/6) ;
            /*Displating the Pad and the position of the hand on it*/
            caseSelected = myKinectDisplay.displayPad((int)(handProjectivePostionX), (int)(handProjectivePostionY), cases);
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
    openni::Status checkResult2=openni::STATUS_OK;
    cout<<checkResult2<<endl;
    myVideoStream.create(myNiDevice, openni::SENSOR_COLOR);
    checkResult2 = myVideoStream.start();
    cout<<checkResult2<<endl;
    
    
    return checkResult;
};

nite::Status Kinect::trackSkeleton(){
    nite::Status checkResult=nite::STATUS_OK;

        myVideoStream.readFrame(&videoFrame);
        const openni::RGB888Pixel* imageBuffer = (const openni::RGB888Pixel*)videoFrame.getData();
        
        myKinectDisplay.displayFrame(imageBuffer, videoFrame);
    
    
    return checkResult;
};








