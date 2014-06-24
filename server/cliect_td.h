#ifndef CLIECT_TD_H
#define CLIECT_TD_H

#include <QWidget>
#include <QProcess>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/ipc.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "api.h"
 /*extern struct NodeInfo *pNodeInfo2;
//extern QList<DeviceInfo*> DevinfoList;
extern struct DeviceInfo*  DevinfoList;
extern struct NodeInfo *arr[20];
extern int i_depth;
extern int flag_coord;
extern float show_buf_temp[720] ;
extern float show_buf_hum[720] ;
extern int temp_count;
extern unsigned long int ptemperature;
extern unsigned long int pid;
extern unsigned long int id_temp;
extern int irda_warn_flag;
extern int int_warn_flag;
extern int smog_warn_flag;
//typedef enum {FALSE=0, TRUE=1} bool;
int Api_GetConnect(char *ipaddr,int port);
bool Api_Cliect_ZigBeeNwkDetect(void);
bool Api_Cliect_GetZigBeeNwkInfo(NwkDesp *pNwkDesp);
bool Api_Cliect_SetSensorWorkMode(unsigned char Mode);
DeviceInfo* Api_Cliect_GetZigBeeDevInfo(unsigned int nwkaddr);
NodeInfo* Api_Cliect_GetZigBeeNwkTopo(int *state);
int Api_Cliect_GetTempHum();
int Api_Cliect_GetRfidId();
int Api_Cliect_GetGPRSSignal();
//bool Api_Cliect_SendGprsMessage(char *phone,char *data,int datalen);
bool Api_Cliect_SendGprsMessage(char *phone,int sensor);
bool Api_Cliect_ClearIntlock();
SensorDesp *Api_Cliect_GetSensorStatus(unsigned int nwkaddr);
bool Api_Cliect_SetSensorStatus(unsigned int nwkaddr,unsigned char Mode);*/
class Cliect : public QThread
{
    Q_OBJECT
public:
    Cliect();
    ~Cliect();
    void run();
    QMutex mutex;
    struct timeval tv;
    fd_set sds;
    int coorstate;

    void Cliect_ZigBeeNwkTopo_Process(unsigned int *node,unsigned int count);

    void Cliect_ZigBeeNwkInfo_Process(unsigned int *nwkinfo);
    void Cliect_RfidId_Process(unsigned int *id);
    void Cliect_TempHum_Process(unsigned int *temp);
    void Cliect_GPRSSignal_Process(unsigned int sig);
    void Cliect_GPRSSend_Process(unsigned int sig);

signals:
    void StateChanged(void);
    void sendMsgsignal(int sensor);
    void sendTempHum(unsigned long value);
    void Irda_StateChanged_1(void);//duishe
    void Smog_StateChanged_1(void);//
    void Int_StateChanged_1(void);//door
    void Irda_StateChanged_0(void);//duishe
    void Smog_StateChanged_0(void);//
    void Int_StateChanged_0(void);//door
    void Rfid_Changed(void);
    void Sim(int sim);
    void Send(int sim);
    //void Sim_0(void);

public slots:
    void GetConnect(char *ipaddr,int port);
    void Api_Cliect_GetZigBeeNwkInfo();

    void Api_Cliect_GetZigBeeNwkTopo();
    void Api_Cliect_GetTempHum();
    void Api_Cliect_GetRfidId();
    void Api_Cliect_GetGPRSSignal();

    void Api_Cliect_SendGprsMessage(char *phone,int sensor);
    void Api_Cliect_ClearIntlock();
    void Api_Cliect_SetSensorStatus(unsigned int nwkaddr,unsigned int Mode);
    void Layer(int coorstate);



};
#endif

