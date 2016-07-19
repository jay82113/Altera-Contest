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

#define UNKNOWN_FLOW_THRESH 1e9

using namespace cv;
using namespace std;


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    cameraTimer = new QTimer(this);


    face_cascade_name = "/home/jay/opencv-2.4.12/data/haarcascades/haarcascade_frontalface_alt.xml";
    eyes_cascade_name = "/home/jay/opencv-2.4.12/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    face_cascade.load( face_cascade_name );
    eyes_cascade.load( eyes_cascade_name );

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


    /*while (true) {
        Ipl_Frame = cvQueryFrame(cap);
    }*/


  //  while(true){
           // if(!cap.read(frame))
         //       break;
          /*  cap.read(frame);
            Mat src = Mat(frame);

            qImg = QCV.CvMat2QImage(src);
            cout << qImg.depth() << ", " << qImg.width() << ", " << qImg.height() << endl;*/



           //  ui->label->setPixmap(QPixmap::fromImage(qImg));

}

void Dialog::videoCap()
{
    double fps;
    QString Height, Width;
    if(cap.read(Frame)){
        src = Mat(Frame);
        src = detectAndDisplay(src);
      /*  cvtColor(Frame, gray, CV_BGR2GRAY);


               if( prevgray.data )
               {
                   calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
                   motionToColor(flow, motion2color);
                   optQimg = QCV.CvMat2QImage(motion2color);
                   ui->label_3->setPixmap(QPixmap::fromImage(optQimg));

               }

               std::swap(prevgray, gray);
*/

        srcQimg = QCV.CvMat2QImage(src);

        frame_t = ((double)cv::getTickCount() - frame_t) / cv::getTickFrequency();

        fps = 1.0 / frame_t;

        frame_t = (double)cv::getTickCount();

        ui->label_2->setNum(fps);
        Height = QString::number(srcQimg.height());
        Width = QString::number(srcQimg.width());

        ui->label_2->setText(ui->label_2->text() + "\nWidth: " + Width + "\nHeight: " + Height);

         ui->label->setPixmap(QPixmap::fromImage(srcQimg));
    }
    else
        cout << "read error" << endl;
}


void Dialog::videoShow()
{

}

cv::Mat Dialog::detectAndDisplay( Mat &frame )
{
   std::vector<Rect> faces;
   Mat frame_gray;

   cvtColor( frame, frame_gray, CV_BGR2GRAY );
   equalizeHist( frame_gray, frame_gray );
   //-- Detect faces
   face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(1, 1) );

   for( int i = 0; i < faces.size(); i++ )
    {
      Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
      ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 0, 255, 0 ), 2, 8, 0 );

      /*Mat faceROI = frame_gray( faces[i] );
      std::vector<Rect> eyes;

      //-- In each face, detect eyes
      eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(.5, .5) );

      for( int j = 0; j < eyes.size(); j++ )
       {
         Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
         int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
         circle( frame, center, radius, Scalar( 255, 0, 0 ), 3, 8, 0 );
       }*/
    }
   return frame;
}

void Dialog::makecolorwheel(vector<Scalar> &colorwheel)
{
    int RY = 15;
    int YG = 6;
    int GC = 4;
    int CB = 11;
    int BM = 13;
    int MR = 6;

    int i;

    for (i = 0; i < RY; i++) colorwheel.push_back(Scalar(255,       255*i/RY,     0));
    for (i = 0; i < YG; i++) colorwheel.push_back(Scalar(255-255*i/YG, 255,       0));
    for (i = 0; i < GC; i++) colorwheel.push_back(Scalar(0,         255,      255*i/GC));
    for (i = 0; i < CB; i++) colorwheel.push_back(Scalar(0,         255-255*i/CB, 255));
    for (i = 0; i < BM; i++) colorwheel.push_back(Scalar(255*i/BM,      0,        255));
    for (i = 0; i < MR; i++) colorwheel.push_back(Scalar(255,       0,        255-255*i/MR));
}

void Dialog::motionToColor(Mat flow, Mat &color)
{
    if (color.empty())
        color.create(flow.rows, flow.cols, CV_8UC3);

    static vector<Scalar> colorwheel; //Scalar r,g,b
    if (colorwheel.empty())
        makecolorwheel(colorwheel);

    // determine motion range:
    float maxrad = -1;

    // Find max flow to normalize fx and fy
    for (int i= 0; i < flow.rows; ++i)
    {
        for (int j = 0; j < flow.cols; ++j)
        {
            Vec2f flow_at_point = flow.at<Vec2f>(i, j);
            float fx = flow_at_point[0];
            float fy = flow_at_point[1];
            if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
                continue;
            float rad = sqrt(fx * fx + fy * fy);
            maxrad = maxrad > rad ? maxrad : rad;
        }
    }

    for (int i= 0; i < flow.rows; ++i)
    {
        for (int j = 0; j < flow.cols; ++j)
        {
            uchar *data = color.data + color.step[0] * i + color.step[1] * j;
            Vec2f flow_at_point = flow.at<Vec2f>(i, j);

            float fx = flow_at_point[0] / maxrad;
            float fy = flow_at_point[1] / maxrad;
            if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
            {
                data[0] = data[1] = data[2] = 0;
                continue;
            }
            float rad = sqrt(fx * fx + fy * fy);

            float angle = atan2(-fy, -fx) / CV_PI;
            float fk = (angle + 1.0) / 2.0 * (colorwheel.size()-1);
            int k0 = (int)fk;
            int k1 = (k0 + 1) % colorwheel.size();
            float f = fk - k0;
            //f = 0; // uncomment to see original color wheel

            for (int b = 0; b < 3; b++)
            {
                float col0 = colorwheel[k0][b] / 255.0;
                float col1 = colorwheel[k1][b] / 255.0;
                float col = (1 - f) * col0 + f * col1;
                if (rad <= 1)
                    col = 1 - rad * (1 - col); // increase saturation with radius
                else
                    col *= .75; // out of range
                data[2 - b] = (int)(255.0 * col);
            }
        }
    }
}

Dialog::~Dialog()
{
    cap.release();
    delete ui;
}
