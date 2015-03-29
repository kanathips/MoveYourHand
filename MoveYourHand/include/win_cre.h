#ifndef WINDOWS_H
#define WINDOWS_H

#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class win_cre
{
    public:
        const char *img_win;
        int low_h = 0, low_s = 0, low_v = 0;
        int most_h = 255, most_s = 255, most_v = 255;
        win_cre(const char *win_name_img);
        void update_trackbar();
        void show_hsv_set();
        void update_hsv(vector<int> avg_hsv);
        void update_hsv(int avg_h, int avg_s, int avg_v);
};

#endif // WINDOWS_H
