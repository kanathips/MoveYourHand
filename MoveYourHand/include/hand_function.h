#ifndef HAND_FUNCTION_H
#define HAND_FUNCTION_H
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class hand_function
{
    public:
        Rect boundRect;
        hand_function();
        hand_function(Mat bw_img);
        Point start;
        Point end;
        Point farth;
        float depth;
        float angle;
        int bigest_index;
        int figer_number;
        vector<Point> bigest_object;
        vector<vector<Point> > contours_ob;
        void set_property(hand_function &figer , vector<Vec4i>::iterator de_ptr);
        void find_hull();
        void draw(Mat image, vector<hand_function> fig_vec, vector<Point> finger_tip);
        void find_figer(vector<hand_function> &fig_vec, vector<Point> &finger_tip);
    private:
        vector<Vec4i> hierarchy;
		vector<vector<int> > hullsI;
		vector<vector<Vec4i> > defects;
        vector<vector<Point> >hull;
        float distance_point(Point point_1, Point point_2);
        float angle_point(Point start, Point farth, Point end);
        void show_text(Mat img, string text, Point posi);

};

#endif // HAND_FUNCTION_H
