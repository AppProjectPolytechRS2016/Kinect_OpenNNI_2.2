/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "KinectDisplay.h"

/*Builder*/
KinectDisplay::KinectDisplay(){}

KinectDisplay::~KinectDisplay(){}

/*Getters and Setters*/
void KinectDisplay::setFrameX(int frameX){
    this->mFrameX=frameX;
}
void KinectDisplay::setFrameY(int frameY){
    this->mFrameY=frameY;
}
void KinectDisplay::setPadX(int padX){
    this->mPadX=padX;
}
void KinectDisplay::setPadY(int padY){
    this->mPadY=padY;
}

void KinectDisplay::displayFrame(const openni::RGB888Pixel* imageBuffer,openni::VideoFrameRef videoFrame){
    cv::Mat frame;
    frame.create(videoFrame.getHeight(), videoFrame.getWidth(), CV_8UC3);
    memcpy( frame.data, imageBuffer, 3*videoFrame.getHeight()*videoFrame.getWidth()*sizeof(uint8_t) );
    
    cv::cvtColor(frame,frame,CV_BGR2RGB); //this will put colors right
    
}

int KinectDisplay::displayPad(int positionX, int positionY, std::vector<std::string> cases) const{
    int selectedCase = -1;
    int mPosX = positionX;
    int mPosY = positionY;
    unsigned long mNumCase = cases.size();
    int numCol = 3;
    int numRow = ceil(mNumCase/(float)numCol);
    std::string  word;
    int rectXOrg;
    int rectYOrg;
    int rectXEnd;
    int rectYEnd;
    int i=0;
    
    /*Create OpenCV image*/
    cv::Mat imagePad;
    imagePad.create(this->mPadY,this->mPadX,CV_8UC3);
    /*Fill with white*/
    cv::rectangle(imagePad,cvPoint(0,0),cv::Point(this->mPadX,this->mPadY),CV_RGB(255,255,255),CV_FILLED);
    /*Add the cases*/
    for(int row=0;row<numRow;row++){
        if((row+1)*numCol>mNumCase){
            numCol=mNumCase-(row)*numCol;
        }
        for(int col=0;col<numCol;col++){
            rectXOrg=(int)((this->mPadX)/numCol*col);
            rectYOrg=(int)((this->mPadY)/numRow*row);
            rectXEnd=(int)((this->mPadX)/numCol*(col+1));
            rectYEnd=(int)((this->mPadY)/numRow*(row+1));
            /*The case is selected*/
            if( rectXOrg<mPosX && mPosX<rectXEnd && rectYOrg<mPosY && mPosY<rectYEnd ){
                /*Drawing a red rectangle in the black box*/
                cv::rectangle(imagePad,cv::Point(rectXOrg+2,rectYOrg+2),cv::Point(rectXEnd-2,rectYEnd-2),CV_RGB(255,0,0),2);
                selectedCase=i;
            }
            /*Drawing all the cases*/
            cv::rectangle(imagePad,cv::Point(rectXOrg,rectYOrg),cv::Point(rectXEnd,rectYEnd),CV_RGB(0,0,0),2);
            /*There is a text for this cases*/
            if (i<mNumCase) {
                word = cases[i];
                cv::putText(imagePad, word, cv::Point(rectXOrg+15,rectYOrg+(rectYEnd-rectYOrg)/2), CV_FONT_HERSHEY_PLAIN, 3.0f, CV_RGB(0,0,0),1);
                i++;
            }
            
        }
    }
    
    //cvNamedWindow("Image :");
    cv::imshow("Image :",imagePad);
    int c = cvWaitKey (2); //attente de 2ms qu'une touche soit pressée, !! permet le rafraîchissement des images !!
    return selectedCase;
}
