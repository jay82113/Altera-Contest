#include "face_tracker.h"

using namespace cv;
using namespace std;

Face_Tracker:: Face_Tracker(string &face_cascade_name ,cv::DetectionBasedTracker::Parameters param)
{
    face_detection = new DetectionBasedTracker(face_cascade_name, param);
    termcrit = new TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    MAX_COUNT = 5;
}

void Face_Tracker:: setParameters(cv::DetectionBasedTracker::Parameters &param)
{
    face_detection->setParameters(param);
}

bool Face_Tracker::DetectionRun()
{
    return face_detection->run();
}

bool Face_Tracker:: findFeatures(cv::Mat &frame, std::vector<cv::Point2f> &point)
{
    Mat frame_gray;
    std::vector<Rect> faces;
    Mat Mask;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    face_detection->process(frame_gray);
    face_detection->getObjects(faces);

    if(!faces.empty()){
        Mask = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
        Rect MaskRect(faces[0].x+65, faces[0].y, faces[0].width/2, faces[0].height/3);
        rectangle(Mask,MaskRect,Scalar(255, 255, 255),-1);

        goodFeaturesToTrack(frame_gray, point, MAX_COUNT, 0.01, 10, Mask, 3, 0, 0.04);
        prevPoint.assign(point.begin(), point.end());
        frame_gray.copyTo(prevGray);
        return true;
    }
    else
        return false;

}

bool Face_Tracker:: OpticalFlow(cv::Mat &frame, std::vector<cv::Point2f> &point)
{
    Mat frame_gray;
    Size winSize(20, 20);


    if(!prevPoint.empty()){
        vector<uchar> status;
        vector<float> err;

        cvtColor(frame, frame_gray, CV_BGR2GRAY);

        calcOpticalFlowPyrLK(prevGray, frame_gray, prevPoint, point, status, err, winSize,
                             3, *termcrit, 0, 0.001);

        prevPoint.assign(point.begin(), point.end());
        frame_gray.copyTo(prevGray);

    }
}

void Face_Tracker::setPrevPoint(std::vector<cv::Point2f> &point)
{
    prevPoint.assign(point.begin(), point.end());
}

void Face_Tracker::setPrevGray(cv::Mat &frame)
{
    Mat frame_gray;
    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    frame_gray.copyTo(prevGray);
}

void Face_Tracker:: Release()
{
    face_detection->stop();
    delete face_detection;
}

void Face_Tracker:: setMAX_COUNT(int num)
{
    MAX_COUNT = num;
}
