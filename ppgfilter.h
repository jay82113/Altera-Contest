#ifndef PPGFILTER_H
#define PPGFILTER_H
#include <QVector>
#include "iir_filter.h"

class PPGFilter
{
public:
    PPGFilter();
    double PPG_Filter(double Raw_R, double Raw_G, double Raw_B, double);


private:

    IIR_Filter filter_Xs;
    IIR_Filter filter_Ys;

    IIR_Filter filter_G;
    IIR_Filter filter_R;
    IIR_Filter filter_B;
    IIR_Filter filter_Pulse;

    double IIR_G;
    double IIR_R;
    double IIR_B;


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
    std::vector <double> Xs_buf;
    std::vector <double> Ys_buf;
    std::vector <double> R_buf;
    std::vector <double> G_buf;
    std::vector <double> B_buf;
    double buf_count;

    const int buf_size = 32;


    // Adaptive Filter Parameter
    int N ;
    double W;
    double E;
    double beta ;
    double Adaptive_Output;


};

#endif // PPGFILTER_H
