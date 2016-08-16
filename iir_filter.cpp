#include "iir_filter.h"

IIR_Filter::IIR_Filter()
{
    for(int i=0;i<10;i++)
    {
        for(int j =0;j<3;j++)
        {
            filter_x [i][j]=0;
            filter_y [i][j]=0;
        }
    }

}


void IIR_Filter::Filter(double &inData, double &outData)
{
    filter_x[0][0] = inData;

    for(int i=0; i<phase+1; i++)
    {
        filter_y[i][0]= (filter_x[i][0]*IIR_B[i][0]) +(filter_x[i][1]*IIR_B[i][1]) + (filter_x[i][2]*IIR_B[i][2]) - (filter_y[i][1]*IIR_A[i][1])- (filter_y[i][2]*IIR_A[i][2]);
        filter_y[i][0] = filter_y[i][0] / IIR_A[i][0];

        filter_y[i][2]=filter_y[i][1];
        filter_y[i][1]=filter_y[i][0];
        filter_x[i][2]=filter_x[i][1];
        filter_x[i][1]=filter_x[i][0];
        filter_x[i+1][0] = filter_y[i][0];
    }

    outData = filter_x[phase+1][0];

}
