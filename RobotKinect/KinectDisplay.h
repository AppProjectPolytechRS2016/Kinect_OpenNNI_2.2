/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include <stdio.h>
#include <string>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include "NiTE.h"

class KinectDisplay{
public:
    KinectDisplay();
    ~KinectDisplay();
    void displayFrame(const openni::DepthPixel* depthData,int resolutionX, int resolutionY, int dataSize, std::vector<float>jointPositions, int areaX, int areaY);
    int displayPad(int positionX, int positionY, std::vector<std::string> cases, bool selectionDone) const;
    void setFrameX(int frameX);
    void setFrameY(int frameY);
    void setPadX(int padX);
    void setPadY(int padY);
    void clearWindow();
    void displayChoice(std::string choice);
    
private:
    int mFrameX;
    int mFrameY;
    int mPadX;
    int mPadY;
    
};