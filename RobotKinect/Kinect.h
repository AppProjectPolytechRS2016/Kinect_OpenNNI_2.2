/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/


#include <stdio.h>
#include "NiTE.h"
#include <opencv/cv.h>
#include <math.h>
#include "KinectDisplay.h"


class Kinect{
public:
    Kinect(int kinectID);
    ~Kinect();
    openni::Status initKinect();
    nite::Status trackSkeleton(int &robotSelected,std::vector<std::string> cases, const std::string what);
    std::vector<float> trackSkeletonMime(int64& timeStamp, int countDown);
    nite::Status trackHand(int &robotSelected,std::vector<std::string> cases);
    nite::Status initHandTracker();
    nite::Status initSkeletonTracker();
    nite::Status stopHandTracker();
    nite::Status stopSkeletonTracker();


    
private:
        
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
    
    void rotationsFromSegment(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z, float &jointPitch, float &jointRoll);
    void elbowRoll(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z,int joint3X,int joint3Y, int joint3Z,float &jointRoll);
    
    int myKinectID;
    bool aUserIsTracked;
    int userTracked;
    
};