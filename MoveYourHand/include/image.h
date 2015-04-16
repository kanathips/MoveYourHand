#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class image
{
    public:
        Mat img, cvt_hsv, src, contour, pyr;
        int width, height;
        VideoCapture cap;
        image();
        void update();
        void show_image(const char *imgshow_name);
        void setup_camera(int camera_no);
        void cal_bin_img(Mat &img_out, Scalar scalar_low, Scalar scalar_up);
};

#endif // IMAGE_H
