#include "ppgfilter.h"
#include "qmath.h"
#include "iir_filter.h"
#include <numeric>
#include "math.h"
#include <iostream>
#include <cmath>

PPGFilter::PPGFilter()
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
     movingwidowsize = 12;

    // Adaptive Filter Parameter
     N = 1;
     beta = 0.00001;
     E = 0;
     W = 0;
     Adaptive_Output = 0;

     movingbuf.resize(movingwidowsize, 0.0);


     R_buf.resize(buf_size, 0.0);
     G_buf.resize(buf_size, 0.0);
     B_buf.resize(buf_size, 0.0);

     Xs_buf.resize(buf_size, 0.0);
     Ys_buf.resize(buf_size, 0.0);

     buf_count = 0;


}

double PPGFilter::PPG_Filter(double RawR, double RawG, double RawB,double RawY)
{
    // Moving Average
         // GRD Parameter
         if(buf_count < buf_size)
             buf_count++;

         for(int i=buf_count-1; i>0; i--){
             R_buf[i] = R_buf[i-1];
             G_buf[i] = G_buf[i-1];
             B_buf[i] = B_buf[i-1];
         }
         R_buf[0] = RawR;
         G_buf[0] = RawG;
         B_buf[0] = RawB;

         double R_sum = std::accumulate(R_buf.begin(), R_buf.begin()+buf_count, 0.0);
         double G_sum = std::accumulate(G_buf.begin(), G_buf.begin()+buf_count, 0.0);
         double B_sum = std::accumulate(B_buf.begin(), B_buf.begin()+buf_count, 0.0);

         double R_mean = R_sum / buf_count;
         double G_mean = G_sum / buf_count;
         double B_mean = B_sum / buf_count;

         double Xs = (3 * (RawR / R_mean) ) - (2 * (RawG / G_mean));
         double Ys = (1.5*(RawR/R_mean)) + (RawG/G_mean) - (1.5*(RawB/B_mean));

         double Xsf_out, Ysf_out;

         filter_Xs.Filter(Xs, Xsf_out);
         filter_Ys.Filter(Ys, Ysf_out);

         double Xs_STD=0, Ys_STD=0;

         for(int i=buf_count-1; i>0; i--){
             Xs_buf[i] = Xs_buf[i-1];
             Ys_buf[i] = Ys_buf[i-1];
         }
         Xs_buf[0] = Xsf_out; Ys_buf[0] = Ysf_out;

         double Xs_sum = std::accumulate(Xs_buf.begin(), Xs_buf.begin()+buf_count, 0.0);
         double Ys_sum = std::accumulate(Ys_buf.begin(), Ys_buf.begin()+buf_count, 0.0);

         double Xs_mean = Xs_sum / buf_count;
         double Ys_mean = Ys_sum / buf_count;

         for(int i=0; i<buf_count; i++){
             Xs_STD += ((Xs_buf[i]-Xs_mean) * (Xs_buf[i]-Xs_mean));
             Ys_STD += ((Ys_buf[i]-Ys_mean) * (Ys_buf[i]-Ys_mean));
         }

         Xs_STD = Xs_STD/(buf_count-1);
         Ys_STD = Ys_STD/(buf_count-1);

         Xs_STD = sqrt(Xs_STD);
         Ys_STD = sqrt(Ys_STD);

         double alpha = Xs_STD / Ys_STD;

         filter_R.Filter(RawR, IIR_R);
         filter_G.Filter(RawG, IIR_G);
         filter_B.Filter(RawB, IIR_B);

         PulseSignal = 0.0;
         PulseSignal = (double) ((3.0-(1.5*alpha)) * IIR_R) - ((2.0+alpha) * IIR_G) + (1.5 * alpha * IIR_B);
         double IIR_PulseSignal=0.0;



         if(std::isfinite((double) PulseSignal)){
            filter_Pulse.Filter(PulseSignal, IIR_PulseSignal);
         }

         std::cout << IIR_PulseSignal << std::endl;
      /*   //Moving Average
         if (moveavgcnt < movingwidowsize )
         {
             movingbuf[moveavgcnt]=IIR_PulseSignal;
             moveavgcnt++;
         }
         else
         {
                for(int j=0;j< movingwidowsize-1;j++)
                {
                    movingbuf[j] = movingbuf[j+1];
                }

                for(int k=0;k<movingwidowsize;k++)
                {
                    movavgout += movingbuf[k];
                }

            movavgout=movavgout/movingwidowsize;
            movingbuf[movingwidowsize-1] = IIR_PulseSignal;
            moveavgcnt++;
         }*/



       /*  for(int i=0; i<buf_size; i++)
             std::cout << Xs_buf[i] << " ";
         std::cout << std::endl;*/


         // IIR Filter



         //Moving Average

         //Adaptive Filter



         return IIR_PulseSignal;

}
