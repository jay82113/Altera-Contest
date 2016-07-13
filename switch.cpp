#include "switch.h"

ImageCVtoQT::ImageCVtoQT(IplImage *_srcImage)
    :srcImage(_srcImage)
{
    assert(srcImage != NULL);
    width = srcImage -> width;
    height = srcImage -> height;
    channel = srcImage -> nChannels;
}

ImageCVtoQT::~ImageCVtoQT()
{
    cvReleaseImage(&srcImage);
}

const QImage ImageCVtoQT::getQtImage()
{
    QImage desImage = QImage(width, height, QImage::Format_RGB32);
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int r,g,b;
            if(RGB_TYPE == channel)
            {
                b = (int)CV_IMAGE_ELEM(srcImage, uchar , i, j*3+0);
                g = (int)CV_IMAGE_ELEM(srcImage, uchar , i, j*3+1);
                r = (int)CV_IMAGE_ELEM(srcImage, uchar , i, j*3+2);
            }
            else if(GRAY_TYPE == channel)
            {
                b = (int)CV_IMAGE_ELEM(srcImage, uchar, i, j);
                g = b;
                r = b;
            }
            desImage.setPixel(j, i, qRgb(r, g, b));
        }
    }
    return desImage;
}
/////////////////////////////////////////////////////////////////////

ImageQTtoCV::ImageQTtoCV(QImage _srcImage)
:srcImage(_srcImage)
{
    assert(!srcImage.isNull());
    width=srcImage.width();
    height=srcImage.height();

}
ImageQTtoCV::~ImageQTtoCV()
{
}

IplImage *ImageQTtoCV::getCvImage()
{
    IplImage *desImage=cvCreateImage(cvSize(width,height),8,3);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            QRgb rgb=srcImage.pixel(j,i);
            CV_IMAGE_ELEM(desImage,uchar,i,j*3+0)=qBlue(rgb);
            CV_IMAGE_ELEM(desImage,uchar,i,j*3+1)=qGreen(rgb);
            CV_IMAGE_ELEM(desImage,uchar,i,j*3+2)=qRed(rgb);
        }
    }
    return desImage;
}
/////////////////////////////////////////////////////////////////////

ConvertQtOpenCV::ConvertQtOpenCV()
{

}

ConvertQtOpenCV::~ConvertQtOpenCV()
{

}

QImage ConvertQtOpenCV::CvMat2QImage( cv::Mat &image )
{
    QImage qtImg;
    if( !image.empty() && image.depth() == CV_8U ){
        if(image.channels() == 1){
            qtImg = QImage( (const unsigned char *)(image.data),
                            image.cols,
                            image.rows,
                            QImage::Format_Indexed8 );
        }
        else{
            cvtColor( image, image, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(image.data),
                            image.cols,
                            image.rows,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg;
}

QImage ConvertQtOpenCV::IplImage2QImage( IplImage *image )
{
    QImage qtImg;
    if (image && image->depth == IPL_DEPTH_8U && cvGetSize(image).width > 0 )
    {
        if(image->nChannels == 1){
            qtImg = QImage( (const unsigned char *)(image->imageData),
                            image->width,
                            image->height,
                            QImage::Format_Indexed8 );
        }
        else{
            cvCvtColor( image, image, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(image->imageData),
                            image->width,
                            image->height,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg;
}

cv::Mat ConvertQtOpenCV::QImage2CvMat(QImage &image)
{
    cv::Mat cvImage;
    switch (image.format()){
    case QImage::Format_RGB888:{
        cvImage = cv::Mat(image.height(),
                          image.width(),
                          CV_8UC3,
                          image.bits(),
                          image.bytesPerLine());
        cv::cvtColor(cvImage, cvImage, CV_RGB2BGR);
        return cvImage;
    }
    case QImage::Format_Indexed8:{
        cvImage = cv::Mat(image.height(),
                          image.width(),
                          CV_8U,
                          image.bits(),
                          image.bytesPerLine());
        return cvImage;
    }
    default:
        break;
    }
    return cvImage;
}

IplImage *ConvertQtOpenCV::QImage2IplImage(QImage &image)
{
    IplImage *cvImage = NULL;
    switch (image.format()){
    case QImage::Format_RGB888:{
        cvImage = cvCreateImageHeader( cvSize(image.width(),
                                       image.height()), 8, 3 );
        cvSetData( cvImage, image.bits(), image.bytesPerLine() );
        cvCvtColor( cvImage, cvImage, CV_RGB2BGR );
        return cvImage;
    }
    case QImage::Format_Indexed8:{
        cvImage = cvCreateImageHeader( cvSize(image.width(),
                                       image.height()), 8, 1 );
        cvSetData( cvImage, image.bits(), image.bytesPerLine() );
        return cvImage;
    }
    default:
        break;
    }
    return cvImage;
}
