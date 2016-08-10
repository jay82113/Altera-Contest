#ifndef FACE_TRACKER_H
#define FACE_TRACKER_H

#include "cxcore.h"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/detection_based_tracker.hpp"

using namespace std;

class Face_Tracker
{
public:
    Face_Tracker(string &face_cascade_name ,cv::DetectionBasedTracker::Parameters param);
    bool DetectionRun();
    void setParameters(cv::DetectionBasedTracker::Parameters &param);
    bool findFeatures(cv::Mat &frame, std::vector<cv::Point2f> &point);
    bool OpticalFlow(cv::Mat &frame, std::vector<cv::Point2f> &point);
    void Release();
    void setMAX_COUNT(int num);
    void setPrevPoint(std::vector<cv::Point2f> &point);
    void setPrevGray(cv::Mat &frame);
private:
    cv::DetectionBasedTracker* face_detection;
    int MAX_COUNT;
    cv::Mat prevGray;
    std::vector<cv::Point2f> prevPoint;
    cv::TermCriteria *termcrit;
};

#endif // FACE_TRACKER_H
