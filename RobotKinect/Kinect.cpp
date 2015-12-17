/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "Kinect.h"

#define AREA_X 3 //Scale of the selection area for the hand
#define AREA_Y 6
#define AREA_Y_H 3
#define PAD_WIDTH 800
#define PAD_HEIGHT 720

using namespace std;

Kinect::Kinect(int kinectID,std::vector<jointPositions> &jointsPositions, std::vector<jointPositions> &jointsPositionsDisplay, int &resolutionX,int &resolutionY, bool &exitB, bool &aUserTracked) : myPoseUser(0){
    myKinectID=kinectID;
    myUserTracker = new nite::UserTracker;
    aUserIsTracked = false;
    myJointsPositions = &jointsPositions;
    myJointsPositionsDisplay = &jointsPositionsDisplay;
    myExit = exitB;
    aUserIsTracked = aUserTracked;
    myKinectDisplay.setPadX(PAD_WIDTH);
    myKinectDisplay.setPadY(PAD_HEIGHT);
    myKinectDisplay.initWindows();
;
    
}

Kinect::~Kinect(){
    
    nite::NiTE::shutdown();
}

//Itinitialisation of the thread
void* Kinect::run(){
    this->initKinect();
    this->initSkeletonTracker();
    while (true) {
        this->trackSkeleton();
    }
    this->stopSkeletonTracker();
    return NULL;
}
//Initialisation of the Kinect
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
    
    myUserTracker = new nite::UserTracker;
    cout<<"Kinect initialized !"<<endl;
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
    else cout<<"UserTracker initialized !"<<endl;
    
    return checkResult;
};

nite::Status Kinect::trackSkeleton(){
        
    nite::Status checkResult=nite::STATUS_OK;
    openni::Status checkResult2=openni::STATUS_OK;
    nite::UserTrackerFrameRef myUserTrackerFrame;
    openni::VideoFrameRef depthFrame;
    
    string what ="test";
    
    jointPositions initPosition;
    initPosition.x=0;
    initPosition.y=0;
    initPosition.z=0;
    
    jointPositions leftShoulderP=initPosition, leftElbowP=initPosition, leftHandP=initPosition, rightShoulderP=initPosition, rightElbowP=initPosition, rightHandP=initPosition, torsoP=initPosition,leftShoulderDisplay=initPosition, leftElbowDisplay=initPosition, leftHandDisplay=initPosition, rightShoulderDisplay=initPosition, rightElbowDisplay=initPosition, rightHandDisplay=initPosition, torsoDisplay=initPosition;
    

    /*Reading the frame from the UserTracker*/
    checkResult = myUserTracker->readFrame(&myUserTrackerFrame);
    if (checkResult !=nite::STATUS_OK) {
        cout<<"Getting next user frame failed"<<endl;
        exit(1);
    }
    
    depthFrame = myUserTrackerFrame.getDepthFrame();
    
    myResolutionX = depthFrame.getWidth();
    myResolutionY = depthFrame.getHeight();
    
    string toDo ="No user detected";
    
    /*Getting and tracking users detected from the UserTrackerFrame*/
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
                
                myJointsPositions->clear();
                myJointsPositionsDisplay->clear();
                
                /*Following the choosen joints*/
                /*Left Shoulder*/
                const nite::SkeletonJoint& joint1 = skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER);/*Attention : the data are hrizontally flipped*/
                if (joint1.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& leftShoulder = joint1.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftShoulder.x, leftShoulder.y, leftShoulder.z, &leftShoulderDisplay.x, &leftShoulderDisplay.y);
                    leftShoulderDisplay.z=leftShoulder.z;
                    leftShoulderP.z=leftShoulder.z;
                    leftShoulderP.x=leftShoulder.x;
                    leftShoulderP.y=leftShoulder.y;
                    myJointsPositionsDisplay->push_back(leftShoulderDisplay);
                    myJointsPositions->push_back(leftShoulderP);
                }
                
                /*Left Elbow*/
                const nite::SkeletonJoint& joint2 = skeleton.getJoint(nite::JOINT_RIGHT_ELBOW);/*Attention : the data are hrizontally flipped*/
                if (joint2.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& leftElbow = joint2.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftElbow.x, leftElbow.y, leftElbow.z, &leftElbowDisplay.x, &leftElbowDisplay.y);
                    leftElbowDisplay.z=leftElbow.z;
                    leftElbowP.z=leftElbow.z;
                    leftElbowP.x=leftElbow.x;
                    leftElbowP.y=leftElbow.y;
                    myJointsPositionsDisplay->push_back(leftElbowDisplay);
                    myJointsPositions->push_back(leftElbowP);
                }
                //Left hand
                const nite::SkeletonJoint& joint3 = skeleton.getJoint(nite::JOINT_RIGHT_HAND);/*Attention : the data are hrizontally flipped*/
                if (joint3.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& leftHand = joint3.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(leftHand.x, leftHand.y, leftHand.z, &leftHandDisplay.x, &leftHandDisplay.y);
                    leftHandDisplay.z=leftHand.z;
                    leftHandP.z=leftHand.z;
                    leftHandP.x=leftHand.x;
                    leftHandP.y=leftHand.y;
                    myJointsPositionsDisplay->push_back(leftHandDisplay);
                    myJointsPositions->push_back(leftHandP);
                }
                
                //Right Shoulder
                const nite::SkeletonJoint& joint4 = skeleton.getJoint(nite::JOINT_LEFT_SHOULDER);/*Attention : the data are hrizontally flipped*/
                if (joint4.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& rightShoulder = joint4.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightShoulder.x, rightShoulder.y, rightShoulder.z, &rightShoulderDisplay.x, &rightShoulderDisplay.y);
                    rightShoulderDisplay.z=rightShoulder.z;
                    rightShoulderP.z=rightShoulder.z;
                    rightShoulderP.x=rightShoulder.x;
                    rightShoulderP.y=rightShoulder.y;
                    myJointsPositionsDisplay->push_back(rightShoulderDisplay);
                    myJointsPositions->push_back(rightShoulderP);
                }
                
                //Right Elbow
                const nite::SkeletonJoint& joint5 = skeleton.getJoint(nite::JOINT_LEFT_ELBOW);/*Attention : the data are hrizontally flipped*/
                if (joint5.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& rightElbow = joint5.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightElbow.x, rightElbow.y, rightElbow.z, &rightElbowDisplay.x, &rightElbowDisplay.y);
                    rightElbowDisplay.z=rightElbow.z;
                    rightElbowP.z=rightElbow.z;
                    rightElbowP.x=rightElbow.x;
                    rightElbowP.y=rightElbow.y;
                    myJointsPositionsDisplay->push_back(rightElbowDisplay);
                    myJointsPositions->push_back(rightElbowP);
                }
                
                //Right hand
                const nite::SkeletonJoint& joint6 = skeleton.getJoint(nite::JOINT_LEFT_HAND);/*Attention : the data are hrizontally flipped*/
                if (joint6.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& rightHand = joint6.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(rightHand.x, rightHand.y, rightHand.z, &rightHandDisplay.x, &rightHandDisplay.y);
                    rightHandDisplay.z=rightHand.z;
                    rightHandP.z=rightHand.z;
                    rightHandP.x=rightHand.x;
                    rightHandP.y=rightHand.y;
                    myJointsPositionsDisplay->push_back(rightHandDisplay);
                    myJointsPositions->push_back(rightHandP);
                }
                //Torso
                const nite::SkeletonJoint& joint7 = skeleton.getJoint(nite::JOINT_TORSO);
                if (joint7.getPositionConfidence() >= 0.5f) {
                    const nite::Point3f& torso = joint7.getPosition();
                    myUserTracker->convertJointCoordinatesToDepth(torso.x, torso.y, torso.z, &torsoDisplay.x, &torsoDisplay.y);
                    torsoDisplay.z=torso.z;
                    torsoP.z=torso.z;
                    torsoP.x=torso.x;
                    torsoP.y=torso.y;
                    myJointsPositionsDisplay->push_back(torsoDisplay);
                    myJointsPositions->push_back(torsoP);
                }
            }
        }
        else if (user.isLost() && user.getId()==userTracked){
            
            toDo = "User lost";
            
            cout<<"User lost !"<<endl;
            myUserTracker->stopSkeletonTracking(user.getId());
            aUserIsTracked = false;
        }
        
        /*The PSI pose is needed to track, thus the app isn't disturbed by other detected users*/
        const nite::PoseData& posePSI = user.getPose(nite::POSE_PSI);
        if (posePSI.isHeld() && !aUserIsTracked) {
            myUserTracker->startSkeletonTracking( user.getId() );
            aUserIsTracked = true;
            userTracked = user.getId();
            cout<<"User tracked !"<<endl;
            myUserTracker->startPoseDetection(user.getId(), nite::POSE_CROSSED_HANDS);
        }
        
        /*The Crossed Hands pose allow to exit case selection without a choice*/
        const nite::PoseData& poseCrossed = user.getPose(nite::POSE_CROSSED_HANDS);
        if (poseCrossed.isHeld()) {
            myExit=true;
        }
        
    }
    
    /*Getting the depth data*/
    const openni::DepthPixel *myDepthData = (const openni::DepthPixel*)depthFrame.getData();
    int myDataSize = depthFrame.getDataSize();
    
    
    //Displaying the data
    /*myKinectDisplay.displayFrame(myDepthData, myResolutionX, myResolutionY, myDataSize, myJointsPositionsDisplay, AREA_X, AREA_Y, what, toDo);
    sleep(1);*/
    cv::namedWindow("Depth frame",cv::WINDOW_AUTOSIZE);

    cv::Mat frame;
    
    frame.create(myResolutionY, myResolutionX, CV_8UC4);
    
    /*Filling the matrix with the pixel data*/
    for(int i=0; i<(myDataSize/sizeof(openni::DepthPixel));i++){
        int index = i*4;
        uchar * data = &frame.data[index];
        uint32_t gray = ((myDepthData[i] * 255) / 8192);
        data[0] = 0;
        data[1] = gray;
        data[2] = gray;
    }
    /*Drawing a circle around each tracked joint*/
    for(int i =0; i < myJointsPositions->size(); i++){
        jointPositions position = myJointsPositions->at(i);
        cv::circle( frame, cvPoint( position.x, position.y ), 5, cv::Scalar( 0, 0, 255 ), 5 );
    }
    
    /*Flip horizontaly the image due to openni data flipped*/
    cv::flip(frame,frame,1);
    
    /*Drawing a rectangle for the selection space*/
    cv::rectangle(frame, cv::Point(0/*resolutionX/areaX*/,myResolutionY/AREA_Y), cv::Point(myResolutionX/**2*//3,myResolutionY/2), cv::Scalar(0,0,255),3);
    
    //Adding text for what you're doing
    cv::putText(frame, what, cv::Point(15,40), CV_FONT_HERSHEY_PLAIN, 2.2f, CV_RGB(0,0,0),2.5);
    cv::putText(frame, toDo, cv::Point(15,myResolutionY-40), CV_FONT_HERSHEY_PLAIN, 2.2f, CV_RGB(255,255,255),2.5);
    cv::imshow("Depth frame",frame);
    int c = cvWaitKey (2); //attente de
    depthFrame.release();
    myUserTrackerFrame.release();
    return checkResult;
};


nite::Status Kinect::stopSkeletonTracker(){
    nite::Status checkResult=nite::STATUS_OK;

    /*Reinit aUserIsTracked*/
    aUserIsTracked=false;
    
    /*Releasing ressources used by UserTracker*/
    myUserTracker->destroy();
    
    return checkResult;
};
