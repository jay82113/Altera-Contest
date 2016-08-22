#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "opencv2/opencv.hpp"
#include "switch.h"
#include "highgui.h"

#include "cxcore.h"
#include "cv.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/detection_based_tracker.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "stepdetection.h"
#include "ppgfilter.h"
#include "iir_filter.h"
#include "ppg_detection.h"
#include "ppgfilter_nomove.h"

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
    ConvertQtOpenCV QCV;
    double frame_t;
    StepDetection step;
    PPGFilter HRFilter;
    PPGFilter_nomove HR_nomoveFilter;
    IIR_Filter Filter_x;
    IIR_Filter Filter_y;
    PPG_Detection HR_Detection;
    ~Dialog();

public slots:
    void realtimeDataSlot(cv::Point);
    void realtimePPGSlot(double, double, double, double,  bool);
    void videoShow(double, double, double, double,  bool);


signals:
    void FindPoint(cv::Point);
    void FindROI(double, double, double, double, bool);
    void Switch_fun(double, double, double, double,  bool);

private:
    Ui::Dialog *ui;
private slots:
    void videoCap();

    cv::Mat detectAndDisplay( cv::Mat &frame , Point &center);
};

#endif // DIALOG_H
