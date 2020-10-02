#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace cv;
class CameraCali {
private:
    vector<vector<Point2f>> cornersvec;
    vector<vector<Point3f>> objectpoints;
    double width;
    int numofphoto;
    Size boardsize;
    Size wholesize;
public:
    Mat cameraMatrix;//camera inner coef
    Mat DistMatrix;//jn
    vector<Mat> rvecs;//R
    vector<Mat> tvecs;//T
    vector<double> out_coef, in_coef, err;

    CameraCali(double w= 17, int n= 41,int r=6,int c=9,int pw=1280,int ph=720) :
    width(w),numofphoto(n),boardsize(c,r),wholesize(pw,ph){}
    double getCoef();
    vector<Point2f> getrealworldposition(const vector<Point3f> &real);

};
double CameraCali::getCoef() {
        for (int no = 0;no < numofphoto; ++no) {
            Mat chess;
            vector<Point2f> corners;
            //document's path
            string r = "../chess/";
            if (no < 10)r.append(1, '0' + no);
            else {
                r.append(1, '0' + no / 10);
                r.append(1, '0' + no % 10);
            }
            r += ".jpg";
            chess = imread(r);
            if (chess.empty())return -1;
            Mat chess_Gray;//sing 8-bit Gray Image as the input image will improve the precision
            cvtColor(chess, chess_Gray, COLOR_BGR2GRAY);

            findChessboardCorners(chess_Gray, boardsize, corners);
            find4QuadCornerSubpix(chess_Gray, corners,
                                  Size(5, 5));//Region Size is the size of the region to optimize the subpix
            cornersvec.push_back(corners);
            /*
            drawChessboardCorners(chess, Size(9, 6), corners, 1);
            imshow("1", chess);//But mark on the origin photo
            waitKey(0);
            */
            objectpoints.push_back(vector<Point3f>());

            //give the real world position

            for (int i = 1; i <= boardsize.height; ++i)
                for (int j = 1; j <= boardsize.width; ++j)
                    objectpoints[no].push_back(Point3f(width * j, width * i, 0));
        }
        return calibrateCamera(objectpoints, cornersvec, wholesize, cameraMatrix, DistMatrix, rvecs,
                        tvecs, in_coef, out_coef, err,CALIB_FIX_PRINCIPAL_POINT);
    }
vector<Point2f> CameraCali::getrealworldposition(const vector<Point3f> &real)
{
    vector<Point2f> img;
    projectPoints(real,rvecs[0],tvecs[0],cameraMatrix,DistMatrix,img);
    return img;
}
int main()
{
    CameraCali c(17,1,6,9,1280,720);
    cout<<"error is "<<c.getCoef()<<endl;
    cout<<"CameraMatrix:\n"<<c.cameraMatrix<<endl;
    cout<<"DistMatrix:"<<c.DistMatrix;
    vector<Point3f> p;
    p.push_back(Point3f(17,17,0));
    cout<<"\n(17,17,0) real->"<<c.getrealworldposition(p)[0]<<"image";
    return 0;
}
