#-------------------------------------------------
#
# Project created by QtCreator 2019-05-30T18:39:57
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = segmentSelection
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += C++14
QMAKE_CXXFLAGS += -std=c++14

INCLUDEPATH += $$PWD/open_cv/include

LIBS += $$PWD/open_cv/bin/libopencv_calib3d320.dll
LIBS += $$PWD/open_cv/bin/libopencv_core320.dll
LIBS += $$PWD/open_cv/bin/libopencv_features2d320.dll
LIBS += $$PWD/open_cv/bin/libopencv_highgui320.dll
LIBS += $$PWD/open_cv/bin/libopencv_imgcodecs320.dll
LIBS += $$PWD/open_cv/bin/libopencv_photo320.dll
LIBS += $$PWD/open_cv/bin/libopencv_imgproc320.dll
LIBS += $$PWD/open_cv/bin/libopencv_video320.dll
LIBS += $$PWD/open_cv/bin/libopencv_videoio320.dll

SOURCES += \
        algorithms.cpp \
        connectedcomponents.cpp \
        figure.cpp \
        label.cpp \
        main.cpp \
        mainwindow.cpp \
        miniview.cpp \
        paintscene.cpp \
        square.cpp

HEADERS += \
        algorithms.h \
        connectedcomponents.h \
        figure.h \
        imageview.h \
        label.h \
        mainwindow.h \
        miniview.h \
        paintscene.h \
        square.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
