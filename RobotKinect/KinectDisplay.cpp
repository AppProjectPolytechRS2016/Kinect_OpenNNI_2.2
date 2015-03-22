/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "KinectDisplay.h"

#define MAX_COL 3
#define SHORT_PAUSE 2
#define LONG_PAUSE 1000

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

void KinectDisplay::displayFrame(const openni::DepthPixel* depthData,int resolutionX, int resolutionY, int dataSize, std::vector<float>jointPositions, int areaX, int areaY){
    cv::Mat frame;
    
    frame.create(resolutionY, resolutionX, CV_8UC4);
    
    /*Filling the matrix with the pixel data*/
    for(int i=0; i<(dataSize/sizeof(openni::DepthPixel));i++){
        int index = i*4;
        uchar * data = &frame.data[index];
        int gray = ~((depthData[i] * 255) / 10000);
        data[0] = gray;
        data[1] = gray;
        data[2] = gray;
    }
    /*Drawing a circle around each tracked joint*/
    for(int i =0; i < (jointPositions.size()/2); i++){
        cv::circle( frame, cvPoint( jointPositions[i*2], jointPositions[i*2+1] ), 5, cv::Scalar( 0, 0, 255 ), 5 );
    }
    
    /*Flip horizontaly the image due to openni data flipped*/
    cv::flip(frame,frame,1);
    
    /*Drawing a rectangle for the selection space*/
    cv::rectangle(frame, cv::Point(0/*resolutionX/areaX*/,resolutionY/areaY), cv::Point(resolutionX/**2*//3,resolutionY/3), cv::Scalar(0,0,255),3);
    
    cv::imshow("Depth frame",frame);
    int c = cvWaitKey (2); //attente de 2ms qu'une touche soit pressée, !! permet le rafraîchissement des images !!
    
}

int KinectDisplay::displayPad(int positionX, int positionY, std::vector<std::string> cases, bool selectionDone) const{
    int selectedCase = -1;
    int mPosX = positionX;
    int mPosY = positionY;
    unsigned long mNumCase = cases.size();
    int numCol = MAX_COL;
    int numRow = ceil(mNumCase/(float)numCol);
    std::string  word;
    int rectXOrg;
    int rectYOrg;
    int rectXEnd;
    int rectYEnd;
    int i=0;
    int rectThickness=2;
    int pauseTime=SHORT_PAUSE;
    
    if (selectionDone) {
        rectThickness=-1;
        pauseTime=LONG_PAUSE;
    }
    
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
                cv::rectangle(imagePad,cv::Point(rectXOrg+2,rectYOrg+2),cv::Point(rectXEnd-2,rectYEnd-2),CV_RGB(255,0,0),rectThickness);
                selectedCase=i;
            }
            /*Drawing all the cases*/
            cv::rectangle(imagePad,cv::Point(rectXOrg,rectYOrg),cv::Point(rectXEnd,rectYEnd),CV_RGB(0,0,0),2
                          );
            /*There is a text for this cases*/
            if (i<mNumCase) {
                word = cases[i];
                cv::putText(imagePad, word, cv::Point(rectXOrg+15,rectYOrg+(rectYEnd-rectYOrg)/2), CV_FONT_HERSHEY_PLAIN, 3.0f, CV_RGB(0,0,0),1);
                i++;
            }
            
        }
    }
    
    cv::imshow("selection Pad",imagePad);
    
    int c = cvWaitKey (pauseTime); //attente de 2ms qu'une touche soit pressée, !! permet le rafraîchissement des images !!
    
    return selectedCase;
}

void KinectDisplay::clearWindow(){
    cv::destroyAllWindows();
    
}

void KinectDisplay::displayChoice(std::string choice){
    cv::displayOverlay("Choice", choice, 2000);
}
