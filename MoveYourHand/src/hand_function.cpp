#include "hand_function.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;

hand_function::hand_function(Mat bw_img)
{
    int i, bigest_area = 0;
    findContours(bw_img, contours_ob, hierarchy, RETR_TREE , CHAIN_APPROX_SIMPLE, Point(0, 0));
    if (contours_ob.size() > 0)
    {
        for(i = 0; i < contours_ob.size(); i++ )
            if(moments(contours_ob[i]).m00 > bigest_area)
            {
                bigest_area = moments(contours_ob[i]).m00;
                bigest_object = contours_ob[i];
                bigest_index = i;
            }
        hull = vector<vector<Point> >(contours_ob.size());
        hullsI = vector<vector<int> >(contours_ob.size());
        defects = vector<vector<Vec4i> >(contours_ob.size());
    }
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
               convexHull( Mat(contours_ob[i]), hull[i], false );
               convexHull( Mat(contours_ob[i]), hullsI[i], false );
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
        figer.ptStart = Point(contours_ob[bigest_index][start_in]);
        figer.ptEnd = Point(contours_ob[bigest_index][end_in]);
        figer.ptFar = Point(contours_ob[bigest_index][farth_in]);
    }
}

void hand_function::find_figer(vector<hand_function> &fig_vec)
{
    if(contours_ob.size() > 0)
    {
        int i = 1;
        vector<Vec4i>::iterator dec_ptr = defects[bigest_index].begin(); // pointer of first element
        while(dec_ptr != defects[bigest_index].end())
        {
            hand_function tmp;
            set_property(tmp, dec_ptr);
            if(tmp.depth > 20 )
            {
                tmp.figer_number = i;
                fig_vec.push_back(tmp);
                i++;
            }
            dec_ptr++;
        }
    }
}

void hand_function::draw(Mat image, vector<hand_function> fig_vec)
{
    if(contours_ob.size() > 0)
    {
        int i;
        char char_tmp[2];
        drawContours(image, hull, bigest_index, Scalar(0, 0, 255), 3);
        for(i = 0; i < fig_vec.size(); i++)
        {
            hand_function tmp = fig_vec[i];
            sprintf(char_tmp, "%d", tmp.figer_number);
            putText(image, char_tmp, tmp.ptStart, FONT_HERSHEY_PLAIN, 2.0f ,Scalar(0,0,255),2);
            line(image, tmp.ptStart, tmp.ptFar, Scalar(0, 255, 255), 3);
            line(image, tmp.ptEnd, tmp.ptFar, Scalar(0, 255, 255), 3);
            circle(image, tmp.ptStart, 3, 3);
        }
    }
}
