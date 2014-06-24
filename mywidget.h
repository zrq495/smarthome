#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "server/cliect_td.h"
#include "zigbeetopo.h"
#include "main.h"

extern  int SmogStatus,IrdaStatue, IntStatus;
extern float d1,d2;//d1为温度，d2为湿度，d3为光照强度
extern bool allAlarmIsOn; //= true;

namespace Ui {
class myWidget;
}

typedef signed   char   int8;
typedef unsigned char   uint8;
typedef unsigned char   byte;
typedef signed   short  int16;
typedef unsigned short  uint16;

typedef signed   long   int32;
typedef unsigned long   uint32;
#define BREAK_UINT32( var, ByteNum ) \
          (byte)((uint32)(((var) >>((ByteNum) * 8)) & 0x00FF))


class myWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit myWidget(QWidget *parent = 0);

    ZigbeeTopo *zb_thread;
    Cliect *cliect_thread;

    char *phone_num,*phone_data;  //手机号码
    int Sim_state;

    int calc_sth11(float *p_humidity ,float *p_temperature);
    int getsht11(char *rData, char *&pTemp, char *&pHumi);

    ~myWidget();
    
private:
    Ui::myWidget *ui;

    double  tempAlarmsValue;//温度报警值
    double  humiAlarmsValue;//湿度报警值
    double  tempAlarmsValueBlue;//温度报警值
    double  humiAlarmsValueBlue;//湿度报警值

    float d1b, d2b;



signals:
    void Smog_StateChanged_1(void);
    void Smog_StateChanged_0(void);
    void Irda_StateChanged_1(void);
    void Irda_StateChanged_0(void);
    void Int_StateChanged_1(void);
    void Int_StateChanged_0(void);
    void alarm();
    void noalarm();

public slots:
    void get_Datetime();
    void datetimeReresh();
    void gprs_sendMsg(int sensor);
    void Smog_change_1();
    void Int_change_1();
    void Irda_change_1();
    void Smog_change_0();
    void Int_change_0();
    void Irda_change_0();
    void show_TempHum(unsigned long x);
    void Set_Sim(int sim);
    void Set_Send(int sim);
    void playAlarm();//如果达到警报则报警
    void playOffAlarm();
    void judgeOffAlarm();
    void closeAllAlarmCheckBox_stateChanged();//关闭所有警报
    void readBluetoothdata();
    void showtemvalue1();//显示实时监测点1的温度
    void showhumvalue1();//显示实时监测点1的湿度
    void showtemvalue2();//显示实时监测点2的温度
    void showhumvalue2();//显示实时监测点2的湿度
    void showSmogStatus();//将获取的烟雾状态显示出来
    void showIrdaStatus();//将获取的红外对射显示出来
    void showIntStatus();//门磁
    void gettemvalue();//得到改变后警告的温度
    void gethumvalue();//得到改变后警告的湿度

private slots:
    void on_Slider_3_valueChanged(double value);
    void on_Slider_4_valueChanged(double value);
    void on_TemAlarmSlider1_valueChanged(int value);
    void on_TemAlarmSlider2_valueChanged(int value);
};

#endif // MYWIDGET_H
