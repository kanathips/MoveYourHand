#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image.h>
#include <windows.h>
#include <win_cre.h>
#include <iostream>
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

string check_move(int posi_x[2], int posi_y[2], int x_scale, int y_scale)
{
    string move_way;
    if(posi_x[0] > posi_x[1] + 10 * x_scale)
    {
        move_way = "Move Right";
        if(posi_y[0] > posi_y[1] +  10 * y_scale)
            move_way += " And Down";
        else if(posi_y[0] < posi_y[1] -  10 * y_scale)
            move_way += " And Up";
    }
    else if(posi_x[0] < posi_x[1] - 10 * x_scale)
    {
        move_way = "Move Left";
        if(posi_y[0] > posi_y[1] +  10 * y_scale)
            move_way += " And Down";
        else if(posi_y[0] < posi_y[1] -  10 * y_scale)
            move_way += " And Up";
    }
    else
    {
        if(posi_y[0] > posi_y[1] +  10 * y_scale)
            move_way =  "Move Down";
        else if(posi_y[0] < posi_y[1] -  10 * y_scale)
            move_way = "Move Up";
    }
    return move_way;
}
int main()
{
    int camera_no, contour_in, i, j;
    vector<Point> object;
    vector<Vec4i> hierarchy;
    vector<vector<Point> > out;
    Moments object_moment;
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
    int posi_x[2] = {0}, posi_y[2] = {0};
    namedWindow(imgshow_name, WINDOW_AUTOSIZE); //create window named Original Image
    caribate_color(capture);
    while(waitKey(30) != 27) //loop until catch esc key
    {
        capture.update(); //update Video frame (look at image.cpp)

        capture.cal_bin_img(capture.src, low_hsv, most_hsv); //convert HSV image to Binary image (capture.src is output argument)
        Canny(capture.src, capture.contour, 10, 10);

        capture.bigest_object(contour_in);
        object = capture.bigest_contours;
        object_moment = moments(object);
        if (capture.contours_ob.size() > 0)
        {
            vector<vector<Point> >hull( capture.contours_ob.size() );
			//find the defects points for each contour
			vector<vector<Vec4i> > defects( capture.contours_ob.size()) ;

			vector<vector<int> > hullsI(capture.contours_ob.size());

			//find the biggest contour

			Point2f rect_points[4];
			vector<RotatedRect> minRect( capture.contours_ob.size() );

			vector<vector<Point> > contours_poly( capture.contours_ob.size() );
			vector<Rect> boundRect( capture.contours_ob.size() );
			try
			{
                for(i = 0; i < capture.contours_ob.size(); i++ )
                {
                    convexHull( Mat(capture.contours_ob[i]), hull[i], false );
                    convexHull( Mat(capture.contours_ob[i]), hullsI[i], false );
                    convexityDefects(Mat(capture.contours_ob[i]),hullsI[i], defects[i]);

                    if(contour_in == i)
                    {
                        minRect[i] = minAreaRect( Mat(capture.contours_ob[i]) );

                        drawContours( capture.img, capture.contours_ob,contour_in, CV_RGB(255,255,255), 2, 8, hierarchy,0, Point() );
                        drawContours( capture.img, hull, contour_in, CV_RGB(255,0,0), 2, 8, hierarchy, 0, Point() );

                        approxPolyDP( Mat(capture.contours_ob[i]), contours_poly[i], 3, true );
						boundRect[i] = boundingRect( Mat(contours_poly[i]) );

						rectangle( capture.img  , boundRect[i].tl(), boundRect[i].br(), CV_RGB(0,0,0), 2, 8, 0 );

                        Point2f rect_points[4];
                        minRect[i].points( rect_points );

                        for(j = 0; j < 4; j++ )
                            line( capture.img, rect_points[j], rect_points[(j+1)%4], CV_RGB(255,255,0), 2, 8 );
                    }
                }
			}
			catch(Exception ex)
			{
			    cout << "error" << endl;
			}

			for( int i = 0; i< capture.contours_ob.size(); i++ )
			{
				size_t count = capture.contours_ob[i].size();
				cout<<"Count : " <<count <<endl;
				if( count < 300 )
					continue;

				vector<Vec4i>::iterator d =defects[i].begin();

				while( d!=defects[i].end() ) {
					Vec4i& v=(*d);
					if(contour_in == i){

						int startidx=v[0];
						Point ptStart( capture.contours_ob[i][startidx] ); // point of the contour where the defect begins
						int endidx=v[1];
						Point ptEnd( capture.contours_ob[i][endidx] ); // point of the contour where the defect ends
						int faridx=v[2];
						Point ptFar( capture.contours_ob[i][faridx] ); // the farthest from the convex hull point within the defect
						float depth = v[3] / 256; // distance between the farthest point and the convex hull


						if(depth > 20 && depth < 80)
						{
							line( capture.img, ptStart, ptFar, CV_RGB(0,255,0), 2 );
							line( capture.img, ptEnd, ptFar, CV_RGB(0,255,0), 2 );
							circle( capture.img, ptStart,   4, Scalar(100,0,255), 2 );
						}

					}
					d++;
				}
			}
        }

        posi_x[0] = object_moment.m10 / object_moment.m00; // find center of x axis
        posi_y[0] = object_moment.m01 / object_moment.m00; // find center of y axis
        if(posi_x[0] > 0 && posi_y[0] > 0)
            circle(capture.img, Point(posi_x[0], posi_y[0]), 10, Scalar(0, 255, 0), -1); //Draw a green circle in the center of binary image
        show_text(capture.img, check_move(posi_x, posi_y, x_scale, y_scale));

        posi_x[1] = posi_x[0];
        posi_y[1] = posi_y[0];

        imshow(imgshow_name, capture.img); //Show Original image
        imshow("Two", capture.src); // Show Binary image
    }
    return 1;
}
