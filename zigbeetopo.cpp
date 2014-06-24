#include "zigbeetopo.h"

ZigbeeTopo::ZigbeeTopo()
{      
    cliect_thread1 = new Cliect();
    this->start();
}

ZigbeeTopo::~ZigbeeTopo()
{
}

void ZigbeeTopo::run(){

    while(1)
    {
        mutex.lock();

        cliect_thread1->Api_Cliect_GetTempHum();

        //cliect_thread1->Api_Cliect_GetRfidId();

        cliect_thread1->Api_Cliect_GetGPRSSignal();

        cliect_thread1->Api_Cliect_GetZigBeeNwkTopo();


        mutex.unlock();

        usleep(800000);
        usleep(800000);
        usleep(800000);
    }

}
