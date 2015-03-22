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
#define AREA_X 3 //Scale of the selection area for the hand
#define AREA_Y 6

Kinect::Kinect(int kinectID) : myPoseUser(0){
    myKinectID=kinectID;
    myUserTracker = new nite::UserTracker;
    aUserIsTracked = false;
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
    if (checkResult2 != nite::STATUS_OK)
    {
        cout<<"Failed to initialize NiTE !"<<endl;
        exit(1);
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
        cout<<"creating HandTracker failed"<<endl;
        exit(1);
    }

    /*Starting gesture detection : the gesture allows the hand tracking*/
    myHandTracker.setSmoothingFactor(0.2);
    checkResult = myHandTracker.startGestureDetection(nite::GESTURE_WAVE);
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Starting gesture detection failed"<<endl;
        exit(1);
    }
    checkResult = myHandTracker.startGestureDetection(nite::GESTURE_HAND_RAISE);
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Starting gesture detection failed"<<endl;
        exit(1);
    }
    checkResult = myHandTracker.startGestureDetection(nite::GESTURE_CLICK);
    if (checkResult!=nite::STATUS_OK) {
        cout<<"Starting gesture detection failed"<<endl;
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
            //caseSelected = myKinectDisplay.displayPad((int)(handProjectivePostionX), (int)(handProjectivePostionY), cases);
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
    
    
    
    checkResult = myUserTracker->create(&myNiDevice);
    if (checkResult !=nite::STATUS_OK) {
        cout<<"Creating UserTracker failed"<<endl;
        exit(1);
    }
    
    return checkResult;
};

nite::Status Kinect::trackSkeleton(int &caseSelected,std::vector<std::string> cases){
    
    nite::Status checkResult=nite::STATUS_OK;
    openni::Status checkResult2=openni::STATUS_OK;
    nite::UserTrackerFrameRef myUserTrackerFrame;
    
    float leftHandX = 0, leftHandY = 0, rightHandX = 0, rightHandY = 0, torsoX = 0, torsoY = 0, rightShoulderX = 0, rightShoulderY = 0, rightHandZ = 0, torsoZ = 0;
    vector<float> jointPositions;
    int casePosition;

    /*Reading the frame from the UserTracker*/
    checkResult = myUserTracker->readFrame(&myUserTrackerFrame);
    if (checkResult !=nite::STATUS_OK) {
        cout<<"Getting next user frame failed"<<endl;
        exit(1);
    }
    
    depthFrame = myUserTrackerFrame.getDepthFrame();
    
    int resolutionX = depthFrame.getWidth();
    int resolutionY = depthFrame.getHeight();
    
    /*Getting and tracking users detected from the UserTrackerFrame*/
    const nite::Array<nite::UserData>& users = myUserTrackerFrame.getUsers();
    for ( int i = 0; i < users.getSize(); ++i ) {
        const nite::UserData& user = users[i];
        if (user.isNew() && !aUserIsTracked) {
            myUserTracker->startPoseDetection( user.getId(), nite::POSE_PSI );
        }
        else if (!user.isLost()) {
            
            const nite::Skeleton& skeelton = user.getSkeleton();
            
            if (skeelton.getState() == nite::SkeletonState::SKELETON_TRACKED) {
                /*Following the choosen joints*/
                const nite::SkeletonJoint& joint = skeelton.getJoint(nite::JOINT_RIGHT_HAND);/*Attention : the data are hrizontally flipped*/
                if (joint.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& leftHand = joint.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftHand.x, leftHand.y, leftHand.z, &leftHandX, &leftHandY);
                    jointPositions.push_back(leftHandX);
                    jointPositions.push_back(leftHandY);
                }
                const nite::SkeletonJoint& joint2 = skeelton.getJoint(nite::JOINT_LEFT_HAND);/*Attention : the data are hrizontally flipped*/
                if (joint.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& rightHand = joint2.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightHand.x, rightHand.y, rightHand.z, &rightHandX, &rightHandY);
                    rightHandZ = rightHand.z;
                    jointPositions.push_back(rightHandX);
                    jointPositions.push_back(rightHandY);
                }
                const nite::SkeletonJoint& joint3 = skeelton.getJoint(nite::JOINT_TORSO);
                if (joint.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& torso = joint3.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(torso.x, torso.y, torso.z, &torsoX, &torsoY);
                    torsoZ = torso.z;
                    jointPositions.push_back(torsoX);
                    jointPositions.push_back(torsoY);
                }
            }
        }
        else if (user.isLost() && user.getId()==userTracked){
            cout<<"User lost !"<<endl;
            myUserTracker->stopSkeletonTracking(user.getId());
            aUserIsTracked = false;
        }
        
        /*The PSI pose is needed to track, thus the app isn't disturbed by other detected users*/
        const nite::PoseData& pose = user.getPose(nite::POSE_PSI);
        if (pose.isHeld() && !aUserIsTracked) {
            myUserTracker->startSkeletonTracking( user.getId() );
            aUserIsTracked = true;
            userTracked = user.getId();
            cout<<"User tracked !";
            
        }
        
    }
    
    /*Getting the depth data*/
    const openni::DepthPixel* depthData = (const openni::DepthPixel*)depthFrame.getData();
    
    /*Displaying the depth data and the Pad if the user is tracked*/
    myKinectDisplay.displayFrame(depthData, resolutionX, resolutionY, depthFrame.getDataSize(), jointPositions, AREA_X, AREA_Y);
    
    if (aUserIsTracked) {
        /*Adapting left hand coordinates for best comfort for users*/
        leftHandX = (resolutionX-leftHandX/*-resolutionX/AREA_X*/)*PAD_WIDTH/(resolutionX/AREA_X) ;
        leftHandY = (leftHandY-resolutionY/AREA_Y)*PAD_HEIGHT/(resolutionY/AREA_Y) ;
        
        /*On which case is the left hand*/
        casePosition = myKinectDisplay.displayPad(leftHandX, leftHandY, cases, false);
        cout<<"X : "<<leftHandX<<" Y : "<<leftHandY<<endl;
        
        /*Validating the choice by putting right hand on torso*/
        /*Calculating the distance between right hand and torso*/
        float validationVector = sqrt(pow((rightHandX-torsoX),2)+pow((rightHandY-torsoY), 2)+pow((rightHandZ - torsoZ), 2));
        if((validationVector < 100) && (rightHandX!=0)){
            caseSelected = casePosition;
            casePosition = myKinectDisplay.displayPad(leftHandX, leftHandY, cases, true);

        }
    }
    
    
    
    return checkResult;
};


nite::Status Kinect::stopSkeletonTracker(){
    nite::Status checkResult=nite::STATUS_OK;

    /*Releasing ressources used by UserTracker*/
    myUserTracker->destroy();
    
    /*Closing all windows*/
    myKinectDisplay.clearWindow();
    
    return checkResult;
};

void Kinect::displayChoice(std::string choice){
    myKinectDisplay.displayChoice(choice);
}




