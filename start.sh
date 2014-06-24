#!/bin/bash

/home/uptech/QTworkspace/for_arm/qt-everywhere-opensource-src-4.6.2/bin/qmake -project
/home/uptech/QTworkspace/for_arm/qt-everywhere-opensource-src-4.6.2/bin/qmake
make 
make clean

echo "LIBS += -L /home/uptech/QTworkspace/qwt/qwt-5.2.3/lib -lqwt" >> smarthome.pro
echo "INCLUDEPATH += /home/uptech/QTworkspace/qwt/qwt-5.2.3/src" >> smarthome.pro
make
