#include "dialog.h"
#include "ui_dialog.h"
#include "switch.h"
#include "QtGui"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/detection_based_tracker.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/video/video.hpp"

#define UNKNOWN_FLOW_THRESH 1e9

using namespace cv;
using namespace std;

DetectionBasedTracker::Parameters param;
std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
DetectionBasedTracker face_detection(face_cascade_name, param);

int history = 10;
double varThreshold = 16;
bool detectShadows = false ;

bool update_bg_model = true;

bool Need_to_Init = true;

BackgroundSubtractorMOG pMOG2(20, 10, 0.5, false);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    cameraTimer = new QTimer(this);

    param.maxObjectSize = 400;
    param.minObjectSize = 20;
   // param.maxTrackLifetime = 20;
   // param.minDetectionPeriod = 7;
   // param.minNeighbors = 3;

    cout << "maxObjectSize = " << param.maxObjectSize << endl;
    cout << "minObjectSize = " << param.minObjectSize <<endl;
    cout << "maxTrackLifetime = " << param.maxTrackLifetime << endl;
    cout << "minDetectionPeriod = " << param.minDetectionPeriod << endl;
    cout << "minNeighbors = " << param.minNeighbors << endl;
    cout << "scaleFactor = " << param.scaleFactor << endl;

    face_detection.setParameters(param);

    eyes_cascade_name = "/home/jay/opencv-2.4.12/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    face_cascade.load( face_cascade_name );
    eyes_cascade.load( eyes_cascade_name );



   /* if(!face_detection.run()){
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
    cameraTimer->start(33);

    connect(cameraTimer, SIGNAL(timeout()), this, SLOT(videoCap()));


}

void Dialog::videoCap()
{
    double fps;
    Mat gray_src;
    QImage gray_srcQimg;
    Point center;

    QString Height, Width;
    if(cap.read(Frame)){
        src = Mat(Frame);
    //    detectAndDisplay(src, center);
        gray_src = detectAndDisplay(src, center);
        gray_srcQimg = QCV.CvMat2QImage(gray_src);

        srcQimg = QCV.CvMat2QImage(src);

        frame_t = ((double)cv::getTickCount() - frame_t) / cv::getTickFrequency();

        fps = 1.0 / frame_t;

        frame_t = (double)cv::getTickCount();

        ui->label_2->setNum(fps);
        Height = QString::number(srcQimg.height());
        Width = QString::number(srcQimg.width());

        ui->label_2->setText(ui->label_2->text() + "\nWidth: " + Width + "\nHeight: " + Height);

         ui->label->setPixmap(QPixmap::fromImage(srcQimg));
         ui->label_3->setPixmap(QPixmap::fromImage(gray_srcQimg));
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
   Mat frame_gray, bgimg;
   static Rect trackWindow;
  static Mat pev_frame;

  /* if(Need_to_Init && faces.empty()){
       cvtColor( frame, frame_gray, CV_BGR2GRAY );

       face_detection.process(frame_gray);
       face_detection.getObjects(faces);
       face_detection.stop();
       Need_to_Init = false;
   }
   else{

   }*/

    pMOG2(frame, frame_gray, update_bg_model ? -1 : 0);
  /* if(Need_to_Init){
       pev_frame = frame;
       Need_to_Init = false;
   }
   else{
       cv::absdiff(frame, pev_frame, frame_gray);
       pev_frame = frame;
   }*/

  /* Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
   morphologyEx(frame_gray,frame_gray, MORPH_OPEN, element);
   morphologyEx(frame_gray,frame_gray, MORPH_CLOSE, element);*/

   //pMOG2.getBackgroundImage(bgimg);

 /*  float scale = 0.25;
   CvSize Frame_size;
   Frame_size.height = frame.rows * scale;
   Frame_size.width = frame.cols * scale;

   cvtColor( frame, frame_gray, CV_BGR2GRAY );

   cv::resize(frame_gray, frame_gray, Frame_size);

   face_detection.process(frame_gray);
   face_detection.getObjects(faces);



   for( int i = 0; i < faces.size(); i++ )
    {
      faces[i].x = faces[i].x / scale;
      faces[i].y = faces[i].y / scale;
      faces[i].width = faces[i].width / scale;
      faces[i].height = faces[i].height / scale;
      center.x = faces[i].x + faces[i].width*0.5;
      center.y = faces[i].y + faces[i].height*0.5;
     // Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
     // ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 0, 255, 0 ), 2, 8, 0 );


    }*/

   return frame_gray;
}


Dialog::~Dialog()
{
    cap.release();
    delete ui;
}
