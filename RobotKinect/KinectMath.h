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

namespace KMath {
    void rotationFromQuaternion(const nite::Quaternion pQua, std::vector<float>& rotations);
    
    void rotation3DFromQuaternion(const std::vector<nite::Quaternion> pQuaternion, std::vector<float>& jointsOrientation);

    void rotation3DFromQuaternion2(const std::vector<nite::Quaternion> pQuaternion, std::vector<float>& jointsOrientation);

}

#endif /* defined(__RobotKinect__KinectMath__) */
