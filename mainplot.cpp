#include "mainplot.h"
#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <math.h>
#include<QTime>
#include<QDebug>
#include"main.h"
#include <qwt_plot_marker.h>
#include <qwt_legend_item.h>

class TimeScaleDraw: public QwtScaleDraw//时间更新轴类
{
public:
    TimeScaleDraw(const QTime &base):
        baseTime(base)
    {
    }
    virtual QwtText label(double v) const
    {
        QTime upTime = baseTime.addSecs((int)v-60);

        QStringList list = (upTime.toString()).split(":");

        QString str =list[1]+":"+list[2];
//        qDebug() << v << baseTime << upTime << str;
        return str;
        // return upTime.toString();

    }
private:
    QTime baseTime;
};

class Background: public QwtPlotItem//背景设置类
{
public:
    Background()
    {
        setZ(0.0);
    }

    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }

    virtual void draw(QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRect &rect) const
    {
        QColor c(Qt::white);
        QRect r = rect;

        for ( int i = 100; i > 0; i -= 10 )
        {
            r.setBottom(yMap.transform(i - 10));
            r.setTop(yMap.transform(i));
            painter->fillRect(r, c);

            c = c.dark(110);
        }
    }
};

class MainCurve: public QwtPlotCurve//主曲线画笔类
{
public:
    MainCurve(const QString &title):
        QwtPlotCurve(title)
    {
#if QT_VERSION >= 0x040000
        setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    }

    void setColor(const QColor &color)
    {
#if QT_VERSION >= 0x040000
        QColor c = color;
        c.setAlpha(150);

        setPen(c);
        setBrush(c);
#else
        setPen(color);
        setBrush(QBrush(color, Qt::Dense4Pattern));
#endif
    }
};

MainPlot::MainPlot(QWidget *parent)://主曲线类
    QwtPlot(parent),
    dataCount(0)
{
    setAutoReplot(false);

    plotLayout()->setAlignCanvasToScales(true);

    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::RightLegend);

    QTime curTime;
    curTime = curTime.currentTime();

    QFont ft;
    ft.setFamily("micro");
    ft.setPointSize(10);

    setAxisTitle(QwtPlot::xBottom, QObject::tr("时间[m:s]"));
    setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(curTime));
    setAxisFont (QwtPlot::xBottom, ft);
    ft.setPointSize(10);
    ft.setFamily("wenquanyi");
    setAxisFont (QwtPlot::yLeft, ft);
    setAxisFont (QwtPlot::yRight, ft);
    setAxisScale(QwtPlot::xBottom, 0, HISTORY);
    setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    /*
     In situations, when there is a label at the most right position of the
     scale, additional space is needed to display the overlapping part
     of the label would be taken by reducing the width of scale and canvas.
     To avoid this "jumping canvas" effect, we add a permanent margin.
     We don't need to do the same for the left border, because there
     is enough space for the overlapping label below the left scale.
     */

    /* QwtScaleWidget *scaleWidget = axisWidget(QwtPlot::xBottom);
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, fmh / 2);
   */
    // setAxisScale (QwtPlot::xBottom,10,10,10);
    setAxisTitle(QwtPlot::yLeft, QObject::tr("温度"));//温度轴
    setAxisScale(QwtPlot::yLeft, 0, 100);

    enableAxis( QwtPlot::yRight );
    setAxisTitle(QwtPlot::yRight,QObject::tr("湿度"));//湿度轴
    setAxisScale(QwtPlot::yRight,0,100);

    //    QwtPlotGrid *grid1= new QwtPlotGrid;//设置显示网格
    //    grid1->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
    //    grid1->attach(this);


    Background *bg = new Background();
    bg->attach(this);


    MainCurve *curve;

    curve = new MainCurve("T");
    curve->setColor(Qt::red);
    curve->attach(this);
    data[Temp].curve = curve;

    curve = new MainCurve("H");
    curve->setColor(Qt::blue);
    curve->setZ(curve->z() - 1);
    curve->attach(this);
    curve->setYAxis( QwtPlot::yRight );//设置为右坐标显示
    curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//抗锯齿
    data[Humi].curve = curve;
    /*
    //增加缩放功能
       QwtPlotZoomer  *zoomer= new QwtPlotZoomer(this->canvas());
         zoomer->setRubberBandPen( QColor( Qt::black ) );
         zoomer->setTrackerPen( QColor( Qt::black ) );
         zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
         zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
*/

//    smogMaker = new QwtPlotMarker();
//    smogMaker->setLineStyle( QwtPlotMarker::HLine);
//    smogMaker->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
//    //    smogMaker->setLabelAlignment( Qt::AlignLeft | Qt::AlignTop );
//    //       smogMaker->setLabelAlignment(  );
//    //    smogMaker->setLinePen( QColor( 200, 150, 0 ), 0, Qt::DashDotLine );
//    //    smogMaker->setSymbol( new QwtSymbol( QwtSymbol::Diamond,
//    //        QColor( Qt::yellow ), QColor( Qt::green ), QSize( 8, 8 ) ) );
//    smogMaker->attach( this );

    //showSmog(0,90);
    showCurve(data[Temp].curve, true);
    showCurve(data[Humi].curve, true);



    for ( int i = 0; i < HISTORY; i++ )
        timeData[HISTORY - 1 - i] = i;

    (void)startTimer(1000); // 1 second

//    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
//            SLOT(showCurve(QwtPlotItem *, bool)));
    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
        SLOT(showCurve(QwtPlotItem *, bool)));
}




//void MainPlot::showSmog( double x, double y )//显示烟雾状态方法
//{
//    QString label;
//    if(x==0)
//    {
//        //    label.sprintf(QObject::tr("烟雾正常"));
//        smogMaker->setValue( 0,y);
//        smogMaker->setLabel(QObject::tr("烟雾正常"));
//    }
//    else
//    {
//        //        label.sprintf(("有火！"));
//        smogMaker->setValue( 0,y);
//        smogMaker->setLabel(QObject::tr("发现烟雾!"));
//    }
    /*
    //QwtText text( label );
QwtText text(QObject::tr("正常"));
    text.setFont( QFont( "Helvetica", 18, QFont::Bold ) );
//    text.setColor( QColor( 200, 150, 0 ) );
    text.setColor( Qt::green );
//    QFont ft;
//    ft.setPointSize(12);

    smogMaker->setValue( 0,y);
//    smogMaker->setLabel( text );
smogMaker->setLabel(QObject::tr("正常"));
*/
//}


void MainPlot::timerEvent(QTimerEvent *)//时间事件
{
    for ( int i = dataCount; i > 0; i-- )
    {
        for ( int c = 0; c < NData; c++ )
        {
            if ( i < HISTORY )
            {
                data[c].data[i] = data[c].data[i-1];
//                qDebug() << data[c].data[i];
            }
        }
    }


    //showSmog(tempData::smogStatus,90);


    data[Temp].data[0]=tempData::tempValue;
    data[Humi].data[0]=tempData::humiValue;



    if ( dataCount < HISTORY )
        dataCount++;

    for ( int j = 0; j < HISTORY; j++ )
        timeData[j]++;

    setAxisScale(QwtPlot::xBottom,
                 timeData[HISTORY - 1], timeData[0]);

    for ( int c = 0; c < NData; c++ )
    {
        data[c].curve->setRawData(timeData, data[c].data, dataCount);

    }

    replot();
}

void MainPlot::showCurve(QwtPlotItem *item, bool on)//显示曲线
{
        item->setVisible(on);
        QWidget *w = legend()->find(item);
        if ( w && w->inherits("QwtLegendItem") )
            ((QwtLegendItem *)w)->setChecked(on);

        replot();
//    item->setVisible( on );

//    QwtLegend *lgd = qobject_cast<QwtLegend *>( legend() );

//    QList<QWidget *> legendWidgets =
//            lgd->legendWidgets( itemToInfo( item ) );

//    if ( legendWidgets.size() == 1 )
//    {
//        QwtLegendLabel *legendLabel =
//                qobject_cast<QwtLegendLabel *>( legendWidgets[0] );

//        if ( legendLabel )
//            legendLabel->setChecked( on );
//    }

//    replot();
}
