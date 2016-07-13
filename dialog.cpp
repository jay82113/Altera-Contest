#include "dialog.h"
#include "ui_dialog.h"
#include "switch.h"
#include "QtGui"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



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

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

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
    //    src = detectAndDisplay(src);
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

      Mat faceROI = frame_gray( faces[i] );
      std::vector<Rect> eyes;

      //-- In each face, detect eyes
      eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(.5, .5) );

      for( int j = 0; j < eyes.size(); j++ )
       {
         Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
         int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
         circle( frame, center, radius, Scalar( 255, 0, 0 ), 3, 8, 0 );
       }
    }
   return frame;
}

Dialog::~Dialog()
{
    delete ui;
}
