#include "win_cre.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;

win_cre::win_cre(const char *win_name)
{
    namedWindow(win_name, WINDOW_AUTOSIZE);

    //Change HSV value
    createTrackbar("Lowest H", win_name, &low_h ,255);
    createTrackbar("Most H", win_name, &most_h ,255);
    createTrackbar("Lowest S", win_name, &low_s ,255);
    createTrackbar("Most S", win_name, &most_s ,255);
    createTrackbar("Lowest V", win_name, &low_v ,255);
    createTrackbar("Most V", win_name, &most_v ,255);

    //Change size of erode and dilate
    namedWindow("Dil_and_Erd", WINDOW_AUTOSIZE);
    createTrackbar("Dilate X", "Dil_and_Erd", &dil_x ,20);
    createTrackbar("Dilate Y", "Dil_and_Erd", &dil_y ,20);
    createTrackbar("Erode X", "Dil_and_Erd", &erd_x ,20);
    createTrackbar("Erode Y", "Dil_and_Erd", &erd_y ,20);

}
