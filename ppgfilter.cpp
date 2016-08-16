#include "ppgfilter.h"
#include "qmath.h"
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
     phase = 8;

//     IIR_B[9][3]={{0.680872798,0,0},
//                     {1,0,-1},
//                     {0.680872798,0,0},
//                     {1,0,-1},
//                     {0.5794138312,0,0},
//                     {1,0,-1},
//                     {0.5794138312,0,0},
//                     {1,0,-1},
//                     {1,0,0}};

//     IIR_A[9][3]={{1,0,0},
//                     {1,-1.295434237,0.6719222069},
//                     {1,0,0},
//                     {1,1.351098657,0.6866300106},
//                     {1,0,0},
//                     {1,0.9060385227,0.2532167435},
//                     {1,0,0},
//                     {1,-0.8466194272,0.2301869392},
//                     {1,0,0}};

     for(int i=0;i<10;i++)
     {
         for(int j =0;j<3;j++)
         {
             filter_x [i][j]=0;
             filter_y [i][j]=0;
             filter_x2 [i][j]=0;
             filter_y2 [i][j]=0;
             filter_x_R [i][j]=0;
             filter_y_R [i][j]=0;

         }

     }


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

     movingbuf.resize(12);


}

double PPGFilter::PPG_Filter(double RawR, double RawG, double RawB,double RawY)
{
    // Moving Average
         // GRD Parameter

         GRDbase = qSqrt((RawR*RawR)+(RawG*RawG)+(RawB*RawB));
         alphaGbetaG = RawG / GRDbase;
         alphaRbeta_R = RawR / GRDbase;

         // IIR Filter
         filter_x[0][0] = RawG;
         filter_x_R[0][0] = RawR;

         for(int i=0;i<phase+1;i++)  // 0~8    max filter_x  [9][2]  IIR max [8][2]
         {
             filter_y[i][0]= (filter_x[i][0]*IIR_B[i][0]) +(filter_x[i][1]*IIR_B[i][1]) + (filter_x[i][2]*IIR_B[i][2]) - (filter_y[i][1]*IIR_A[i][1])- (filter_y[i][2]*IIR_A[i][2]);
             filter_y_R[i][0]= (filter_x_R[i][0]*IIR_B[i][0]) + (filter_x_R[i][1]*IIR_B[i][1]) + (filter_x_R[i][2]*IIR_B[i][2]) - (filter_y_R[i][1]*IIR_A[i][1]) - ( filter_y_R[i][2]*IIR_A[i][2] );
             filter_y[i][0] = filter_y[i][0] / IIR_A[i][0];
             filter_y_R[i][0] = filter_y_R[i][0]/IIR_A[i][0];
             filter_y[i][2]=filter_y[i][1];
             filter_y_R[i][2]=filter_y_R[i][1];
             filter_y[i][1]=filter_y[i][0];
             filter_y_R[i][1]=filter_y_R[i][0];
             filter_x[i][2]=filter_x[i][1];
             filter_x_R[i][2]=filter_x_R[i][1];
             filter_x[i][1]=filter_x[i][0];
             filter_x_R[i][1]=filter_x_R[i][0];
             filter_x[i+1][0] = filter_y[i][0];
             filter_x_R[i+1][0] = filter_y_R[i][0];
         }

         //GRD operation   // 0~8    max filter_x  [9][2]  IIR max [8][2]
         filtered_PPG=(filter_x[phase+1][0]/alphaGbetaG);
         filtered_R=(filter_x_R[phase+1][0]/alphaRbeta_R);
         GRD_out= (filtered_PPG - filtered_R)*20;

         //Moving Average
         if (moveavgcnt < movingwidowsize )
         {
             movingbuf[moveavgcnt]=GRD_out;
             moveavgcnt++;
         }
         else
         {
                for(int j=0;j< movingwidowsize-1;j++)
                {
                    movingbuf[j] = movingbuf[j+1];
                }

                for(int k=0;k<12;k++)
                {
                    movavgout += movingbuf[k];
                }

            movavgout=movavgout/12;
            movingbuf[movingwidowsize-1] = GRD_out;
            moveavgcnt++;
         }

         //Adaptive Filter

         Adaptive_Output = W * RawY; // approximate niose
         E = movavgout - Adaptive_Output; // Desire signal
         W=W + (beta*E*RawY);

         return movavgout;

}
