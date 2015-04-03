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
    if(cap.isOpened())
    {
        cap.read(img);
        flip(img, img, 1); //flip image if you don't do like this you'll get mirrored image
    }
}

void image::cal_bin_img(Mat &img_out, Scalar scalar_low, Scalar scalar_up)
{
    cvtColor(img, cvt_hsv, CV_BGR2HSV); // Convert from BGR to HSV
    inRange(cvt_hsv, scalar_low, scalar_up, img_out); //Cut the colors are not in this range out. result is binary image
    morphologyEx(img_out, img_out, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))); //Function to fill noisy image
    morphologyEx(img_out, img_out, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))); //Function to cut the noises out
    medianBlur(img_out, img_out, 5);
}

void image::bigest_object(int &contour_in)
{
    int i, bigest_area = 0;
    findContours(contour, contours_ob, hierarchy, RETR_TREE , CHAIN_APPROX_SIMPLE, Point(0, 0));
    if (contours_ob.size() > 0)
    {
        for(i = 0; i < contours_ob.size(); i++ )
            if(moments(contours_ob[i]).m00 > bigest_area)
            {
                bigest_area = moments(contours_ob[i]).m00;
                bigest_contours = contours_ob[i];
                contour_in = i;
            }
            drawContours(img, contours_ob, contour_in, Scalar(255,0,0), 3);
    }
}
