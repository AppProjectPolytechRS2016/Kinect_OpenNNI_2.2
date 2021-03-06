/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "Kinect.h"

using namespace std;

#define PAD_WIDTH 800
#define PAD_HEIGHT 720
#define AREA_X 3 //Scale of the selection area for the hand
#define AREA_Y 6
#define AREA_Y_H 3

Kinect::Kinect(int kinectID) : myPoseUser(0){
    myKinectID=kinectID;
    myUserTracker = new nite::UserTracker;
    aUserIsTracked = false;
}

Kinect::~Kinect(){
    nite::NiTE::shutdown();
}

//Initialisation of the Kinect
openni::Status Kinect::initKinect(){
    
    //Openni init
    openni::Status checkResult = openni::OpenNI::initialize();
    if (checkResult != openni::STATUS_OK)
    {
        printf("Failed to initialize OpenNI\n%s\n", openni::OpenNI::getExtendedError());
        return checkResult;
    }
    
    const char* deviceUri = openni::ANY_DEVICE;
    
    //Poening openni device
    checkResult = myNiDevice.open(deviceUri);
    if (checkResult != openni::STATUS_OK)
    {
        printf("Failed to open device\n%s\n", openni::OpenNI::getExtendedError());
        return checkResult;
    }
    
    //NiTE init
    nite::Status checkResult2 = nite::NiTE::initialize();
    if (checkResult2 != nite::STATUS_OK)
    {
        cout<<"Failed to initialize NiTE !"<<endl;
        exit(1);
    }
    
    //Setting the size of the displayPad
    myKinectDisplay.setPadX(PAD_WIDTH);
    myKinectDisplay.setPadY(PAD_HEIGHT);
    
    myUserTracker = new nite::UserTracker;
    
    return checkResult;
};


//Preparing hand tracker
nite::Status Kinect::initHandTracker(){
    nite::Status checkResult = nite::STATUS_OK;
    
    //Creating the hand tracker node
    checkResult = myHandTracker.create();
    if (checkResult!=nite::STATUS_OK) {
        cout<<"creating HandTracker failed"<<endl;
        exit(1);
    }

    //Starting gesture detection : the gesture allows the hand tracking
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

//Tracking hand
nite::Status Kinect::trackHand(int &robotSelected,std::vector<std::string> cases){
    nite::Status checkResult=nite::STATUS_OK;
    int caseSelected=-1;

    //Loading the frame data from the hand tracker
    checkResult = myHandTracker.readFrame(&myHandTrackerFrame);
    if (checkResult != nite::STATUS_OK)
    {
        cout<<"Get next frame failed"<<endl;
        exit(1);
    }

    //Loading the depthFrame data from the frame : needed to define the correspondance between hand position and the position of the selected case on the pad
    openni::VideoFrameRef depthFrame = myHandTrackerFrame.getDepthFrame();
    int resolutionX = depthFrame.getVideoMode().getResolutionX();
    int resolutionY = depthFrame.getVideoMode().getResolutionY();
    
    //Looking for gesture to start tracking
    const nite::Array<nite::GestureData>& gestures = myHandTrackerFrame.getGestures();
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        if (gestures[i].isComplete())
        {
            nite::HandId newId;
            myHandTracker.startHandTracking(gestures[i].getCurrentPosition(), &newId);
            

        }
        //Selecting the case
        if (gestures[i].isComplete() & (gestures[i].getType()==nite::GESTURE_CLICK))
        {
            robotSelected = caseSelected;
            cout<<"robot selected : "<<robotSelected<<endl;
        }
    }
    //Tracking hand
    const nite::Array<nite::HandData>& hands = myHandTrackerFrame.getHands();
    for (int i = 0; i < hands.getSize(); ++i)
    {
        const nite::HandData& hand = hands[i];
        if (hand.isTracking())
        {
            float handProjectivePostionX;
            float handProjectivePostionY;
            //Converting coordinates
            myHandTracker.convertHandCoordinatesToDepth(hand.getPosition().x, hand.getPosition().y, hand.getPosition().z, &handProjectivePostionX, &handProjectivePostionY);
            handProjectivePostionX = (resolutionX-handProjectivePostionX-resolutionX/3)*PAD_WIDTH/(resolutionX/3) ;
            handProjectivePostionY = (handProjectivePostionY-resolutionY/6)*PAD_HEIGHT/(resolutionY/6) ;
            //Displating the Pad and the position of the hand on it
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
    
    myKinectDisplay.initWindows();
    
    checkResult = myUserTracker->create(&myNiDevice);
    if (checkResult !=nite::STATUS_OK) {
        cout<<"Creating UserTracker failed"<<endl;
        exit(1);
    }
    
    return checkResult;
};

nite::Status Kinect::trackSkeleton(int &caseSelected,std::vector<std::string> cases, const std::string what){
    
    nite::Status checkResult=nite::STATUS_OK;
    openni::Status checkResult2=openni::STATUS_OK;
    nite::UserTrackerFrameRef myUserTrackerFrame;
    
    float leftHandX = 0, leftHandY = 0, rightHandX = 0, rightHandY = 0, torsoX = 0, torsoY = 0, rightShoulderX = 0, rightShoulderY = 0, rightHandZ = 0, torsoZ = 0;
    vector<float> jointPositions;
    int casePosition;

    //Reading the frame from the UserTracker
    checkResult = myUserTracker->readFrame(&myUserTrackerFrame);
    if (checkResult !=nite::STATUS_OK) {
        cout<<"Getting next user frame failed"<<endl;
        exit(1);
    }
    
    depthFrame = myUserTrackerFrame.getDepthFrame();
    
    int resolutionX = depthFrame.getWidth();
    int resolutionY = depthFrame.getHeight();
    
    string toDo ="No user detected";
    
    //Getting and tracking users detected from the UserTrackerFrame
    const nite::Array<nite::UserData>& users = myUserTrackerFrame.getUsers();
    for ( int i = 0; i < users.getSize(); ++i ) {
        const nite::UserData& user = users[i];
        if (user.isNew() && !aUserIsTracked) {
            myUserTracker->startPoseDetection( user.getId(), nite::POSE_PSI );
        }
        else if (!user.isLost()) {
            
            const nite::Skeleton& skeleton = user.getSkeleton();
            
            toDo = "Calibration requiered";
            
            if (skeleton.getState() == nite::SkeletonState::SKELETON_TRACKED) {
                
                toDo = "Choose one, cross arms to exit";
                
                //Following the choosen joints
                const nite::SkeletonJoint& joint = skeleton.getJoint(nite::JOINT_RIGHT_HAND);//Attention : the data are hrizontally flipped
                if (joint.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& leftHand = joint.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftHand.x, leftHand.y, leftHand.z, &leftHandX, &leftHandY);
                    jointPositions.push_back(leftHandX);
                    jointPositions.push_back(leftHandY);
                }
                const nite::SkeletonJoint& joint2 = skeleton.getJoint(nite::JOINT_LEFT_HAND);//Attention : the data are hrizontally flipped
                if (joint2.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& rightHand = joint2.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightHand.x, rightHand.y, rightHand.z, &rightHandX, &rightHandY);
                    rightHandZ = rightHand.z;
                    jointPositions.push_back(rightHandX);
                    jointPositions.push_back(rightHandY);
                }
                const nite::SkeletonJoint& joint3 = skeleton.getJoint(nite::JOINT_TORSO);
                if (joint3.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& torso = joint3.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(torso.x, torso.y, torso.z, &torsoX, &torsoY);
                    torsoZ = torso.z;
                    jointPositions.push_back(torsoX);
                    jointPositions.push_back(torsoY);
                }
            }
        }
        else if (user.isLost() && user.getId()==userTracked){
            
            toDo = "User lost";
            
            cout<<"User lost !"<<endl;
            myUserTracker->stopSkeletonTracking(user.getId());
            aUserIsTracked = false;
        }
        
        //The PSI pose is needed to track, thus the app isn't disturbed by other detected users
        const nite::PoseData& posePSI = user.getPose(nite::POSE_PSI);
        if (posePSI.isHeld() && !aUserIsTracked) {
            myUserTracker->startSkeletonTracking( user.getId() );
            aUserIsTracked = true;
            userTracked = user.getId();
            cout<<"User tracked !"<<endl;
            myUserTracker->startPoseDetection(user.getId(), nite::POSE_CROSSED_HANDS);
        }
        
        //The Crossed Hands pose allow to exit case selection without a choice
        const nite::PoseData& poseCrossed = user.getPose(nite::POSE_CROSSED_HANDS);
        if (poseCrossed.isHeld()) {
            caseSelected=-2;
        }
        
    }
    
    //Getting the depth data
    const openni::DepthPixel* depthData = (const openni::DepthPixel*)depthFrame.getData();
    
    //Displaying the depth data and the Pad if the user is tracked
    myKinectDisplay.displayFrame(depthData, resolutionX, resolutionY, depthFrame.getDataSize(), jointPositions, AREA_X, AREA_Y, what, toDo);
    
    if (aUserIsTracked) {
        //Adapting left hand coordinates for best comfort for users
        leftHandX = (resolutionX-leftHandX)*PAD_WIDTH/(resolutionX/AREA_X) ;
        leftHandY = (leftHandY-resolutionY/AREA_Y)*PAD_HEIGHT/(resolutionY/AREA_Y_H) ;
        
        //On which case is the left hand
        casePosition = myKinectDisplay.displayPad(leftHandX, leftHandY, cases, false);
        cout<<"X : "<<leftHandX<<" Y : "<<leftHandY<<endl;
        
        //Validating the choice by putting right hand on torso
        //Calculating the distance between right hand and torso
        float validationVector = sqrt(pow((rightHandX-torsoX),2)+pow((rightHandY-torsoY), 2)+pow((rightHandZ - torsoZ), 2));
        if((validationVector < 100) && (rightHandX!=0)){
            caseSelected = casePosition;
            casePosition = myKinectDisplay.displayPad(leftHandX, leftHandY, cases, true);

        }
    }
    
    
    
    return checkResult;
};

std::vector<float> Kinect::trackSkeletonMime(bool &startCount, int countDown){
    
    nite::Status checkResult=nite::STATUS_OK;
    openni::Status checkResult2=openni::STATUS_OK;
    nite::UserTrackerFrameRef myUserTrackerFrame;
    
    vector<float> jointOrientation;
    vector<float> jointPositions;

    float leftShoulderX = 0, leftShoulderY = 0, rightShoulderX = 0, rightShoulderY = 0, leftElbowX = 0, leftElbowY = 0, leftHandX=0,leftHandY=0,rightElbowX = 0, rightElbowY = 0,leftElbowZ=0,leftShoulderZ=0,leftHandZ= 0,rightElbowZ=0,rightShoulderZ=0,rightHandZ= 0,rightHandX=0,rightHandY=0;
    
    
    //Reading the frame from the UserTracker
    checkResult = myUserTracker->readFrame(&myUserTrackerFrame);
    if (checkResult !=nite::STATUS_OK) {
        cout<<"Getting next user frame failed"<<endl;
        exit(1);
    }
    
    depthFrame = myUserTrackerFrame.getDepthFrame();
    int resolutionX = depthFrame.getWidth();
    int resolutionY = depthFrame.getHeight();
    
    string toDo ="No user detected";
    
    //Getting and tracking users detected from the UserTrackerFrame
    const nite::Array<nite::UserData>& users = myUserTrackerFrame.getUsers();
    for ( int i = 0; i < users.getSize(); ++i ) {
        const nite::UserData& user = users[i];
        if (user.isNew() && !aUserIsTracked) {
            myUserTracker->startPoseDetection( user.getId(), nite::POSE_PSI );
        }
        else if (!user.isLost()) {
            
            toDo = "Calibration requiered";
            
            const nite::Skeleton& skeleton = user.getSkeleton();
            
            if (skeleton.getState() == nite::SkeletonState::SKELETON_TRACKED) {
                
                jointOrientation.clear();
                
                toDo = "Wait for the picture : "+std::to_string(countDown);
                
                //Following the choosen joints
                
                //Left Shoulder
                const nite::SkeletonJoint& joint1 = skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER);//Attention : the data are hrizontally flipped
                if (joint1.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& leftShoulder = joint1.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftShoulder.x, leftShoulder.y, leftShoulder.z, &leftShoulderX, &leftShoulderY);
                    jointPositions.push_back(leftShoulderX);
                    jointPositions.push_back(leftShoulderY);
                    leftShoulderZ = leftShoulder.z;
                    leftShoulderX = leftShoulder.x;
                    leftShoulderY = leftShoulder.y;
                }

                //Left Elbow
                const nite::SkeletonJoint& joint2 = skeleton.getJoint(nite::JOINT_RIGHT_ELBOW);//Attention : the data are hrizontally flipped
                if (joint2.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& leftElbow = joint2.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftElbow.x, leftElbow.y, leftElbow.z, &leftElbowX, &leftElbowY);
                    jointPositions.push_back(leftElbowX);
                    jointPositions.push_back(leftElbowY);
                    leftElbowZ=leftElbow.z;
                    leftElbowX=leftElbow.x;
                    leftElbowY=leftElbow.y;
                }
                
                //Left Hand
                const nite::SkeletonJoint& joint3 = skeleton.getJoint(nite::JOINT_RIGHT_HAND);//Attention : the data are hrizontally flipped
                if (joint3.getPositionConfidence() >= 0.5f) {
                    cout<<"hand tracked"<<endl;
                    const nite::Point3f& leftHand = joint3.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftHand.x, leftHand.y, leftHand.z, &leftHandX, &leftHandY);
                    cout<<"left hand x : "<<leftHandX<<" y :"<<leftHandY<<endl;
                    jointPositions.push_back(leftHandX);
                    jointPositions.push_back(leftHandY);
                    leftHandX=leftHand.x;
                    leftHandY=leftHand.y;
                    leftHandZ=leftHand.z;
                }

                
                //calculate left arm rotations
                float leftShoulderPitch, leftShoulderRoll;
                rotationsFromSegmentG(leftShoulderX,leftShoulderY,leftShoulderZ, leftElbowX,leftElbowY,leftElbowZ,leftShoulderPitch,leftShoulderRoll);
                jointOrientation.push_back(leftShoulderPitch);
                jointOrientation.push_back(leftShoulderRoll);
                float leftElbowRoll;
                elbowRollG(leftShoulderX,leftShoulderY,leftShoulderZ, leftElbowX,leftElbowY,leftElbowZ,leftHandX,leftHandY,leftHandZ, leftElbowRoll);
                jointOrientation.push_back(leftElbowRoll);

                //Right Shoulder
                const nite::SkeletonJoint& joint4 = skeleton.getJoint(nite::JOINT_LEFT_SHOULDER);//Attention : the data are hrizontally flipped
                if (joint4.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& rightShoulder = joint4.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightShoulder.x, rightShoulder.y, rightShoulder.z, &rightShoulderX, &rightShoulderY);
                    jointPositions.push_back(rightShoulderX);
                    jointPositions.push_back(rightShoulderY);
                    rightShoulderX=rightShoulder.x;
                    rightShoulderY=rightShoulder.y;
                    rightShoulderZ=rightShoulder.z;
                }

                //Right Elbow
                const nite::SkeletonJoint& joint5 = skeleton.getJoint(nite::JOINT_LEFT_ELBOW);//Attention : the data are hrizontally flipped
                if (joint5.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& rightElbow = joint5.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightElbow.x, rightElbow.y, rightElbow.z, &rightElbowX, &rightElbowY);
                    jointPositions.push_back(rightElbowX);
                    jointPositions.push_back(rightElbowY);
                    rightElbowX=rightElbow.x;
                    rightElbowY=rightElbow.y;
                    rightElbowZ=rightElbow.z;
                }
                
                //Right Hand
                const nite::SkeletonJoint& joint6 = skeleton.getJoint(nite::JOINT_LEFT_HAND);//Attention : the data are hrizontally flipped
                if (joint6.getPositionConfidence() >= 0.5f) {
                    cout<<"hand tracked"<<endl;
                    const nite::Point3f& rightHand = joint6.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightHand.x, rightHand.y, rightHand.z, &rightHandX, &rightHandY);
                    jointPositions.push_back(rightHandX);
                    jointPositions.push_back(rightHandY);
                    rightHandX=rightHand.x;
                    rightHandY=rightHand.y;
                    rightHandZ=rightHand.z;
                }
                //calculate left arm rotations
                float rightShoulderPitch, rightShoulderRoll;
                rotationsFromSegmentD(rightShoulderX,rightShoulderY,rightShoulderZ, rightElbowX,rightElbowY,rightElbowZ,rightShoulderPitch,rightShoulderRoll);
                jointOrientation.push_back(rightShoulderPitch);
                jointOrientation.push_back(rightShoulderRoll);
                float rightElbowRoll;
                elbowRollG(rightShoulderX,rightShoulderY,rightShoulderZ, rightElbowX,rightElbowY,rightElbowZ,rightHandX,rightHandY,rightHandZ, rightElbowRoll);
                jointOrientation.push_back(-rightElbowRoll);
            }
            
        }
        else if (user.isLost() && user.getId()==userTracked){
            cout<<"User lost !"<<endl;
            myUserTracker->stopSkeletonTracking(user.getId());
            aUserIsTracked = false;
        }
        
        //The PSI pose is needed to track, thus the app isn't disturbed by other detected users
        const nite::PoseData& posePSI = user.getPose(nite::POSE_PSI);
        if (posePSI.isHeld() && !aUserIsTracked) {
            myUserTracker->startSkeletonTracking( user.getId() );
            aUserIsTracked = true;
            userTracked = user.getId();
            cout<<"User tracked !"<<endl;
            myUserTracker->startPoseDetection(user.getId(), nite::POSE_CROSSED_HANDS);
        }
        
        //The Crossed Hands pose allow to exit case selection without a choice
        const nite::PoseData& poseCrossed = user.getPose(nite::POSE_CROSSED_HANDS);
        if (poseCrossed.isHeld()) {
            jointOrientation.clear();
        }
        
    }
    
    //Getting the depth data
    const openni::DepthPixel* depthData = (const openni::DepthPixel*)depthFrame.getData();
    
    //Displaying the depth data and the Pad if the user is tracked
    myKinectDisplay.displayFrame(depthData, resolutionX, resolutionY, depthFrame.getDataSize(), jointPositions, 10, 10, "Strike the pose ! ",toDo);
    
    startCount=aUserIsTracked;
    return jointOrientation;
};

nite::Status Kinect::stopSkeletonTracker(){
    nite::Status checkResult=nite::STATUS_OK;

    //Reinit aUserIsTracked
    aUserIsTracked=false;
    
    //Releasing ressources used by UserTracker
    myUserTracker->destroy();
    
    //Closing all windows
    myKinectDisplay.clearWindow();
    
    return checkResult;
};

void Kinect::rotationsFromSegmentG(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z, float &jointPitch, float &jointRoll){
    //float norm;
    float y = joint2Y-joint1Y;
    float z = joint2Z-joint1Z;
    float x = joint2X-joint1X;
    if (y==0) {
        jointPitch = 0.01;
    }
    else if (y>0) {
        jointPitch = -atan(y/(-z));
    }
    else if (y<0){
        if(z<0){
            jointPitch = atan(y/z);
        }
        else if (z==0){
            jointPitch = M_PI/2;
        }
        else if (z>0){
            jointPitch = M_PI/2 + atan(z/(-y));
        }
    }

    if (y>=0) {
        jointRoll = M_PI/2;
    }
    else{
        if(x<=0){
            jointRoll = 0.01;
        }
        else{
            jointRoll = atan(x/(-y));
        }
    }
};

void Kinect::rotationsFromSegmentD(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z, float &jointPitch, float &jointRoll){
    //float norm;
    float y = joint2Y-joint1Y;
    float z = joint2Z-joint1Z;
    float x = joint2X-joint1X;
    if (y==0) {
        jointPitch = 0.01;
    }
    else if (y>0) {
        jointPitch = -atan(y/(-z));
    }
    else if (y<0){
        if(z<0){
            jointPitch = atan(y/z);
        }
        else if (z==0){
            jointPitch = M_PI/2;
        }
        else if (z>0){
            jointPitch = M_PI/2 + atan(z/(-y));
        }
    }
    
    if (y>=0) {
        jointRoll = -M_PI/2;
    }
    else{
        if(x>=0){
            jointRoll = 0.01;
        }
        else{
            jointRoll = -atan(x/y);
        }
    }
};

void Kinect::elbowRollG(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z,int joint3X,int joint3Y, int joint3Z,float &jointRoll){
    float y1 = joint1Y-joint2Y;
    float z1 = joint1Z-joint2Z;
    float x1 = joint1X-joint2X;
    float y2 = joint3Y-joint2Y;
    float z2 = joint3Z-joint2Z;
    float x2 = joint3X-joint2X;
    jointRoll=acos((x1*x2+y1*y2+z1*z2)/(sqrt(y1*y1+z1*z1+x1*x1)*sqrt(y2*y2+z2*z2+x2*x2)))-M_PI;
};




