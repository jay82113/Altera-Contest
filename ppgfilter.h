#ifndef PPGFILTER_H
#define PPGFILTER_H
#include <QVector>

class PPGFilter
{
public:
    PPGFilter();
    double PPG_Filter(double, double, double, double);


private:


    double GRDbase;
    double alphaGbetaG;
    double alphaRbeta_R;
    double filtered_PPG;
    double filtered_R;
    double GRD_out;

    int Count ;
    //IIR Filter Parameters
    int phase ;

    double IIR_B[9][3]={{0.4242803156,0,0},
                     {1,0,-1},
                     {0.4242803156,0,0},
                     {1,0,-1},
                     {0.3654437661,0,0},
                     {1,0,-1},
                     {0.3654437661,0,0},
                     {1,0,-1},
                     {1,0,0}};

    double IIR_A[9][3]={{1,0,0},
                     {1,-0.4170512855,0.5992619991},
                     {1,0,0},
                     {1,-1.751938462,0.8476499319},
                     {1,0,0},
                     {1,-1.39305377,0.5316658616},
                     {1,0,0},
                     {1,-0.6177121401,0.2233224511},
                     {1,0,0}};

    double filter_x [10][3];
    double filter_y [10][3];
    double filter_x2 [10][3];
    double filter_y2 [10][3];
    double filter_x_R [10][3];
    double filter_y_R [10][3];

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
