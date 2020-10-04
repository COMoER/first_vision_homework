#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main() {
    Mat apple=imread("../apple.png");
    if(apple.empty())return -1;
    Mat apple_HSV;
    cvtColor(apple,apple_HSV,COLOR_BGR2HSV);
    Mat apple_3c[3];
    split(apple_HSV,apple_3c);
    Mat detect1,detect2,detect;
    //range of red
    inRange(apple_3c[0],156,180,detect2);
    inRange(apple_3c[0],0,10,detect1);
    //combine
    detect=detect1+detect2;

    morphologyEx(detect,detect,MORPH_OPEN,Mat(5, 5, CV_8U));//open operation

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(detect,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE);
    //drawContours(apple,contours,-1,Scalar(255,255,255,0));
    for(int i=0;i<contours.size();++i) {
        if(contours[i][0].x<10||contours[i].size()<5)continue;
        RotatedRect bound = fitEllipse(contours[i]);
        if(bound.size.height<20)continue;
        ellipse(apple, bound,Scalar(0, 255, 0, 0), 2);
    }
    imshow("result",apple);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
