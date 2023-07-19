QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#include(..\..\..\opencv\opencv.pri)
OPENCV_PATH = C:\Users\User\Documents\ActiveContours\opencv


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
