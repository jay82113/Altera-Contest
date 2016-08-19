#ifndef IIR_FILTER_H
#define IIR_FILTER_H

class IIR_Filter
{
public:
    IIR_Filter();
    void Filter(double &inData, double &outData, bool test=false);
private:
    const int phase = 8;
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

};

#endif // IIR_FILTER_H
