#ifndef MAIN_H
#define MAIN_H

#include <QDebug>

class tempData
{
public:
    static double tempValue;//温度值
    static double humiValue;//湿度值
    static int smogStatus; //烟雾
    static double maxTempValue;//最大温度
    static double minTempValue;//最小温度
    static double maxHumiValue;//最大湿度
    static double minHumiValue;//最小湿度
    static bool allAlarmIsOn;//所有报警开标志
    static int irdaStatue; //红外
    static double tempValueBlue;
    static double humiValueBlue;
};

#endif // MAIN_H
