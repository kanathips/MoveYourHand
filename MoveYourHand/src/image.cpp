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
    width = img.cols;
    height = img.rows;
    pyr = Mat(img.size(), CV_8UC3);
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

void image::show_image(const char *imgshow_name)
{
    int i;
    vector<Mat> bw_tmp;
    Mat bw_show;
    for(i = 0;i < 2; i++)
        pyrDown(src, src);
    Rect bw_img(Point(3 * width / 4, 0), src.size());
    for(i = 0; i < 3; i++)
        bw_tmp.push_back(src);
    merge(bw_tmp, bw_show);
    bw_show.copyTo( img(bw_img));
    imshow(imgshow_name, img); //Show Original image
}
