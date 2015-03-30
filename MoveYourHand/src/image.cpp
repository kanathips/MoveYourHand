#include "image.h"
#include <win_cre.h>
#include <setting.h>

using namespace cv;

image::image(int camera) // constructor of image class
{
    cap = VideoCapture(camera);
    cap.read(img);
    flip(img, img, 1);
    width = img.rows;
    height = img.cols;
}
void image::change_camera(int camera_no)
{
    cap = VideoCapture(camera_no);
}
void image::update() //read image form camera I call this update function
{
    cap.read(img);
    flip(img, img, 1); //flip image if you don't do like this you'll get mirrored image
}

void image::cal_bin_img(Mat &img_out, Scalar scalar_low, Scalar scalar_up)
{
    cvtColor(img, cvt_hsv, CV_BGR2HSV); // Convert from BGR to HSV
    inRange(cvt_hsv, scalar_low, scalar_up, img_out); //Cut the colors are not in this range out. result is binary image
}
