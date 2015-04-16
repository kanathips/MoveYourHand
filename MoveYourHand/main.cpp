#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include <image.h>
#include <windows.h>
#include <win_cre.h>
#include <iostream>
#include <stdio.h>
#include <hand_function.h>

using namespace cv;
using namespace std;

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

void caribate_color(image cap)
{
    char time[50];
    while(waitKey(30) != 27 && click_count != sample)
    {
        cap.update();
        sprintf(time, "Click on your hand %d times.", sample - click_count);
        putText(capture.img, time, Point(20, 60), FONT_HERSHEY_PLAIN, 2.0f,Scalar(0,0,255),2);
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
        Mat text_screen = Mat::zeros(cap.src.size(), CV_8UC1 );
        putText(text_screen, "Press Tab to next step", Point(20, 60), FONT_HERSHEY_PLAIN, 2.0f,Scalar(255), 2);
        cap.src = cap.src + text_screen;
        imshow(imgshow_name, cap.src);
    }
    destroyWindow(win.img_win);
}

int main()
{
    int camera_no, click = 0;
    vector<Point> object;
    Point cursor;
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

    double y_scale = ceil(GetSystemMetrics(SM_CYSCREEN) / capture.height);
    double x_scale = ceil(GetSystemMetrics(SM_CXSCREEN) / capture.width);
    Size downsize(capture.width / 3, capture.height  / 3);
    namedWindow(imgshow_name, WINDOW_AUTOSIZE);

    caribate_color(capture);
    while(waitKey(30) != 27)
    {
        vector<hand_function> fig_vec;
        capture.update();
        capture.cal_bin_img(capture.src, low_hsv, most_hsv);
        Canny(capture.src, capture.contour, 10, 10);

        hand_function hand_cal(capture.contour);
        hand_cal.find_hull();
        hand_cal.find_figer(fig_vec);
        hand_cal.draw(capture.img, fig_vec);
        if(hand_cal.contours_ob.size() > 0)
        {
            cursor = hand_cal.find_middel();
            if(cursor.x > 0 && cursor.y > 0)
            {
              circle(capture.img, cursor, 10, Scalar(0, 255, 0), -1);
              SetCursorPos((cursor.x - 300) * x_scale * 5, (cursor.y - 200) * y_scale * 5);
            }
            if(fig_vec.size() == 4 && click == 0)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, cursor.x, cursor.y, 0, 0);
                click = 1;
            }
            else if(fig_vec.size() == 3 && click == 0)
            {
                mouse_event(MOUSEEVENTF_RIGHTDOWN, cursor.x, cursor.y, 0, 0);
                click = 2;
            }
            else if(fig_vec.size() != 4 && click == 1)
            {
                mouse_event(MOUSEEVENTF_LEFTUP, cursor.x, cursor.y, 0, 0);
                click = 0;
            }
            else if(fig_vec.size() != 3 && click == 2)
            {
                mouse_event(MOUSEEVENTF_RIGHTUP, cursor.x, cursor.y, 0, 0);
                click = 0;
            }
        }
        capture.show_image(imgshow_name);
    }
    return 1;
}
