//
//  KinectMath.h
//  RobotKinect
//
//  Created by Mikael on 10/04/2015.
//  Copyright (c) 2015 mikael. All rights reserved.
//

#ifndef __RobotKinect__KinectMath__
#define __RobotKinect__KinectMath__

#include "NiTE.h"
#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>

typedef struct {float x;float y;float z;} jointPositions;

namespace KMath {
    void rotationFromQuaternion(const nite::Quaternion pQua, std::vector<float>& rotations);
    
    void rotation3DFromQuaternion(const std::vector<nite::Quaternion> pQuaternion, std::vector<float>& jointsOrientation);

    void rotation3DFromQuaternion2(const std::vector<nite::Quaternion> pQuaternion, std::vector<float>& jointsOrientation);
    
    void rotationsFromSegment(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z, float &jointPitch, float &jointRoll);

    
    void rotationsFromSegment(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z, float &jointPitch, float &jointRoll);
    void elbowRoll(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z,int joint3X,int joint3Y, int joint3Z,float &jointRoll);
}

#endif /* defined(__RobotKinect__KinectMath__) */
