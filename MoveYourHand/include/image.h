#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class image
{
    public:
        Mat img, cvt_hsv;
        int camera_no;
        int width, height;
        VideoCapture cap;
        image(int camera_no);
        void update();
        void cal_bin_img(Mat &img_out, Scalar scalar_low, Scalar scalar_up);
};

#endif // IMAGE_H
