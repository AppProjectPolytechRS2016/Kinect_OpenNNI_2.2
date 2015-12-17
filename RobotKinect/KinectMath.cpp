//
//  KinectMath.cpp
//  RobotKinect
//
//  Created by Mikael on 10/04/2015.
//  Copyright (c) 2015 mikael. All rights reserved.
//

#include "KinectMath.h"

namespace KMath {
    void rotationFromQuaternion(const nite::Quaternion pQua, std::vector<float>& rotations)
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
    

    void rotation3DFromQuaternion(const std::vector<nite::Quaternion> pQuaternion, std::vector<float>& jointsOrientation)
    {
        std::cout<<"taille du vecteur de quaternion : "<<pQuaternion.size()<<std::endl;
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
    
    void rotation3DFromQuaternion2(const std::vector<nite::Quaternion> pQuaternion, std::vector<float>& jointsOrientation){
        
        for (int i =0; i<pQuaternion.size(); i++) {
            //rotationFromQuaternion
            nite::Quaternion quat = pQuaternion[i];
            float w = quat.w;
            float x = quat.x;
            float y = quat.y;
            float z = quat.z;
            float wy = atan2(2 * ((y * z) + (w * x)), (w * w) - (x * x) - (y * y) + (z * z));
            float wz = asin(2 * ((w * y) - (x * z)));
            float wx= atan2(2 * ((x * y) + (w * z)), (w * w) + (x * x) - (y * y) - (z * z));
            
            jointsOrientation.push_back(wx);
            jointsOrientation.push_back(wy);
            jointsOrientation.push_back(wz);
        }

    };
    
    /*void rotationsFromSegment(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z, float &jointPitch, float &jointRoll){
        float norm;
        norm = sqrtf(pow((joint1X-joint2X),2)+pow((joint1Y-joint2Y),2)+pow((joint1Z-joint2Z),2));
        jointPitch = M_PI/2 - acos((-joint2Z+joint1Z)/norm);
        jointRoll = M_PI/2 + acos((joint2Y-joint1Y)/norm);
    };*/
    void rotationsFromSegment(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z, float &jointPitch, float &jointRoll){
        /*float norm;
         norm = sqrtf(pow((joint1X-joint2X),2)+pow((joint1Y-joint2Y),2)+pow((joint1Z-joint2Z),2));
         jointPitch = M_PI/2 - acos((-joint2Z+joint1Z)/norm);
         jointRoll = M_PI/2 + acos((joint2Y-joint1Y)/norm);*/
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
    
    void elbowRoll(int joint1X, int joint1Y, int joint1Z, int joint2X,int joint2Y, int joint2Z,int joint3X,int joint3Y, int joint3Z,float &jointRoll){
        float y1 = joint1Y-joint2Y;
        float z1 = joint1Z-joint2Z;
        float x1 = joint1X-joint2X;
        float y2 = joint3Y-joint2Y;
        float z2 = joint3Z-joint2Z;
        float x2 = joint3X-joint2X;
        jointRoll=acos((x1*x2+y1*y2+z1*z2)/(sqrt(y1*y1+z1*z1+x1*x1)*sqrt(y2*y2+z2*z2+x2*x2)))-M_PI;
    };

};





