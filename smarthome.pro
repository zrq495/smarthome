#-------------------------------------------------
#
# Project created by QtCreator 2014-05-27T09:37:44
#
#-------------------------------------------------

QT       += core gui

TARGET = smarthome
TEMPLATE = app


SOURCES += main.cpp\
        mywidget.cpp \
    mainplot.cpp \
    zigbeetopo.cpp \
    server/cliect_td.cpp

HEADERS  += mywidget.h \
    mainplot.h \
    main.h \
    zigbeetopo.h \
    server/zigbee_struct.h \
    server/cliect_td.h \
    server/api.h

FORMS    += mywidget.ui

LIBS += -L /usr/local/qwt-5.2.4-svn/lib -lqwt
INCLUDEPATH += /usr/local/qwt-5.2.4-svn/include

OTHER_FILES +=

RESOURCES += \
    images/images.qrc
