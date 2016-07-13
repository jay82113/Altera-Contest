#ifndef SWITCH_H
#define SWITCH_H

#include "highgui.h"

#include "cxcore.h"
#include "cv.h"
//#include "opencv2/opencv.hpp"

#include <QImage>


#define RGB_TYPE  3
#define GRAY_TYPE 1

class ImageCVtoQT
{
public:
    ImageCVtoQT(IplImage *_srcImage);
    ~ImageCVtoQT();
    const QImage getQtImage(void);
private:
    IplImage *srcImage;

    //QImage desImage;
    int width;
    int height;
    int channel;
};

class ImageQTtoCV
{
public:
    ImageQTtoCV(QImage _srcImage);
    ~ImageQTtoCV();
    IplImage *getCvImage(void);
private:
    QImage srcImage;
    int width;
    int height;
    //int channel;
};

class ConvertQtOpenCV
{
public:
    ConvertQtOpenCV();
    ~ConvertQtOpenCV();
    QImage CvMat2QImage( cv::Mat &image );
    QImage IplImage2QImage( IplImage *image );
    cv::Mat QImage2CvMat(QImage &image);
    IplImage *QImage2IplImage(QImage &image);
private:

};


#endif // SWITCH_H
