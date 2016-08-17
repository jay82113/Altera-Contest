#ifndef PPGFILTER_H
#define PPGFILTER_H
#include <QVector>
#include "iir_filter.h"

class PPGFilter
{
public:
    PPGFilter();
    double PPG_Filter(double, double, double, double);


private:

    IIR_Filter filter_G;
    IIR_Filter filter_R;

    double IIR_G;
    double IIR_R;


    double GRDbase;
    double alphaGbetaG;
    double alphaRbeta_R;
    double filtered_PPG;
    double filtered_R;
    double GRD_out;

    int Count ;


    //Moving AveragCounte Parameter
    int moveavgcnt ;
    int movavgout ;
    int movingwidowsize ;
    QVector<double> movingbuf;


    // Adaptive Filter Parameter
    int N ;
    double W;
    double E;
    double beta ;
    double Adaptive_Output;


};

#endif // PPGFILTER_H
