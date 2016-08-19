#ifndef PPG_DETECTION_H
#define PPG_DETECTION_H
#include <QVector>

class PPG_Detection
{
public:
    PPG_Detection();
    bool PPG_Cnt(double, int, double &PPG_FFI, double &HeartRate, QString &DATA_str);

private:
    double PeakDetect_CurrentValue;
    double PeakDetect_CurrentSlop;
    double PeakValue;
    double currentcycletime;
    double CurrentFFI;
    double currentcycleFoottime;
    double PeakDetect_PreviousSlop;

    double PreviousPPI;
    double CurrentPPI;
    double PeakDetect_PreviousPeak;
    double framerate;
    double PeakDetect_PreviousValue;
    bool Flag_IsStartToDetect;
    double Threshold_TheBiggest_Slope;
    double BiggestSlope;

    int DataNumberCount;
    double cycletimeavg;
    int PeakCount;
    int realPeakcnt;
    bool IsPPIGet;
    int IsPPIGet_Count;
    std::vector <double> cycletime;
    int PeakTime;
    int GaitCount;
    // Foot Detection
    int realFootcnt;
    std::vector <double> cycletimeFoot;
    double PeakDetect_PreviousFoot;
    double PreviousFFI;
    int Footfndcnt;
    std::vector <int> FootTime;
    double cycleFoottimeavg;
};

#endif // PPG_DETECTION_H
