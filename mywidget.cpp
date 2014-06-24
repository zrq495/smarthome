#include "mywidget.h"
#include "ui_mywidget.h"
#include "mainplot.h"
#include <math.h>
#include "server/api.h"
#include "server/cliect_td.h"
#include <QDateTime>
#include "main.h"
#include <stdlib.h>

int flagt1=0,flagh1=0,flagt2=0,flagh2=0;
float d1,d2;
int SmogStatus,IrdaStatue, IntStatus;
bool allAlarmIsOn;

myWidget::myWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myWidget)
{
    ui->setupUi(this);

//    ui->frame_3->setStyleSheet("QFrame#frame_3{border-image:url(images/b1.jpg)}");
//    ui->frame_4->setStyleSheet("QFrame#frame_4{border-image:url(images/2267807_180803075_2.jpg)}");
//    ui->frame_5->setStyleSheet("QFrame#frame_5{border-image:url(images/1348966782.1694587446.jpg)}");
//    ui->frame_6->setStyleSheet("QFrame#frame_6{border-image:url(images/2012052412255329.jpg)}");
//    ui->frame_7->setStyleSheet("QFrame#frame_7{border-image:url(images/1348966782.1694587446.jpg)}");
//    ui->frame_8->setStyleSheet("QFrame#frame_8{border-image:url(images/2310993_092131041337_2.jpg)}");

    QWidget vBox;
    MainPlot *plot1 = new MainPlot(&vBox);
    plot1->setTitle(QObject::tr("温湿度曲线"));
    //plot1->setMargin(5);
    plot1->setStyleSheet("QwtPlot { padding: 3px }");
    ui->tabWidget->addTab(plot1,QObject::tr("温湿度曲线"));

    ui->tabWidget->setStyleSheet("QTabBar::tab{width:120;height:45}");

    humiAlarmsValue = 60;
    tempAlarmsValue = 40;
    humiAlarmsValueBlue = 60;
    tempAlarmsValueBlue = 40;

    allAlarmIsOn = true;

    phone_data=(char*)malloc(39);
    //phone_num="18353363262";//发送目的地的手机号码
    phone_num="18353364950";
    Sim_state=0;

    char *ipaddr="192.168.12.248";
    printf("ipaddr%s\n",ipaddr);
    int port=7838;
    cliect_thread = new Cliect();
    cliect_thread->GetConnect(ipaddr,port);
    connect(cliect_thread,SIGNAL(sendMsgsignal(int )),this,SLOT(gprs_sendMsg(int)));
    connect(cliect_thread,SIGNAL(Int_StateChanged_1()),this,SLOT(Int_change_1()));
    connect(cliect_thread,SIGNAL(Irda_StateChanged_1()),this,SLOT(Irda_change_1()));
    connect(cliect_thread,SIGNAL(Smog_StateChanged_1()),this,SLOT(Smog_change_1()));
    connect(cliect_thread,SIGNAL(Int_StateChanged_0()),this,SLOT(Int_change_0()));
    connect(cliect_thread,SIGNAL(Irda_StateChanged_0()),this,SLOT(Irda_change_0()));
    connect(cliect_thread,SIGNAL(Smog_StateChanged_0()),this,SLOT(Smog_change_0()));

    connect(cliect_thread,SIGNAL(sendTempHum(unsigned long )),this,SLOT(show_TempHum(unsigned long )));
    connect(cliect_thread,SIGNAL(Sim(int )),this,SLOT(Set_Sim(int)));
    connect(cliect_thread,SIGNAL(Send(int)),this,SLOT(Set_Send(int)));

    connect(this,SIGNAL(Int_StateChanged_1()),this,SLOT(Int_change_1()));
    connect(this,SIGNAL(Irda_StateChanged_1()),this,SLOT(Irda_change_1()));
    connect(this,SIGNAL(Smog_StateChanged_1()),this,SLOT(Smog_change_1()));
    connect(this,SIGNAL(Int_StateChanged_0()),this,SLOT(Int_change_0()));
    connect(this,SIGNAL(Irda_StateChanged_0()),this,SLOT(Irda_change_0()));
    connect(this,SIGNAL(Smog_StateChanged_0()),this,SLOT(Smog_change_0()));
    cliect_thread->start();
    zb_thread = new ZigbeeTopo();

    ui->TemAlarmlcdNumber1->display(tempAlarmsValue);//设置初始低温度警告值为25
    ui->TemAlarmlcdNumber2->display(tempAlarmsValueBlue);//设置初始高温度警告值为35
    ui->HumAlarmlcdNumber1->display(humiAlarmsValue);//设置初始低湿度警告值为28
    ui->HumAlarmlcdNumber2->display(humiAlarmsValueBlue);//设置初始高湿度警告值为65
    ui->TemAlarmSlider1->setValue(tempAlarmsValue);//设置初始温度警告值为25
    ui->TemAlarmSlider2->setValue(tempAlarmsValueBlue);
    ui->Slider_3->setValue(humiAlarmsValue);
    ui->Slider_4->setValue(humiAlarmsValueBlue);

    QTimer *paintTimer=new QTimer(this);
    paintTimer->start(200);
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(showtemvalue1()));//实时显示监测点1温度的值
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(showhumvalue1()));//实时显示监测点1湿度的值
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(showtemvalue2()));//实时显示监测点2温度的值
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(showhumvalue2()));//实时显示监测点2湿度的值
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(showSmogStatus()));//将获取的烟雾状态显示出来
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(showIrdaStatus()));//将获取的红外状态显示出来
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(showIntStatus()));
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(gettemvalue()));//得到改变后的警告温度值
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(gethumvalue()));//得到改变后的警告湿度值
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(readBluetoothdata()));//读取蓝牙数据
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(judgeOffAlarm()));
    connect(paintTimer,SIGNAL(timeout()),this,SLOT(closeAllAlarmCheckBox_stateChanged()));//定时检测是否关闭所有警报
    connect(this,SIGNAL(alarm()),this,SLOT(playAlarm()));//如果检测到警报信号则报警
    connect(this,SIGNAL(noalarm()),this,SLOT(playOffAlarm()));//如果检测到警报取消信号则取消报警

    QTimer *Timernow=new QTimer(this);//显示现在的时间
    Timernow->start(1000);
    connect(Timernow,SIGNAL(timeout()),this,SLOT(get_Datetime()));
}

myWidget::~myWidget()
{
    delete ui;
}

void myWidget::get_Datetime()
{
    datetimeReresh();
}

void myWidget::datetimeReresh()
{
    QDateTime time=QDateTime::currentDateTime();
    QString text= time.toString("yyyy-MM-dd hh:mm:ss");
    //qDebug() << text;
    ui->lcdNumber->display(text);
}

void myWidget::gprs_sendMsg(int sensor)
{
    if(Sim_state==1)
    {
        if(sensor==1)
        {
            ui->send_msg->setText("send irda...");
            cliect_thread->Api_Cliect_SendGprsMessage(phone_num,1);
        }
        else if(sensor==2)
        {
            ui->send_msg->setText("send smog...");
            cliect_thread->Api_Cliect_SendGprsMessage(phone_num,2);

        }
        else if(sensor==3)
        {
            ui->send_msg->setText("send int...");
            cliect_thread->Api_Cliect_SendGprsMessage(phone_num,3);

        }
    }
}

void myWidget::Smog_change_1()
{
    SmogStatus=1;
    emit alarm();
    qDebug("---------------------------SmogStatus=1-----------------------------");
}

void myWidget::Int_change_1()
{
    IntStatus = 1;
    emit alarm();
    qDebug("---------------------------IntStatue=1-----------------------------");
}

void myWidget::Irda_change_1()
{
    IrdaStatue = 1;
    emit alarm();
    qDebug("---------------------------IrdaStatue=1-----------------------------");
}

void myWidget::Smog_change_0()
{
    SmogStatus=0;
    qDebug("---------------------------SmogStatus=0-------------------------------");
}

void myWidget::Int_change_0()
{
    IntStatus = 0;
    qDebug("---------------------------IntStatue=0-----------------------------");
}

void myWidget::Irda_change_0()
{
    IrdaStatue = 0;
    qDebug("---------------------------IrdaStatue=0-----------------------------");
}

void myWidget::show_TempHum(unsigned long x)
{
    char temp[5],humi[5],data[4];
    char *pTemp,*pHumi;
    pTemp = temp;
    pHumi = humi;
    int return_t=0;
    data[0]=BREAK_UINT32( x,3 );
    data[1]=BREAK_UINT32( x,2 );
    data[2]=BREAK_UINT32( x,1 );
    data[3]=BREAK_UINT32( x,0 );
    return_t=getsht11(data, pTemp, pHumi);
    if(return_t){
        d1=atof(pTemp);
        d2=atof(pHumi);
        tempData::tempValue = d1;
        tempData::humiValue = d2;
        //        ui->TemAlarmInflabel->setText(pTemp);
        //        ui->HumAlarmInflabel->setText(pHumi);
    }
}

void myWidget::Set_Sim(int sim)
{
    if(sim==0)
    {
        ui->state_sim->setText(QString::fromLocal8Bit("初始化..."));
        Sim_state=0;
    }
    else if(sim==1)
    {
        ui->state_sim->setText(QString::fromLocal8Bit("初始化成功"));
        Sim_state=1;
    }
}

void myWidget::Set_Send(int sim)
{
    if(sim==1)
        ui->send_msg->setText(QString::fromLocal8Bit("发送成功"));
    sim=0;
}

int myWidget::calc_sth11(float *p_humidity, float *p_temperature)
{
    int ret= 0;

    const float C1=-4.0; // for 12 Bit
    const float C2= 0.0405; // for 12 Bit
    const float C3=-0.0000028; // for 12 Bit
    const float T1=0.01; // for 14 Bit @ 5V
    const float T2=0.00008; // for 14 Bit @ 5V

    float rh=*p_humidity; // rh: Humidity [Ticks] 12 Bit
    float t=*p_temperature; // t: Temperature [Ticks] 14 Bit
    float rh_lin; // rh_lin: Humidity linear
    float rh_true; // rh_true: Temperature compensated humidity
    float t_C; // t_C : Temperature [C]
    t_C=t*0.01 - 42; //calc. Temperature from ticks to [C]
    rh_lin=C3*rh*rh + C2*rh + C1; //calc. Humidity from ticks to [%RH]
    rh_true=(t_C-25)*(T1+T2*rh)+rh_lin; //calc. Temperature compensated humidity [%RH]
    if(rh_true>100)rh_true=100; //cut if the value is outside of
    if(rh_true<0.1)rh_true=0.1; //the physical possible range
    *p_temperature=t_C; //return temperature [C]
    *p_humidity=rh_true; //return humidity[%RH]

    if (t_C>0)return ret = 1;
    return ret;
}

int myWidget::getsht11(char *rData, char *&pTemp, char *&pHumi)
{
    int i=0,j=0;
    float temp,humi;
    uint8 tlen,hlen;
    int calcret=0;
    //char Temp_t[16];


    i = rData[1] * 256 + rData[0];
    j = rData[3] * 256 + rData[2];


    temp = (float)j;
    humi = (float)i;
    calcret = calc_sth11(&humi,&temp);
    //qDebug("\rtemp=%f\t humi=%f\r",temp,humi);
    if( ((int)temp) > 0  && 0 <= humi <= 100 && calcret) {
        tlen = sprintf(pTemp, "%d.%d",(int)temp,(int)((temp-(int)temp)*10));
        hlen = sprintf(pHumi, "%d.%d",(int)humi,(int)((humi-(int)humi)*10));
        //qDebug("\rTEMP=%s\tHUMI=%s\r",pTemp,pHumi);
        return 1;
    }
    return 0;
}

void myWidget::readBluetoothdata()
{
    char temp[5],humi[5],data[4];
    char *pTemp,*pHumi;
    pTemp = temp;
    pHumi = humi;
    int return_t=0;

    {
        //qDebug("-----------------bluetooth data----------------\n");
        data[0]=BREAK_UINT32( ptemperature,3 );
        data[1]=BREAK_UINT32( ptemperature,2 );
        data[2]=BREAK_UINT32( ptemperature,1 );
        data[3]=BREAK_UINT32( ptemperature,0 );
        return_t=getsht11(data, pTemp, pHumi);


        d1b=atof(pTemp);//蓝牙读取的温度信息
        d2b=atof(pHumi);//蓝牙读取的湿度信息
    }
}

void myWidget::playAlarm()
{
    if(allAlarmIsOn)
    {
        //        ui->Alarmlabelshow1->setVisible(false);
        //        ui->Alarmlabelshow2->setVisible(true);
        cliect_thread->Api_Cliect_SetSensorStatus(0x50f5,1);
    }
    else
    {
        //        ui->Alarmlabelshow1->setVisible(true);
        //        ui->Alarmlabelshow2->setVisible(false);
        cliect_thread->Api_Cliect_SetSensorStatus(0x50f5,0);
    }
}

void myWidget::playOffAlarm()
{
    cliect_thread->Api_Cliect_SetSensorStatus(0x50f5,0);
}

void myWidget::judgeOffAlarm()
{
    if(flagt1==0&&flagt2==0&&flagh1==0&&flagh2==0&&SmogStatus==0&&IrdaStatue==0&&IntStatus==0)
        emit noalarm();
}

void myWidget::showtemvalue1()
{
    QPalette peRed;
    peRed.setColor (QPalette::WindowText, Qt::red);

    QPalette peBlack;
    peBlack.setColor (QPalette::WindowText, Qt::black);

    QPalette peBlue;
    peBlue.setColor (QPalette::WindowText, Qt::blue);
    QString over,normal;

    over = over.fromLocal8Bit("过高");
    normal = normal.fromLocal8Bit("正常");
    if(d1>=tempAlarmsValue)
    {
        ui->TemAlarmInflabel->setPalette(peRed);
        ui->TemAlarmInflabel->setText(over);
        flagt1=1;
        emit alarm();
    }
    else
    {
        ui->TemAlarmInflabel->setPalette(peBlack);
        ui->TemAlarmInflabel->setText(normal);
        flagt1=0;
    }
    //ui->Thermo->setAlarmLevel(tempAlarmsValue);
    ui->TempLcdNumber->display(d1);
    ui->Thermo->setValue(d1);
}

void myWidget::showhumvalue1()
{
    QPalette peRed;
    peRed.setColor (QPalette::WindowText, Qt::red);

    QPalette peBlack;
    peBlack.setColor (QPalette::WindowText, Qt::black);

    QPalette peBlue;
    peBlue.setColor (QPalette::WindowText, Qt::blue);
    QString over,normal;
    over = over.fromLocal8Bit("过大");
    normal = normal.fromLocal8Bit("正常");
    if(d2>=humiAlarmsValue)
    {

        ui->HumAlarmInflabel->setPalette(peRed);
        ui->HumAlarmInflabel->setText(over);
        flagh1=1;
        emit alarm();
    }
    else
    {
        ui->HumAlarmInflabel->setPalette(peBlack);
        ui->HumAlarmInflabel->setText(normal);
        flagh1=0;
    }
    //ui->Thermo_3->setAlarmLevel(humiAlarmsValue);
    ui->HumLcdNumber->display(d2);
    ui->Thermo_3->setValue(d2);
}

void myWidget::showtemvalue2()
{
    QPalette peRed;
    peRed.setColor (QPalette::WindowText, Qt::red);

    QPalette peBlack;
    peBlack.setColor (QPalette::WindowText, Qt::black);

    QPalette peBlue;
    peBlue.setColor (QPalette::WindowText, Qt::blue);
    QString over,normal;

    over = over.fromLocal8Bit("过高");
    normal = normal.fromLocal8Bit("正常");
    if(d1b>=tempAlarmsValueBlue)
    {
        ui->TemAlarmInflabel2->setPalette(peRed);
        ui->TemAlarmInflabel2->setText(over);
        flagt2=1;
        emit alarm();
    }
    else
    {
        ui->TemAlarmInflabel2->setPalette(peBlack);
        ui->TemAlarmInflabel2->setText(normal);
        flagt2=0;
    }
    //ui->Thermo_2->setAlarmLevel(tempAlarmsValue);
    ui->TempLcdNumber2->display(d1b);
    ui->Thermo_2->setValue(d1b);
}

void myWidget::showhumvalue2()
{
    QPalette peRed;
    peRed.setColor (QPalette::WindowText, Qt::red);

    QPalette peBlack;
    peBlack.setColor (QPalette::WindowText, Qt::black);

    QPalette peBlue;
    peBlue.setColor (QPalette::WindowText, Qt::blue);
    QString over,normal;
    over = over.fromLocal8Bit("过大");
    normal = normal.fromLocal8Bit("正常");
    if(d2b>=humiAlarmsValueBlue)
    {

        ui->HumAlarmInflabel2->setPalette(peRed);
        ui->HumAlarmInflabel2->setText(over);
        flagh2=1;
        emit alarm();
    }
    else
    {
        ui->HumAlarmInflabel2->setPalette(peBlack);
        ui->HumAlarmInflabel2->setText(normal);
        flagh2=0;
    }
    //ui->Thermo_4->setAlarmLevel(humiAlarmsValue);
    ui->HumLcdNumber2->display(d2b);
    ui->Thermo_4->setValue(d2b);
}

void myWidget::showSmogStatus()
{
    QPalette peRed;
    peRed.setColor (QPalette::WindowText, Qt::red);

    QPalette peBlack;
    peBlack.setColor (QPalette::WindowText, Qt::black);

    QString fire,normal;
    fire = fire.fromLocal8Bit("有火");
    normal = normal.fromLocal8Bit("正常");


    if(SmogStatus==1 )//
    {
        ui->SmogStatelabel->setPalette(peRed);
        ui->SmogAlarmInflabel->setPalette(peRed);
        ui->SmogStatelabel->setText(fire);
        ui->SmogAlarmInflabel->setText(fire);
    }

    else
    {
        ui->SmogStatelabel->setPalette(peBlack);
        ui->SmogAlarmInflabel->setPalette(peBlack);
        ui->SmogStatelabel->setText(normal);
        ui->SmogAlarmInflabel->setText(normal);
    }
}

void myWidget::showIrdaStatus()
{
    QPalette peRed;
    peRed.setColor (QPalette::WindowText, Qt::red);

    QPalette peBlack;
    peBlack.setColor (QPalette::WindowText, Qt::black);

    QString someone,normal;
    someone = someone.fromLocal8Bit("有人闯入");
    normal = normal.fromLocal8Bit("正常");

    if(IrdaStatue==1)
    {
        ui->IrdaStatelabel->setPalette(peRed);
        ui->IrdaAlarmInflabel->setPalette(peRed);
        ui->IrdaStatelabel->setText(someone);
        ui->IrdaAlarmInflabel->setText(someone);
    }
    else
    {
        ui->IrdaStatelabel->setPalette(peBlack);
        ui->IrdaAlarmInflabel->setPalette(peBlack);
        ui->IrdaStatelabel->setText(normal);
        ui->IrdaAlarmInflabel->setText(normal);
    }
}

void myWidget::gettemvalue()
{
    tempAlarmsValue = ui->TemAlarmSlider1->value();
    tempAlarmsValueBlue = ui->TemAlarmSlider2->value();
    ui->Thermo->setAlarmLevel(tempAlarmsValue);
    ui->Thermo_2->setAlarmLevel(tempAlarmsValueBlue);
}

void myWidget::gethumvalue()
{
    humiAlarmsValue = ui->Slider_3->value();
    humiAlarmsValueBlue = ui->Slider_4->value();
    ui->Thermo_3->setAlarmLevel(humiAlarmsValue);
    ui->Thermo_4->setAlarmLevel(humiAlarmsValueBlue);
}

void myWidget::on_Slider_3_valueChanged(double value)
{
    ui->Thermo_3->setAlarmLevel(value);
    ui->HumAlarmlcdNumber1->display(value);
}

void myWidget::on_Slider_4_valueChanged(double value)
{
    ui->Thermo_4->setAlarmLevel(value);
    ui->HumAlarmlcdNumber2->display(value);
}

void myWidget::on_TemAlarmSlider1_valueChanged(int value)
{
    ui->Thermo->setAlarmLevel(value);
    ui->TemAlarmlcdNumber1->display(value);
}

void myWidget::on_TemAlarmSlider2_valueChanged(int value)
{
    ui->Thermo_2->setAlarmLevel(value);
    ui->TemAlarmlcdNumber2->display(value);
}

void myWidget::closeAllAlarmCheckBox_stateChanged()
{
    if(ui->closeAllAlarmCheckBox->isChecked())
    {
        allAlarmIsOn = false;
    }
    else
        allAlarmIsOn = true;
}

void myWidget::showIntStatus()
{
    QPalette peRed;
    peRed.setColor (QPalette::WindowText, Qt::red);

    QPalette peBlack;
    peBlack.setColor (QPalette::WindowText, Qt::black);

    QString someone,normal;
    someone = someone.fromLocal8Bit("报警");
    normal = normal.fromLocal8Bit("正常");

    if(IntStatus==1)
    {
        ui->IntStatelabel->setPalette(peRed);
        ui->IntAlarmInflabel->setPalette(peRed);
        ui->IntStatelabel->setText(someone);
        ui->IntAlarmInflabel->setText(someone);
    }
    else
    {
        ui->IntStatelabel->setPalette(peBlack);
        ui->IntAlarmInflabel->setPalette(peBlack);
        ui->IntStatelabel->setText(normal);
        ui->IntAlarmInflabel->setText(normal);
    }
}
