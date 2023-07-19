#-------------------------------------------------
#
# Project created by QtCreator 2018-10-24T13:20:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled4
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

CONFIG += c++14

SOURCES += \
    anizo.cpp \
        main.cpp \
        mainwindow.cpp \
    mygraphicsitem.cpp \
    original.cpp \
    otchetalddiv.cpp \
    otchetx3.cpp \
    analys.cpp \
    obrabotkaalg.cpp \
    resultfound.cpp \
    rgb2hsv.cpp \
    convertersvet.cpp \
    error.cpp \
    centrmass.cpp \
    countour.cpp \
    imagobject.cpp

HEADERS += \
    anizo.h \
        mainwindow.h \
    mygraphicsitem.h \
    original.h \
    otchetalddiv.h \
    otchetx3.h \
    analys.h \
    obrabotkaalg.h \
    resultfound.h \
    rgb2hsv.h \
    convertersvet.h \
    error.h \
    centrmass.h \
    countour.h \
    imagobject.h

FORMS += \
    anizo.ui \
        mainwindow.ui \
    original.ui \
    otchetalddiv.ui \
    otchetx3.ui \
    error.ui \
    resultfound.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#include(..\..\..\opencv\opencv.pri)
OPENCV_PATH = C:\test-melanoma-C13-301-2019\opencv


INCLUDEPATH += open_cv/include

LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_calib3d320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_core320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_features2d320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_highgui320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgcodecs320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_photo320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgproc320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_video320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_videoio320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_ml320.dll
