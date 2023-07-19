
QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestViborka
TEMPLATE = app
CONFIG += c++14
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        cvservice.cpp \
        main.cpp \
        widget.cpp

HEADERS += \
    cvservice.h \
    widget.h


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
