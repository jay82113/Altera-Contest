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
#include "opencv2/video/tracking.hpp"


#define UNKNOWN_FLOW_THRESH 1e9

using namespace cv;
using namespace std;

DetectionBasedTracker::Parameters param;
std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
DetectionBasedTracker face_detection(face_cascade_name, param);

vector<Point2f> points[2];
TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    Size subPixWinSize(10,10), winSize(31,31);

    const int MAX_COUNT = 20;
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



   if(!face_detection.run()){
        cout << "face detection not run" << endl;
        return ;
    }


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
  //  Mat gray_src;
  //  QImage gray_srcQimg;
    Point center;

    QString Height, Width;
    if(cap.read(Frame)){
        src = Mat(Frame);
        detectAndDisplay(src, center);
    //    gray_src = detectAndDisplay(src);
    //    gray_srcQimg = QCV.CvMat2QImage(gray_src);

        srcQimg = QCV.CvMat2QImage(src);

        frame_t = ((double)cv::getTickCount() - frame_t) / cv::getTickFrequency();

        fps = 1.0 / frame_t;

        frame_t = (double)cv::getTickCount();

        ui->label_2->setNum(fps);
        Height = QString::number(srcQimg.height());
        Width = QString::number(srcQimg.width());

        ui->label_2->setText(ui->label_2->text() + "\nWidth: " + Width + "\nHeight: " + Height);

  //       ui->label->setPixmap(QPixmap::fromImage(srcQimg));
      //   ui->label_3->setPixmap(QPixmap::fromImage(gray_srcQimg));
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
   Mat frame_gray;
   Mat Mask;


   size_t i, k;
   cvtColor( frame, frame_gray, CV_BGR2GRAY );



   if(!needToInit){
       face_detection.process(frame_gray);
       face_detection.getObjects(faces);
       if(!faces.empty()){
           Mask = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
           Rect MaskRect(faces[0].x+40, faces[0].y+40, faces[0].width-80, faces[0].height-80);
           rectangle(Mask,MaskRect,Scalar(255, 255, 255),-1);
         //  QImage MaskQImg;
        //   MaskQImg = QCV.CvMat2QImage(Mask);
        //   ui->label_3->setPixmap(QPixmap::fromImage(MaskQImg));
       }
   }

   if( !needToInit && !faces.empty())
           {
               // automatic initialization
               goodFeaturesToTrack(frame_gray, points[1], MAX_COUNT, 0.01, 10, Mask, 3, 0, 0.04);
               //cout << points[1].size() << endl;
            //   cornerSubPix(frame_gray, points[1], subPixWinSize, Size(-1,-1), termcrit);

               for( i = k = 0; i < points[1].size(); i++ )
               {


               //    if( !status[i] )
               //        continue;

               //    points[1][k++] = points[1][i];
                   circle( frame, points[1][i], 3, Scalar(0,255,0), -1, 8);
               }

               needToInit = true;
               face_detection.resetTracking();
           }

   else if( !points[0].empty() )
           {
               vector<uchar> status;
               vector<float> err;
               if(prevGray.empty())
                   frame_gray.copyTo(prevGray);
               calcOpticalFlowPyrLK(prevGray, frame_gray, points[0], points[1], status, err, winSize,
                                    3, termcrit, 0, 0.001);
               size_t i, k;
               for( i = k = 0; i < points[1].size(); i++ )
               {


                   if( !status[i] )
                       continue;

                   points[1][k++] = points[1][i];
                   circle( frame, points[1][i], 3, Scalar(0,255,0), -1, 8);
               }
               points[1].resize(k);
           }

   std::swap(points[1], points[0]);
   cv::swap(prevGray, frame_gray);



   /*for( int i = 0; i < faces.size(); i++ )
    {

      Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
      ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 0, 255, 0 ), 2, 8, 0 );


    }*/
   return frame_gray;
}


Dialog::~Dialog()
{
    cap.release();
    delete ui;
}
