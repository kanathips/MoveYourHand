#include "hand_function.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;

hand_function::hand_function(Mat bw_img)
{
    int i, bigest_area = 0;
    findContours(bw_img, contours_ob, RETR_TREE , CHAIN_APPROX_SIMPLE, Point(0, 0));
    if (contours_ob.size() > 0)
    {
        for(i = 0; i < contours_ob.size(); i++ )
            if(moments(contours_ob[i]).m00 > bigest_area)
            {
                bigest_area = moments(contours_ob[i]).m00;
                bigest_object = contours_ob[i];
                bigest_index = i;
            }
        boundRect = boundingRect(Mat(contours_ob[bigest_index]));
        hull = vector<vector<Point> >(contours_ob.size());
        hullsI = vector<vector<int> >(contours_ob.size());
        defects = vector<vector<Vec4i> >(contours_ob.size());
    }
}

float hand_function::distance_point(Point point_1, Point point_2)
{
    float ans = sqrt(fabs(pow(point_2.x - point_1.x, 2) + pow(point_2.y - point_1.y, 2)));
    return ans;
}

float hand_function::angle_point(Point start, Point farth, Point end)
{
    /* Cosine's law */
	float dis_1, dis_2, dis_3;
	dis_1 = distance_point(farth, start);
	dis_2 = distance_point(farth, end);
	dis_3 = (start.x - farth.x)*(end.x - farth.x) + (start.y - farth.y)*(end.y - farth.y);
	return acos(dis_3 / (dis_1 * dis_2)) * 180 / (22/7);
}

void hand_function::find_hull()
{
    int i;
    if (contours_ob.size() > 0)
    {
        try
		{
            for(i = 0; i < contours_ob.size(); i++ )
            {
               convexHull( Mat(contours_ob[i]), hull[i], false , false);
               convexHull( Mat(contours_ob[i]), hullsI[i], false , false);
               convexityDefects(Mat(contours_ob[i]),hullsI[i], defects[i]);
            }
        }
		catch(Exception ex)
		{
			    //will fix some bug
		}
    }
}

hand_function::hand_function()
{

}

void hand_function::set_property(hand_function &figer, vector<Vec4i>::iterator de_ptr)
{
    if(contours_ob.size() > 0)
    {
        Vec4i &v = (*de_ptr); // assign address of first element to temp variable
        int start_in =v[0];
        int end_in=v[1];
        int farth_in=v[2];
        figer.depth = v[3] / 256;
        figer.start = Point(contours_ob[bigest_index][start_in]);
        figer.end = Point(contours_ob[bigest_index][end_in]);
        figer.farth = Point(contours_ob[bigest_index][farth_in]);
        figer.angle = angle_point(figer.start, figer.farth, figer.end);
    }
}

void hand_function::find_figer(vector<hand_function> &fig_vec, vector<Point> &finger_tip)
{
    if(contours_ob.size() > 0)
    {
        int i = 1;
        finger_tip.clear();
        vector<Vec4i>::iterator dec_ptr = defects[bigest_index].begin(); // pointer of first element
        while(dec_ptr != defects[bigest_index].end())
        {
            hand_function tmp;
            set_property(tmp, dec_ptr);
            if((distance_point(tmp.farth, tmp.start) > boundRect.height / 5 || distance_point(tmp.farth, tmp.end) > boundRect.height / 5) && tmp.start.y < boundRect.y + (boundRect.height / 1.5))
               fig_vec.push_back(tmp);
            dec_ptr++;
        }
    }
}

void hand_function::show_text(Mat img, string text, Point posi)
{
	putText(img, text, posi, FONT_HERSHEY_PLAIN, 2.0f,Scalar(0,0,255),2);
}

void hand_function::draw(Mat image, vector<hand_function> fig_vec, vector<Point> finger_tip)
{
    if(contours_ob.size() > 0)
    {
        int i;
        char char_tmp[10];
        drawContours(image, hull, bigest_index, Scalar(0, 0, 255), 3);
        for(i = 0; i < fig_vec.size(); i++)
        {
            hand_function tmp = fig_vec[i];
            circle(image, tmp.start, 3, Scalar(255, 0, 0), 3);
        }
        rectangle(image, boundRect, Scalar(255, 0, 0), 3);
        char tmp[3];
        sprintf(tmp, "%d", fig_vec.size());
        show_text(image, tmp, Point(20, 60));

    }
}
