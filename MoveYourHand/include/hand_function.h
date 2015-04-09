#ifndef HAND_FUNCTION_H
#define HAND_FUNCTION_H
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class hand_function
{
    public:
        hand_function();
        hand_function(Mat bw_img);
        Point ptStart;
        Point ptEnd;
        Point ptFar;
        float depth;
        int bigest_index;
        int figer_number;
        vector<Point> bigest_object;
        vector<vector<Point> > contours_ob;
        void set_property(hand_function &figer , vector<Vec4i>::iterator de_ptr);
        void find_hull();
        void draw(Mat image, vector<hand_function> fig_vec);
        void find_figer(vector<hand_function> &fig_vec);
    private:
        vector<Vec4i> hierarchy;
		vector<vector<int> > hullsI;
		vector<vector<Vec4i> > defects;
        vector<vector<Point> >hull;
};

#endif // HAND_FUNCTION_H
