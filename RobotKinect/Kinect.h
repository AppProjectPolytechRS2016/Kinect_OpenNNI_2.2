/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/


#include <stdio.h>
#include "NiTE.h"
#include <opencv/cv.h>
#include "KinectDisplay.h"


class Kinect{
public:
    Kinect(int kinectID);
    ~Kinect();
    openni::Status initKinect();
    nite::Status trackSkeleton(int &robotSelected,std::vector<std::string> cases);
    nite::Status trackHand(int &robotSelected,std::vector<std::string> cases);
    nite::Status initHandTracker();
    nite::Status initSkeletonTracker();
    nite::Status stopHandTracker();
    nite::Status stopSkeletonTracker();


    
private:
    
    openni::Status poseDetect();
    
    nite::HandTracker myHandTracker;
    nite::UserTracker *myUserTracker;
    nite::UserId myPoseUser;
    openni::Device myNiDevice;
    //openni::VideoStream myVideoStream;
    //openni::VideoFrameRef videoFrame;
    openni::VideoFrameRef depthFrame;
    nite::HandTrackerFrameRef myHandTrackerFrame;
    nite::UserTrackerFrameRef myUserTrackerFrame;
    
    KinectDisplay myKinectDisplay;
    
    int myKinectID;
    bool aUserIsTracked;
    
};