#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <image.h>
#include <win_cre.h>
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

const char *setwin_name = "Setting";
const char *imgshow_name = "Original Image";

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {

     }
}

int main()
{
    Mat cvt_Mat;
    double tmp_posi_x = -1, tmp_posi_y = -1;
    win_cre win(setwin_name);
    image capture;
    namedWindow(imgshow_name, WINDOW_AUTOSIZE);
    Mat Avg;
    while(true)
    {
        capture.update();
        setMouseCallback(imgshow_name, CallBackFunc, Avg);
    }



    while(waitKey(30) != 27) //delay 30 millisecond for show image
    {
        POINT mouse_click;
        Scalar low_hsv(win.low_h, win.low_s, win.low_v);
        Scalar most_hsv(win.most_h, win.most_s, win.most_v);

        capture.update();

        capture.cal_bin_img(cvt_Mat, low_hsv, most_hsv);

        morphologyEx(cvt_Mat, cvt_Mat, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(win.dil_x, win.dil_y)));
        morphologyEx(cvt_Mat, cvt_Mat, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(win.erd_x, win.erd_y)));

        Moments moment_cvt = moments(cvt_Mat, 1);
        double area = moment_cvt.m00;
        double moment01 = moment_cvt.m01;
        double moment10 = moment_cvt.m10;

        cout << mouse_click.x << " " << mouse_click.y << endl;
        if (area > 10000)
        {
            int posi_x = moment10 / area;
            int posi_y = moment01 / area;

            if (tmp_posi_x >= 0 && tmp_posi_y >= 0 && posi_x >= 0 && posi_y >= 0)
                circle(capture.img, Point(posi_x, posi_y), 10, Scalar(0, 255, 0), -1);

            tmp_posi_x = posi_x;
            tmp_posi_y = posi_y;
        }

        imshow(imgshow_name, capture.img);
        imshow("Two", cvt_Mat);

    }
    return 1;
}
