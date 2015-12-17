/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#ifndef __RobotKinect__Kinect__
#define __RobotKinect__Kinect__

#include <stdio.h>
#include "NiTE.h"
#include <opencv/cv.h>
#include <math.h>
#include "Thread.h"
#include "KinectMath.h"
#include "KinectDisplay.h"


class Kinect : public Thread{

public:
    Kinect(int kinectID, std::vector<jointPositions> &jointsPositions, std::vector<jointPositions> &jointsPositionsDisplay, int &resolutionX, int &resolutionY, bool &exitB, bool &aUserTracked);
    ~Kinect();
    
    void *run();
    
private:
    
    openni::Status initKinect();
    nite::Status trackSkeleton();
    nite::Status initSkeletonTracker();
    nite::Status stopSkeletonTracker();
    
    nite::UserTracker *myUserTracker;
    nite::UserId myPoseUser;
    openni::Device myNiDevice;
    std::vector<jointPositions> *myJointsPositions;
    std::vector<jointPositions> *myJointsPositionsDisplay;
    int myResolutionX, myResolutionY;
    bool myExit;
    int myKinectID;
    bool aUserIsTracked;
    int userTracked;
    KinectDisplay myKinectDisplay;

};
#endif