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
    void displayFrame(const openni::RGB888Pixel* imageBuffer,openni::VideoFrameRef videoFrame);
    int displayPad(int positionX, int positionY, std::vector<std::string> cases) const;
    void setFrameX(int frameX);
    void setFrameY(int frameY);
    void setPadX(int padX);
    void setPadY(int padY);
    
private:
    int mFrameX;
    int mFrameY;
    int mPadX;
    int mPadY;
    
};