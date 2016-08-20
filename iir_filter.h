#ifndef IIR_FILTER_H
#define IIR_FILTER_H

class IIR_Filter
{
public:
    IIR_Filter();
    void Filter(double &inData, double &outData, bool test=false);
private:
    const int phase = 8;
    double IIR_B[9][3]={{0.2625637949,0,0},
                     {1,0,-1},
                     {0.2625637949,0,0},
                     {1,0,-1},
                     {0.2351121157,0,0},
                     {1,0,-1},
                     {0.2351121157,0,0},
                     {1,0,-1},
                     {1,0,0}};

    double IIR_A[9][3]={{1,0,0},
                     {1,-1.305454493,0.6986559629},
                     {1,0,0},
                     {1,-1.914948583,0.9310349226},
                     {1,0,0},
                     {1,-1.741175771,0.7661800981},
                     {1,0,0},
                     {1,-1.219252348,0.4220845699},
                     {1,0,0}};
    double filter_x [10][3];
    double filter_y [10][3];

};

#endif // IIR_FILTER_H
