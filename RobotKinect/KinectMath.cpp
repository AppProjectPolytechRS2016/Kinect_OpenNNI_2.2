//
//  KinectMath.cpp
//  RobotKinect
//
//  Created by Mikael on 10/04/2015.
//  Copyright (c) 2015 mikael. All rights reserved.
//

#include "KinectMath.h"

namespace KMath {
    void rotationFromQuaternion(const nite::Quaternion pQua, std::vector<float> rotations)
    {
        rotations.push_back(1.0f - 2.0f*(pow(pQua.y, 2) + pow(pQua.z, 2)));
        rotations.push_back(2.0f*(pQua.x*pQua.y - pQua.z*pQua.w));
        rotations.push_back(2.0f*(pQua.x*pQua.z + pQua.y*pQua.w));
        
        rotations.push_back(2.0f*(pQua.x*pQua.y + pQua.z*pQua.w));
        rotations.push_back(1.0f - 2.0f*(pow(pQua.x, 2) + pow(pQua.z, 2)));
        rotations.push_back(2.0f*(pQua.y*pQua.z - pQua.x*pQua.w));
        
        rotations.push_back(2.0f*(pQua.x*pQua.z - pQua.y*pQua.w));
        rotations.push_back(2.0f*(pQua.y*pQua.z + pQua.x*pQua.w));
        rotations.push_back(1.0f - 2.0f*(pow(pQua.x, 2) + pow(pQua.y, 2)));
    };
    

    void rotation3DFromQuaternion(const std::vector<nite::Quaternion> pQuaternion, std::vector<float> jointsOrientation)
    {
        for (int i =0; i<pQuaternion.size(); i++) {
            //rotationFromQuaternion
            std::vector<float> rotations;
            rotationFromQuaternion(pQuaternion[i], rotations);
            
            float wz = atan2(rotations[3], rotations[0]);//z
            const float sy = sin(wz);
            const float cy = cos(wz);
            float wy = atan2(-rotations[6], cy*rotations[0]+sy*rotations[3]);//y
            float wx = atan2(sy*rotations[2]-cy*rotations[5], cy*rotations[4]-sy*rotations[3]);//x
            
            jointsOrientation.push_back(wx);
            jointsOrientation.push_back(wy);
            jointsOrientation.push_back(wz);
        }
    };

};





