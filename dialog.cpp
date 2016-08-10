#include "dialog.h"
#include "ui_dialog.h"
#include "switch.h"
#include "face_tracker.h"
#include "QtGui"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/detection_based_tracker.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/video/tracking.hpp"


#define UNKNOWN_FLOW_THRESH 1e9

using namespace cv;
using namespace std;

DetectionBasedTracker::Parameters param;
std::string face_cascade_name = "haarcascade_mcs_nose.xml";
//DetectionBasedTracker face_detection(face_cascade_name, param);
DetectionBasedTracker* face_detection = new DetectionBasedTracker(face_cascade_name, param);

Face_Tracker* face_tracker;

vector<Point2f> points[2];
//TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
TermCriteria *termcrit = new TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    Size subPixWinSize(10,10), winSize(40,40);

    const int MAX_COUNT = 5;
    bool needToInit = false;
    bool nightMode = false;
    Mat prevGray;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    cameraTimer = new QTimer(this);

   // param.maxObjectSize = 400;
    param.minObjectSize = 30;
   // param.maxTrackLifetime = 20;
   // param.minDetectionPeriod = 7;
   // param.minNeighbors = 3;

    face_tracker = new Face_Tracker(face_cascade_name, param);

    if(!face_tracker->DetectionRun()){
        cout << "face detection not run" << endl;
        return ;
    }

 /*  if(!face_detection->run()){
        cout << "face detection not run" << endl;
        return ;
    }*/


    cap.open(0);
    if(!cap.isOpened()){
            cout << "can not open camera" << endl;
            return ;
        }
    else
        cout << "camera is opened" << endl;

    frame_t = (double)cv::getTickCount();
    cameraTimer->start(56);

    connect(cameraTimer, SIGNAL(timeout()), this, SLOT(videoCap()));


}

void Dialog::videoCap()
{
    double fps;
   // Mat gray_src;
   // QImage gray_srcQimg;
    Point center;
    Mat src_resize;
    float scale = 0.5;
    CvSize Frame_size;

    QString Height, Width;
    if(cap.read(Frame)){
        src = Mat(Frame);


        detectAndDisplay(src, center);
  //      gray_src = detectAndDisplay(src, center);
  //      gray_srcQimg = QCV.CvMat2QImage(gray_src);


        srcQimg = QCV.CvMat2QImage(src);

        frame_t = ((double)cv::getTickCount() - frame_t) / cv::getTickFrequency();

        fps = 1.0 / frame_t;

        frame_t = (double)cv::getTickCount();

        ui->label_2->setNum(fps);
        Height = QString::number(srcQimg.height());
        Width = QString::number(srcQimg.width());

        ui->label_2->setText(ui->label_2->text() + "\nWidth: " + Width + "\nHeight: " + Height);

         ui->label->setPixmap(QPixmap::fromImage(srcQimg));
 //        ui->label_3->setPixmap(QPixmap::fromImage(gray_srcQimg));
    }
    else
        cout << "read error" << endl;
}


void Dialog::videoShow()
{

}


cv::Mat Dialog::detectAndDisplay( Mat &frame, Point &center )
{
   std::vector<Rect> faces;
   Rect ROI;
   Mat frame_gray;
   Mat Mask;
   Mat src_resize;
   float scale = 0.2;
   CvSize Frame_size;

   static int center_num = 2;

   Mat src = Mat(frame);

   size_t i, k;
   cvtColor( frame, frame_gray, CV_BGR2GRAY );


   if(!needToInit){

      needToInit = face_tracker->findFeatures(frame, points[0]);
      if(needToInit){
          for(int i=0; i < points[0].size(); i++){
                     points[0][i].x = round(points[0][i].x * scale);
                     points[0][i].y = round(points[0][i].y * scale);
                 }
          face_tracker->setPrevPoint(points[0]);
          Frame_size.height = src.rows * scale;
          Frame_size.width = src.cols * scale;
          cv::resize(src, src_resize, Frame_size);
          face_tracker->setPrevGray(src_resize);
          face_tracker->Release();
      }

   }
   else{
       Frame_size.height = src.rows * scale;
       Frame_size.width = src.cols * scale;
       cv::resize(src, src_resize, Frame_size);


       face_tracker->OpticalFlow(src_resize, points[0]);

       points[1].assign(points[0].begin(), points[0].end());

       for(int i=0; i < points[1].size(); i++){
                  points[1][i].x = round(points[1][i].x / scale);
                  points[1][i].y = round(points[1][i].y / scale);
              }

       ROI.x = points[1][center_num].x - 10;
       ROI.y = points[1][center_num].y - 10;
       ROI.height = 20;
       ROI.width = 20;

       rectangle(frame,ROI,Scalar(0, 200, 0));

     //  for( i = k = 0; i < points[1].size(); i++ )
     //      circle( frame, points[1][i], 1, Scalar(0,255,0), -1, 8);



   }


   return frame_gray;
}


Dialog::~Dialog()
{
    cap.release();
    delete ui;
}
