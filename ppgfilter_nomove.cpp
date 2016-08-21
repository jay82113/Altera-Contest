#include "ppgfilter_nomove.h"
#include "qmath.h"
#include "iir_filter.h"

PPGFilter_nomove::PPGFilter_nomove()
{
    GRDbase=0;
         alphaGbetaG=0;
         alphaRbeta_R=0;
         filtered_PPG=0;
         filtered_R=0;
         GRD_out=0;

         Count = 0;
        //IIR Filter Parameters


        //Moving AveragCounte Parameter
         moveavgcnt = 0;
         movavgout = 0;
         movingwidowsize = 10;
         movavgsum = 0;
        // Adaptive Filter Parameter
         N = 1;
         beta = 0.00001;
         E = 0;
         W = 0;
         Adaptive_Output = 0;

         movingbuf.resize(movingwidowsize);
}

double PPGFilter_nomove::PPG_Filter(double RawR, double RawG, double RawB,double RawY)
{
    // Moving Average
         // GRD Parameter

         GRDbase = qSqrt((RawR*RawR)+(RawG*RawG)+(RawB*RawB));
         alphaGbetaG = RawG / GRDbase;
         alphaRbeta_R = RawR / GRDbase;

         // IIR Filter

         filter_G.Filter(RawG, IIR_G);
         filter_R.Filter(RawR, IIR_R);

         filtered_PPG=(IIR_G/alphaGbetaG);
         filtered_R=(IIR_R/alphaRbeta_R);
         GRD_out= (filtered_PPG - filtered_R)*20;

         filter_GRD.Filter(GRD_out, GRD_IIR);

         //Moving Average
         if (moveavgcnt < movingwidowsize )
         {
             movingbuf[moveavgcnt]=GRD_IIR;
             moveavgcnt++;
         }
         else
         {
                for(int j=0;j< movingwidowsize-1;j++)
                {
                    movingbuf[j] = movingbuf[j+1];
                }
                movingbuf[movingwidowsize-1] = GRD_IIR;

         }
         for(int k=0;k<movingwidowsize;k++)
         {
             movavgsum += movingbuf[k];
         }

         movavgout=movavgsum/moveavgcnt;
         movavgsum = 0;

         //Adaptive Filter

         Adaptive_Output = W * RawY; // approximate niose
         E = movavgout - Adaptive_Output; // Desire signal
         W=W + (beta*E*RawY);

         return movavgout;

}
