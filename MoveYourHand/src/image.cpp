#include "image.h"
#include <win_cre.h>
#include <setting.h>
#include <iostream>

using namespace cv;

image::image() // constructor of image class
{

}

void image::setup_camera(int camera_no)
{
    cap = VideoCapture(camera_no);
    cap.read(img);
    width = img.rows;
    height = img.cols;
}
void image::update() //read image form camera I call this update function
{
    cap.read(img);
    flip(img, img, 1); //flip image if you don't do like this you'll get mirrored image
}

void image::cal_bin_img(Mat &img_out, Scalar scalar_low, Scalar scalar_up)
{
    blur(img, src, Size(3,3));
    cvtColor(src, cvt_hsv, CV_BGR2HSV); // Convert from BGR to HSV
    inRange(cvt_hsv, scalar_low, scalar_up, img_out); //Cut the colors are not in this range out. result is binary image
    morphologyEx(src, src, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))); //Function to fill noisy image
    morphologyEx(src, src, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))); //Function to cut the noises out
}

Moments image::bigest_object(double &bigest_area)
{
    vector<vector<Point> > contours_ob;
    vector<Vec4i> hierarchy;
    double contour_area[contours_ob.size()], tmp;
    int i;
    findContours(contour, contours_ob, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    bigest_area = moments(contours_ob[0]).m00;

    for(int i = 0; i < contours_ob.size(); i++ )
    {
        tmp = moments(contours_ob[i]).m00;
        if(tmp < bigest_area)
            continue;
        circle(img, Point(moments(contours_ob[i]).m10 / tmp, moments(contours_ob[i]).m01 / tmp), 3, Scalar(0, 0, 255), -1);
        bigest_area = tmp;
        bigest_contours = moments(contours_ob[i]);
    }
    std::cout << bigest_area << " BIG" << std::endl;
    return bigest_contours;
}
