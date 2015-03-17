#include "image.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <setting.h>
#include <win_cre.h>

using namespace cv;

image::image()
{
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
    Mat tmp;
    cvtColor(img, tmp, CV_BGR2HSV);
    inRange(tmp, scalar_low, scalar_up, img_out);
}
