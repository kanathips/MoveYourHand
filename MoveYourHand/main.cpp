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

const char *setwin_name = "HSV";
const char *denois_winname = "Denois";
const char *imgshow_name = "Original Image";
int click_count = 0;
const int sample = 5;
vector<vector<int> > Clikc_position;
int avg_color[sample][3];
win_cre win(setwin_name);
Scalar low_hsv, most_hsv;
image capture;

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
        loop = 0;
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
        click_count++;
        Clikc_position.push_back(x_y);
    }
}

void show_text(Mat img, string text)
{
	putText(img, text, Point(img.cols / 10, img.rows / 10), FONT_HERSHEY_PLAIN, 2.0f,Scalar(0,0,255),2);
}

void caribate_color(image cap)
{
    char time[50];
    while(waitKey(30) != 27 && click_count != sample)
    {
        cap.update();
        sprintf(time, "Click on your hand %d times.", sample - click_count);
        string text = string(time);
        show_text(cap.img, text);
        imshow(imgshow_name, cap.img);
        setMouseCallback(imgshow_name, callback_click_posi, NULL);
    }
    win.show_hsv_set();
    win.update_hsv(mean_color());
    while(waitKey(30) != 9)
    {
        cap.update();
        low_hsv = Scalar(win.low_h, win.low_s, win.low_v);
        most_hsv = Scalar(win.most_h, win.most_s, win.most_v);
        capture.cal_bin_img(cap.src, low_hsv, most_hsv);
        string text = string("Press TAB to finish.");
        show_text(cap.src, text);
        imshow(imgshow_name, cap.src);
    }
    destroyWindow(win.img_win);
}

int main()
{
    int camera_no;
    double object_area;
    Moments object;
    while(1)
    {
        printf("Please Enter Your Camera Number: ");
        scanf("%d", &camera_no);
        capture.setup_camera(camera_no);
        if(!capture.cap.isOpened())
            printf("Sorry your camera is not open.\nPlease try again.\n\n");
        else
            break;
    }
    double y_scale = ceil(GetSystemMetrics(SM_CYSCREEN) / capture.width);
    double x_scale = ceil(GetSystemMetrics(SM_CXSCREEN) / capture.height);
    namedWindow(imgshow_name, WINDOW_AUTOSIZE); //create window named Original Image
    caribate_color(capture);
    while(waitKey(30) != 27) //loop until catch esc key
    {
        capture.update(); //update Video frame (look at image.cpp)
        capture.cal_bin_img(capture.src, low_hsv, most_hsv); //convert HSV image to Binary image (capture.src is output argument)
        Canny(capture.src, capture.contour, 10, 10);

        object = capture.bigest_object(object_area);
        /**
                    Mouse Control
        int posi_x = object.m10 / object_area; // find center of x axis
        int posi_y = object.m01 / object_area; // find center of y axis
        if(posi_x > 0 && posi_y > 0)
        {
            circle(capture.img, Point(posi_x, posi_y), 10, Scalar(0, 255, 0), -1); //Draw a green circle in the center of binary image
            SetCursorPos(round(x_scale * (posi_x - 100) * 2), round(y_scale * (posi_y - 100) * 3));
        }
        */
        imshow(imgshow_name, capture.img); //Show Original image
        imshow("Two", capture.src); // Show Binary image
    }
    return 1;
}
