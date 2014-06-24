#ifndef ZIGBEETOPO_H
#define ZIGBEETOPO_H

#include <QWidget>
#include <QProcess>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QMutex>
#include"server/api.h"
#include"server/cliect_td.h"
class ZigbeeTopo : public QThread
{
    Q_OBJECT
public:
    ZigbeeTopo();
    ~ZigbeeTopo();
    void run();
    QMutex mutex;
    Cliect *cliect_thread1;

private slots:
private:

};
#endif
