#include "image.h"
#include <win_cre.h>
#include <setting.h>

using namespace cv;

image::image(int camera)
{
    camera_no = camera;
    cap = VideoCapture(camera);
    cap.read(img);
    flip(img, img, 1);
    width = img.rows;
    height = img.cols;
}

void image::update()
{
    cap.read(img);
    flip(img, img, 1);
}

void image::cal_bin_img(Mat &img_out, Scalar scalar_low, Scalar scalar_up)
{
    cvtColor(img, cvt_hsv, CV_BGR2HSV);
    inRange(cvt_hsv, scalar_low, scalar_up, img_out);
}
