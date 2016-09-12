#include "stepdetection.h"
#include <QVector>
#include <numeric>

StepDetection::StepDetection()
{

    Speed=0;
    PeakDetect_PreviousSlop=0;
    currentcycletime=0;
    PeakDetect_CurrentSlop=0;
    PeakDetect_CurrentValue=0;
    PPI_count = 0;
    PreviousPPI = 0;
    CurrentPPI = 0;
    PeakDetect_PreviousPeak = 0;
    framerate = 16;
    PeakDetect_PreviousValue = 0;
    Flag_IsStartToDetect = false;
    Threshold_TheBigest_Slope = 0;
    BiggestSlope = 0;
    DataNumberCount = 0;
    cycletimeavg = 1.0;
    PeakCount = 0;
    realPeakcnt = 0;
    IsPPIGet = false;
    IsPPIGet_Count = 0;
    height = 1.63;
    cycletime.resize(16);
    stepinmeter=0;
    LastPeakCount = 0;
    LastFoodCount = 0;
    Footcount = 0;
    Speedbufsize = 3;
    Speedbuf.resize(Speedbufsize+1);
    PPI.resize(100);
    Speedoutcnt = 0;
    stepcntwithin2snd=0;
    stepcntwithin2snd_foot=0;
    Calorie=0;
    Gaitcount = 0;
    PeakTime = 0;
    PeakValue = 0;

}

void StepDetection::StepCntInit()
{
    Gaitcount = 0;
}

bool StepDetection::StepCnt(double RawY, int n, int &StepCount)
{
       bool FindPeak = false;
       PeakDetect_CurrentValue = RawY; //Rawdata
       PeakDetect_CurrentSlop = PeakDetect_CurrentValue - PeakDetect_PreviousValue; //斜率計算


       if (Flag_IsStartToDetect == true)
       {
           if (PeakDetect_CurrentSlop < 0)
           {
               Flag_IsStartToDetect = false; //偵測到Peak點，然後開始下降
               if (BiggestSlope >= Threshold_TheBigest_Slope) //&& PeakDetect_PreviousValue > 1.5 && PeakDetect_PreviousValue < 25.0
               {
                   CurrentPPI = (n-1) - PeakDetect_PreviousPeak; // 第一個Peak有問題
                   currentcycletime = framerate/CurrentPPI; // in sec
                   if(n>1)
                   {
                           PeakCount = 1;
                           PeakTime = n - 1;
                           PeakValue = PeakDetect_PreviousValue;
                   }

                    //紀錄現在的數據(包含(第幾筆資料、數值、時間))
                 // Threshold_TheBigest_Slope = BiggestSlope;//先把這次的最大斜率 當成 Threshold
                   //Threshold_TheBigest_Slope = Threshold_TheBigest_Slope * 0.35;//動態調整threshold就是去把threshold乘以0.35
                 //  PreviousPPI = CurrentPPI;
                 //  PeakDetect_PreviousPeak = n-1;//把現在這次的數值時間記錄下來，若下次再找帶一個Peak可以該這個相減進而找出PPI
                   if( IsPPIGet == false)
                   {
                       IsPPIGet_Count = IsPPIGet_Count + 1;
                       if (IsPPIGet_Count >= 2)//剛開始都沒有PPI所以一定要等得到3筆資料以上才有PPI當參考比較
                           IsPPIGet = true;
                   }
                   else //有兩個Peak點被偵測到了
                   {
                       if ( PeakDetect_CurrentValue > 0)//要落在這個範圍內的才能當作是正常的PPI  0.3,1.7
                       {
                           Gaitcount = Gaitcount + 1;
                           FindPeak = true;
                           //PPI_count = PPI_count + 1;
                           //PPI[PPI_count] = CurrentPPI;

                           if (realPeakcnt < 10)
                           {
                               realPeakcnt = realPeakcnt + 1;
                               cycletime[realPeakcnt] = currentcycletime;
                               cycletimeavg = (std::accumulate(cycletime.begin(),cycletime.end(),0.0))/realPeakcnt;

                           }
                           else
                           {
                               if( currentcycletime < cycletimeavg*1.5 && currentcycletime > cycletimeavg*0.6 )
                               {
                                   for(int i=1;i<10;i++)
                                   {
                                          cycletime[i] = cycletime[i+1];
                                   }
                                   cycletime[10] = currentcycletime;
                                   cycletimeavg = (std::accumulate(cycletime.begin(),cycletime.end(),0.0))/10;

                               }
                           }
                        }
                       else // 不符合
                       {
                           BiggestSlope = 0;
                           //IsPPIGet = false;
                           //Flag_IsStartToDetect = false;
                           Threshold_TheBigest_Slope = 0;
                           DataNumberCount = 0;
                       }
                   }
                   Threshold_TheBigest_Slope = BiggestSlope; //先把這次的最大斜率 當成 Threshold
                   Threshold_TheBigest_Slope = Threshold_TheBigest_Slope * 0.25; //動態調整threshold就是去把threshold乘以0.35
                   PreviousPPI = CurrentPPI;
                   PeakDetect_PreviousPeak = n-1; //把現在這次的數值時間記錄下來，若下次再找帶一個Peak可以該這個相減進而找出PPI
                   if (IsPPIGet_Count >= 2) //剛開始都沒有PPI所以一定要等得到3筆資料以上才有PPI當參考比較
                      IsPPIGet = true;
               }
           }
           else //還在上升,還未下降
           {
               //去比較以取出最大的斜率
               if (PeakDetect_CurrentSlop >= BiggestSlope)//若再偵測到的斜率>上一次的，那就更新這次的為參考threshold
               {
                   BiggestSlope = PeakDetect_CurrentSlop;
               }
           }
       }
       else //還不能偵測Peak(ie,還沒偵測到漸升坡(ie,還未偵測到 Slope > 0 ))
       {
           if (PeakDetect_CurrentSlop > Threshold_TheBigest_Slope) //偵測到現在的斜率 > threshold
           {
               Flag_IsStartToDetect = true;
               BiggestSlope = 0;
               DataNumberCount = 0;//因為已經開始升坡了，代表開始有偵測到升波，所以TimeoutInitial要重新初始化，從0再開始計算

           }
           else
           {
               DataNumberCount = DataNumberCount + 1;
               //現在的斜率仍然是小於Threshold所以不做事，直接往下一筆資料移動
               if( DataNumberCount ==  25)
               {
                   //TimeoutInitial================================
                   //Flag_IsStartToDetect = false;
                   Threshold_TheBigest_Slope = 0;
                   DataNumberCount = 0;
                   //IsPPIGet = false;
                   //TimeoutInitial================================
               }
           }
       }
       PeakDetect_PreviousSlop = PeakDetect_CurrentSlop;
        PeakDetect_PreviousValue = PeakDetect_CurrentValue; //記錄下來，當成是下一筆資料的上一筆
       //endregion
   //--------------------------------------------------------------------------

       StepCount = Gaitcount;

       return FindPeak;


}

