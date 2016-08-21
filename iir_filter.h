#ifndef IIR_FILTER_H
#define IIR_FILTER_H

class IIR_Filter
{
public:
    IIR_Filter();
    void Filter(double &inData, double &outData, bool test=false);
private:
    const int phase = 8;
    double IIR_B[9][3]={{0.298106432,0,0},
                     {1,0,-1},
                     {0.298106432,0,0},
                     {1,0,-1},
                     {0.2641755044,0,0},
                     {1,0,-1},
                     {0.2641755044,0,0},
                     {1,0,-1},
                     {1,0,0}};

    double IIR_A[9][3]={{1,0,0},
                     {1,-1.158835769,0.6663268805},
                     {1,0,0},
                     {1,-1.899261832,0.9205448031},
                     {1,0,0},
                     {1,-1.702393413,0.7348755598},
                     {1,0,0},
                     {1,-1.102208614,0.3633402288 },
                     {1,0,0}};
    double filter_x [10][3];
    double filter_y [10][3];

};

#endif // IIR_FILTER_H
