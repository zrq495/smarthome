#ifndef MAINPLOT_H
#define MAINPLOT_H

#include <qwt_plot.h>
#include <qdatetime.h>
#include "main.h"

#define HISTORY 60 // seconds

class QwtPlotCurve;
class QwtPlotMarker;

class MainPlot : public QwtPlot
{
    Q_OBJECT
public:
    enum Data
    {
        Temp,
        Humi,

        NData
    };

    MainPlot(QWidget * = 0);
    //const QwtPlotCurve *cpuCurve(int id) const
    //    { return data[id].curve; }

protected:
    void timerEvent(QTimerEvent *e);//绘图更新

    void showSmog( double x, double y );

    QwtPlotMarker *smogMaker;



private slots:
    void showCurve(QwtPlotItem *, bool on);//曲线选择槽函数



private:
    struct
    {
        QwtPlotCurve *curve;
        double data[HISTORY];
    } data[NData];
    double timeData[HISTORY];

    int dataCount;
};


#endif // MAINPLOT_H
