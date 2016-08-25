#ifndef STEPDETECTION_H
#define STEPDETECTION_H
#include <QVector>

class StepDetection
{


public:
    StepDetection();
    bool StepCnt(double, int, int &);


private:

    double Calorie;
    int Speedoutcnt;
    double Speed;
    double PeakDetect_PreviousSlop;
    double currentcycletime;
    double PeakDetect_CurrentSlop;
    double PeakDetect_CurrentValue;
    int PPI_count;
    int PreviousPPI;
    int CurrentPPI;
    double PeakDetect_PreviousPeak;
    int framerate;
    double PeakDetect_PreviousValue;
    bool Flag_IsStartToDetect;
    double Threshold_TheBigest_Slope;
    double BiggestSlope;
    int DataNumberCount;
    double cycletimeavg;
    int PeakCount;
    int realPeakcnt;
    bool IsPPIGet;
    int IsPPIGet_Count;
    QVector <double> cycletime;
    double height;
    double stepinmete;
    int LastPeakCount;
    int LastFoodCount;
    int Footcount;
    int Speedbufsize;
    QVector <double> Speedbuf;
    QVector <double> PPI;
    int stepcntwithin2snd;
    int stepcntwithin2snd_foot;
    double stepinmeter;
    int Gaitcount;
    double PeakValue;
    int PeakTime;



};

#endif // STEPDETECTION_H
