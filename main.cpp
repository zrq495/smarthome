#include <QtGui/QApplication>
#include "mywidget.h"
#include "main.h"
#include <QTextCodec>

double tempData::tempValue =0;
double tempData::humiValue =0;
int tempData::smogStatus = 0;
bool tempData::allAlarmIsOn= true;
double tempData::maxTempValue = 0;
double tempData::minTempValue = 0xfffff;
double tempData::maxHumiValue = 0;
double tempData::minHumiValue = 0xfffff;
int tempData::irdaStatue = 0;
double tempData::tempValueBlue = 0;
double tempData::humiValueBlue = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setFamily("wenquanyi");
    //font.setPointSize(10);
    a.setFont(font);

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    myWidget w;
    //w.show();
    w.showFullScreen();
    
    return a.exec();
}
