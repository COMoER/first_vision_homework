#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <math.h>
using namespace cv;
using namespace std;
double getDistance(const Point2f &p1,const Point2f &p2)
{
    return sqrt(powf((p1.x-p2.x),2)+powf((p1.y-p2.y),2));
}
int according_to_handmake()
{
    Mat car=imread("../origin.jpg");
    if(car.empty())return -1;

    vector<Point2f> originpoints,targetpoints;

    originpoints.push_back(Point2f(598,333));//left_up
    originpoints.push_back(Point2f(768,335));//right_up
    originpoints.push_back(Point2f(604,390));//left_bottom
    originpoints.push_back(Point2f(775,392));//right_bottom

    double length=getDistance(originpoints[0],originpoints[1]);
    double width=getDistance(originpoints[0],originpoints[2]);

    targetpoints.push_back(Point2f(0,0));//left_up
    targetpoints.push_back(Point2f(length,0));//right_up
    targetpoints.push_back(Point2f(0,width));//left_bottom
    targetpoints.push_back(Point2f(length,width));//right_bottom
    //The function 'getAffineTransform' uses three points,while the 'getPerspectiveTransform' uses four.
    Mat T=getPerspectiveTransform(originpoints,targetpoints);
    Mat AT;
    warpPerspective(car,AT,T,Size(length,width));
    imshow("AT",AT);
    waitKey(0);
    //binary
    cvtColor(AT,AT,COLOR_BGR2GRAY);
    threshold(AT,AT,100,255,THRESH_BINARY);
    imshow("AT",AT);
    waitKey(0);
    return 0;
}

int main() {
    return according_to_handmake();
}
