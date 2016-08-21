#ifndef PPGFILTER_NOMOVE_H
#define PPGFILTER_NOMOVE_H
#include <QVector>
#include "iir_filter.h"

class PPGFilter_nomove
{
public:
    PPGFilter_nomove();
    double PPG_Filter(double, double, double, double);


private:

    IIR_Filter filter_G;
    IIR_Filter filter_R;
    IIR_Filter filter_GRD;

    double IIR_G;
    double IIR_R;



    double GRDbase;
    double alphaGbetaG;
    double alphaRbeta_R;
    double filtered_PPG;
    double filtered_R;
    double GRD_out;
    double GRD_IIR;

    int Count ;


    //Moving AveragCounte Parameter
    int moveavgcnt ;
    double movavgout ;
    int movingwidowsize ;
    double movavgsum;
    QVector<double> movingbuf;


    // Adaptive Filter Parameter
    int N ;
    double W;
    double E;
    double beta ;
    double Adaptive_Output;
};

#endif // PPGFILTER_NOMOVE_H
