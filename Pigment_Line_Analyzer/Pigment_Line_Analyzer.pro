#-------------------------------------------------
#
# Project created by QtCreator 2018-11-14T19:50:50
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Pigment_Line_Analyzer
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

CONFIG += c++11

INCLUDEPATH += open_cv/include

LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_calib3d320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_core320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_features2d320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_highgui320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgcodecs320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_photo320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgproc320.dll

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    viewer.cpp \
    imagematrix.cpp \
    skelet/skiletizationfilter.cpp \
    skelet/utils.cpp \
    processing.cpp \
    features.cpp \
    qcustomplot.cpp \
    base.cpp \
    form.cpp

HEADERS += \
        mainwindow.h \
    viewer.h \
    imagematrix.h \
    skelet/skiletizationfilter.h \
    skelet/utils.h \
    processing.h \
    features.h \
    qcustomplot.h \
    base.h \
    form.h

FORMS += \
        mainwindow.ui \
    form.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
