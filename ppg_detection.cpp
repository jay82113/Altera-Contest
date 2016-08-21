#include "ppg_detection.h"
#include <QVector>
#include <numeric>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QString>


PPG_Detection::PPG_Detection()
{
    PeakDetect_CurrentValue = 0;
    PeakDetect_CurrentSlop = 0;
    PeakValue = 0;
    currentcycletime = 0;
    CurrentFFI = 0;
    currentcycleFoottime = 0;
    PeakDetect_PreviousSlop = 0;

    PreviousPPI = 0;
    CurrentPPI = 0;
    PeakDetect_PreviousPeak = 0;
    framerate = 26;
    PeakDetect_PreviousValue = 0;
    Flag_IsStartToDetect = false;
    Threshold_TheBiggest_Slope = 0;
    BiggestSlope = 0;
    DataNumberCount = 0;
    cycletimeavg = 0;
    PeakCount = 0;
    realPeakcnt = 0;
    IsPPIGet = false;
    IsPPIGet_Count = 0;
    cycletime.resize(10);
    PeakTime = 0;
    GaitCount = 0;
    // Foot Detection
    realFootcnt = 0;
    cycletimeFoot.resize(10);
    PeakDetect_PreviousFoot = 0;
    PreviousFFI = 0;
    Footfndcnt = 0;
    FootTime.resize(3);
    cycleFoottimeavg = 0;
}

bool PPG_Detection::PPG_Cnt(double PPG_Data, int n, double &PPG_FFI, double &HeartRate, QString &DATA_str)
{
    bool FindFoot = false;

    PeakDetect_CurrentValue = PPG_Data;//Rawdata
    PeakDetect_CurrentSlop = PeakDetect_CurrentValue - PeakDetect_PreviousValue;//斜率計算

    if (Flag_IsStartToDetect == true){
        if (PeakDetect_CurrentSlop < 0){
            Flag_IsStartToDetect = false; //偵測到Peak點，然後開始下降
            if (BiggestSlope >= Threshold_TheBiggest_Slope){ //&& PeakDetect_PreviousValue > 1.5 && PeakDetect_PreviousValue < 25.0
                CurrentPPI = (n-1) - PeakDetect_PreviousPeak; // 第一個Peak有問題
                currentcycletime = framerate/CurrentPPI; // in sec
                if(n>1){
                    PeakCount = 1;
                    PeakTime = n - 1;
                    PeakValue = PeakDetect_PreviousValue;
                }
                 //紀錄現在的數據(包含(第幾筆資料、數值、時間))
                if (IsPPIGet == false){
                    IsPPIGet_Count = IsPPIGet_Count + 1;
                    if (IsPPIGet_Count >= 2){ //剛開始都沒有PPI所以一定要等得到3筆資料以上才有PPI當參考比較
                        IsPPIGet = true;
                    }
                }
                else{ //有兩個Peak點被偵測到了
                    if (CurrentPPI >= PreviousPPI*0.3 && CurrentPPI <= PreviousPPI*1.7 && PeakDetect_CurrentValue > 2){ //要落在這個範圍內的才能當作是正常的PPI  0.3,1.7
                        GaitCount = GaitCount + 1; //步數
                        if (realPeakcnt < 10){
                            cycletime[realPeakcnt++] = currentcycletime;
                            cycletimeavg = (std::accumulate(cycletime.begin(),cycletime.end(),0))/realPeakcnt;
                        }
                        else{
                            if (currentcycletime < cycletimeavg*1.5 && currentcycletime > cycletimeavg*0.6){
                                for(int i=0; i<9; i++)
                                       cycletime[i] = cycletime[i+1];

                                cycletime[9] = currentcycletime;
                                cycletimeavg = (std::accumulate(cycletime.begin(),cycletime.end(),0))/10;
                            }
                        }
                    }
                    else{ // 不符合
                        BiggestSlope = 0;
                        IsPPIGet = false;
                        Flag_IsStartToDetect = false;
                        Threshold_TheBiggest_Slope = 0;
                        DataNumberCount = 0;
                      //  PeakCount = 0;
                      //   Footfndcnt = 0;
                    }
                }
                Threshold_TheBiggest_Slope = BiggestSlope; //先把這次的最大斜率 當成 Threshold
                Threshold_TheBiggest_Slope = Threshold_TheBiggest_Slope * 0.35; //動態調整threshold就是去把threshold乘以0.35
                PreviousPPI = CurrentPPI;
                PeakDetect_PreviousPeak = n-1; //把現在這次的數值時間記錄下來，若下次再找帶一個Peak可以該這個相減進而找出PPI
                if (IsPPIGet_Count >= 2) //剛開始都沒有PPI所以一定要等得到3筆資料以上才有PPI當參考比較
                   IsPPIGet = true;

            }
        }
        else{ //還在上升,還未下降
              //去比較以取出最大的斜率
            if (PeakDetect_CurrentSlop >= BiggestSlope) //若再偵測到的斜率>上一次的，那就更新這次的為參考threshold
                BiggestSlope = PeakDetect_CurrentSlop;

        }
    }
    else{ //還不能偵測Peak(ie,還沒偵測到漸升坡(ie,還未偵測到 Slope > 0 ))
        if (PeakDetect_CurrentSlop > Threshold_TheBiggest_Slope){ //偵測到現在的斜率 > threshold
            Flag_IsStartToDetect = true;
            BiggestSlope = 0;
            DataNumberCount = 0; //因為已經開始升坡了，代表開始有偵測到升波，所以TimeoutInitial要重新初始化，從0再開始計算

            if(n>1 && PPG_Data < 0){ //記錄下開始判定為上升波的點
                if (Footfndcnt < 3){
                    FootTime[Footfndcnt++] = n-1;
                }
                else{
                    FootTime[0] = FootTime[1];
                    FootTime[1] = FootTime[2];
                    FootTime[2] = n-1;
                }
                if (PeakCount > 0){
                    if (FootTime[1] < PeakTime && PeakTime < FootTime[2] && PeakValue > 0 && PeakDetect_CurrentValue < 0){
                         if (Footfndcnt >=2){
                            CurrentFFI = (n-1) - PeakDetect_PreviousFoot; // 第一個Peak有問題
                            currentcycleFoottime = framerate/CurrentFFI; // in sec
                            if (CurrentFFI >= (PreviousFFI*0.6) && CurrentFFI <= (PreviousFFI*2.5)){ //要落在這個範圍內的才能當作是正常的PPI  0.3,1.7
                                if (realFootcnt < 10){
                                    cycletimeFoot[realFootcnt++] = currentcycleFoottime;
                                    cycleFoottimeavg = (double) (std::accumulate(cycletimeFoot.begin(),cycletimeFoot.end(),0.0))/realFootcnt;
                                }
                                else{
                                    if (currentcycleFoottime < cycleFoottimeavg*2.5 && currentcycleFoottime > cycleFoottimeavg*0.6){
                                        for(int i=0; i<9; i++)
                                            cycletimeFoot[i] = cycletimeFoot[i+1];

                                        cycletimeFoot[9] = currentcycleFoottime;
                                        cycleFoottimeavg = (double) (std::accumulate(cycletimeFoot.begin(),cycletimeFoot.end(),0.0))/10;
                                        //std::cout << "********" << std::endl;
                                    }
                                }
                            }
                            PeakDetect_PreviousFoot = n-1;
                            PPG_FFI = CurrentFFI;
                            HeartRate = (double) cycleFoottimeavg*120;
                            DATA_str = "";
                            for(int i=0; i<10; i++)
                                DATA_str = DATA_str + QString::number(cycletimeFoot[i], 'f', 3) + " ";
                            DATA_str = DATA_str + "\n" + QString::number(cycleFoottimeavg, 'f', 3) + "\n" + QString::number(realFootcnt);
                            FindFoot = true;

                            PreviousFFI = CurrentFFI;


                            //std::cout << PPG_FFI << "   " << cycleFoottimeavg << std::endl;
                                    //心律為 cycleFoottimeavg*60
                         }
                    }
                }
            }
        }
        else{
            DataNumberCount = DataNumberCount + 1;
            //現在的斜率仍然是小於Threshold所以不做事，直接往下一筆資料移動
            if( DataNumberCount ==  25){
                //TimeoutInitial================================
                Flag_IsStartToDetect = false;
                Threshold_TheBiggest_Slope = 0;
                DataNumberCount = 0;
                IsPPIGet = false;
                //TimeoutInitial================================
            }
        }
    }

    PeakDetect_PreviousValue = PeakDetect_CurrentValue;//記錄下來，當成是下一筆資料的上一筆

    PeakDetect_PreviousSlop = PeakDetect_CurrentSlop;
    //endregion

    return FindFoot;

}
