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

void get_Transposion(const vector<Point2f> &originpoints,const Mat &car)
{
    vector<Point2f> targetpoints;
    double length=getDistance(originpoints[0],originpoints[1]);
    double width=getDistance(originpoints[0],originpoints[2]);

    targetpoints.push_back(Point2f(0,0));//left_up
    targetpoints.push_back(Point2f(length,0));//right_up
    targetpoints.push_back(Point2f(0,width));//left_bottom
    targetpoints.push_back(Point2f(length,width));//right_bottom
    //The function 'getAffineTransform' uses three points, while the 'getPerspectiveTransform' uses four.
    Mat T=getPerspectiveTransform(originpoints,targetpoints);
    Mat AT;
    warpPerspective(car,AT,T,Size(length,width));
    imshow("After_Transposion",AT);
    //binary
    cvtColor(AT,AT,COLOR_BGR2GRAY);
    threshold(AT,AT,100,255,THRESH_BINARY);
    imshow("AT_BINARY",AT);
    waitKey(0);
    destroyAllWindows();
}

int According_To_Handmake() {
    Mat car = imread("../origin.jpg");
    if (car.empty())return -1;

    vector<Point2f> originpoints;

    originpoints.push_back(Point2f(598, 333));//left_up
    originpoints.push_back(Point2f(768, 335));//right_up
    originpoints.push_back(Point2f(604, 390));//left_bottom
    originpoints.push_back(Point2f(775, 392));//right_bottom

    get_Transposion(originpoints,car);
}

//Transportation Structure
struct Vision
{
    vector<Point2f> point;
    Mat img;
    Vision(Mat photo){img=photo;}
};
//on_Mouse function
void on_Mouse(int EVENT,int x,int y,int flag,void* param)
{
    char text[50];
    if(EVENT==EVENT_LBUTTONDOWN)
    {
        Vision* v=(Vision*)param;
        v->point.push_back(Point2f(x,y));
        sprintf(text,"(%d,%d)",x,y);

        putText(v->img,text,v->point.back(),FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,0,255));
        circle(v->img,v->point.back(),2,Scalar(0,0,0,255));
        imshow("car",v->img);
    }
}
int According_To_MouseClick()
{
    Mat car=imread("../origin.jpg");
    if(car.empty())return -1;
    imshow("car",car);
    vector<Point2f> targetpoints;
    Vision v(car.clone());
    setMouseCallback("car", on_Mouse, &v);//lp->rp->lb->rb
    waitKey(0);

    if(v.point.empty())return -1;

    get_Transposion(v.point,car);
    return 0;
}

int According_To_BoundryDetection()
{
    Mat car=imread("../origin.jpg");
    if(car.empty())return -1;
    Mat car_HSV;
    cvtColor(car,car_HSV,COLOR_BGR2HSV);
    //split the channels
    //Mat car_3c[3];
    //split(car,car_3c);
    Mat detect1,detect2,detect;
    //range of the board
    inRange(car_HSV,Scalar(100,150,110),Scalar(124,255,180),detect);
    //combine
    imshow("dect",detect);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(detect,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE);
    //drawContours(car,contours,-1,Scalar(255,255,255,0));
    for(int i=1;i<contours.size();++i)
        contours[0].insert(contours[0].end(),contours[i].begin(),contours[i].end());

    vector<Point> point;
    RotatedRect bound=minAreaRect(contours[0]);
    approxPolyDP(contours[0],point,bound.size.height-5,1);

    imshow("car",car);
    waitKey(0);
    vector<Point2f> originpoints;

    //Three find six points.(I don't know how to select them auto.
    originpoints.push_back(point[2]);//left_up
    originpoints.push_back(point[1]);//right_up
    originpoints.push_back(point[3]);//left_bottom
    originpoints.push_back(point[4]);//right_bottom

    get_Transposion(originpoints,car);
    return 0;
}

int main() {
    According_To_Handmake();
    According_To_MouseClick();
    return According_To_BoundryDetection();
}
