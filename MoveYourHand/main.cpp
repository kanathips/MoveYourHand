#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image.h>
#include <windows.h>
#include <win_cre.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace cv;
using namespace std;

/**< Declare Window name *///

char *setwin_name = "Setting";
char *denois_winname = "Denoising";
char *imgshow_name = "Original Image";
int click_cout = 0;
int sample = 5;
vector<vector<int> > Clikc_position;

/**< Create window and image command */
image capture(1); // look image.h and image.cpp
win_cre win(setwin_name, denois_winname); // look at win_cre.h and win_cre.cpp

vector<int> mean_color()
{
    vector<int> avg_color;
    cvtColor(capture.img, capture.cvt_hsv, CV_BGR2HSV);

    int i, posi[3] = {1, 0 , -1}, j = 0, loop = 0, k, x, y;
    int avg_h = 0 , avg_s = 0, avg_v = 0;

    for(k = 0; k < sample; k++)
    {
        x = Clikc_position[k][0];
        y = Clikc_position[k][1];
        while (loop < 9)
        {
            for(i = 0; i < 3; i++)
            {
                avg_h += capture.cvt_hsv.at<Vec3b>(Point(x + posi[j], y + posi[i]))[0]; //pick H tone
                avg_s += capture.cvt_hsv.at<Vec3b>(Point(x + posi[j], y + posi[i]))[1]; //pick S tone
                avg_v += capture.cvt_hsv.at<Vec3b>(Point(x + posi[j], y + posi[i]))[2]; //pick V tone
                j++;
                loop++;
            }
            j = 0;
        }
    }
    avg_color.push_back(avg_h / (9 * sample));
    avg_color.push_back(avg_s / (9 * sample));
    avg_color.push_back(avg_v / (9 * sample));
    return avg_color;
}

void callback_click_posi(int event, int x, int y, int flags, void* userdata)
{
    vector<int> x_y;
    if  ( event == EVENT_LBUTTONDOWN) //if left mouse click
    {
        x_y.push_back(x);
        x_y.push_back(y);
        click_cout++;
        Clikc_position.push_back(x_y);
    }
}

void caribate_color()
{
    char text[100];
    int i, j, x, y;
    vector<int> print_test;
    while(waitKey(30 != 27) && click_cout != sample)
    {
        capture.update();
        sprintf(text, "Please click on your hand %d Time", sample - click_cout);
        imshow(imgshow_name, capture.img);
        setMouseCallback(imgshow_name, callback_click_posi, NULL);
    }
    win.update_hsv(mean_color());
}

int main()
{
    Mat cvt_Mat;
    double tmp_posi_x = -1, tmp_posi_y = -1;
    int y_scale = round(GetSystemMetrics(SM_CYSCREEN) / capture.height);
    int x_scale = round(GetSystemMetrics(SM_CXSCREEN) / capture.width);

    namedWindow(imgshow_name, WINDOW_AUTOSIZE); //create window named Original Image
    caribate_color();

    while(waitKey(30) != 27) //loop until catch esc key
    {
        Scalar low_hsv(win.low_h, win.low_s, win.low_v); //Create Scalar contain lower bound of HSV color
        Scalar most_hsv(win.most_h, win.most_s, win.most_v); //Create Scalar contain upper bound of HSV color

        capture.update(); //update Video frame (look at image.cpp)

        capture.cal_bin_img(cvt_Mat, low_hsv, most_hsv); //convert HSV image to Binary image (cvt_Mat is output argument)

        morphologyEx(cvt_Mat, cvt_Mat, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(win.opn_x, win.opn_y))); //Function to fill noisy image
        morphologyEx(cvt_Mat, cvt_Mat, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(win.cls_x, win.cls_y))); //Function to cut the noises out

        Moments moment_cvt = moments(cvt_Mat, 1);
        double area = moment_cvt.m00; //area of white color of Binary image
        double moment01 = moment_cvt.m01; // Y axis of white space of Binary image
        double moment10 = moment_cvt.m10;// X axis of white space of Binary image

        if (area > 10000) // if area are more than 10000
        {
            int posi_x = moment10 / area; // find center of x axis
            int posi_y = moment01 / area; // find center of y axis

            circle(capture.img, Point(posi_x, posi_y), 10, Scalar(0, 255, 0), -1); //Draw a green circle in the center of binary image

            tmp_posi_x = posi_x; //store old position in temp
            tmp_posi_y = posi_y;
        }

        imshow(imgshow_name, capture.img); //Show Original image
        imshow("Two", cvt_Mat); // Show Binary image
    }
    return 1;
}
