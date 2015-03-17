#include "win_cre.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;

win_cre::win_cre(char *img_name, char *denois_name)
{
    namedWindow(img_name, WINDOW_AUTOSIZE);

    //Change HSV value
    createTrackbar("Lowest H", img_name, &low_h ,255);
    createTrackbar("Most H", img_name, &most_h ,255);
    createTrackbar("Lowest S", img_name, &low_s ,255);
    createTrackbar("Most S", img_name, &most_s ,255);
    createTrackbar("Lowest V", img_name, &low_v ,255);
    createTrackbar("Most V", img_name, &most_v ,255);

    //Change size of erode and dilate
    namedWindow(denois_name, WINDOW_AUTOSIZE);
    createTrackbar("Open X", denois_name, &opn_x ,20);
    createTrackbar("Open Y", denois_name, &opn_y ,20);
    createTrackbar("Close X", denois_name, &cls_x ,20);
    createTrackbar("Close Y", denois_name, &cls_y ,20);

    img_win = img_name;
    denois_win = denois_name;
}

void win_cre::update_hsv(Vec3b avg_hsv)  //set lower bound and upper bound by avg HSV color with vector
{
    low_h = avg_hsv[0] - 10;
    most_h = avg_hsv[0] + 10;
    low_s = avg_hsv[1] - 10;
    most_s = avg_hsv[1] + 10;
    low_v = avg_hsv[2] - 10;
    most_v = avg_hsv[2] + 10;
    update_trackbar();
}

void win_cre::update_hsv(int avg_h, int avg_s, int avg_v) //set lower bound and upper bound by avg HSV color with each chanal average
{
    low_h = avg_h - 10;
    most_h = avg_h + 10;
    low_s = avg_s - 10;
    most_s = avg_s + 10;
    low_v = avg_v - 10;
    most_v = avg_v + 10;
    update_trackbar();
}

void win_cre::update_trackbar() // function to update trackbar position
{

    setTrackbarPos("Lowest H", img_win, low_h);
    setTrackbarPos("Most H", img_win, most_h);
    setTrackbarPos("Lowest S", img_win, low_s);
    setTrackbarPos("Most S", img_win, most_s);
    setTrackbarPos("Lowest V", img_win, low_v);
    setTrackbarPos("Most V", img_win, most_v);
}
