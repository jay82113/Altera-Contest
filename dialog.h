#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "opencv2/opencv.hpp"
#include "switch.h"
#include "highgui.h"

#include "cxcore.h"
#include "cv.h"

using namespace cv;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    cv::VideoCapture cap;
    cv::Mat Frame, src, prevgray, gray, flow, cflow, motion2color;
    IplImage* Ipl_Frame;
    QImage srcQimg, optQimg;
    QTimer *cameraTimer;
    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eyes_cascade;
    ConvertQtOpenCV QCV;
    double frame_t;
    ~Dialog();

private:
    Ui::Dialog *ui;
    std::string face_cascade_name;
    std::string eyes_cascade_name;
private slots:
    void videoCap();
    void videoShow();
    void motionToColor(Mat flow, Mat &color);
    void makecolorwheel(vector<Scalar> &colorwheel);
    cv::Mat detectAndDisplay( cv::Mat &frame );
};

#endif // DIALOG_H
