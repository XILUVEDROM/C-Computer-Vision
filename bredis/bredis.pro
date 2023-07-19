QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base.cpp \
    diagramma.cpp \
    figure.cpp \
    global.cpp \
    imagesetting.cpp \
    main.cpp \
    mainwindow.cpp \
    markerwindow.cpp \
    paintscene.cpp \
    patientsetting.cpp \
    romb.cpp \
    square.cpp \
    triangle.cpp \
    line.cpp


HEADERS += \
    base.h \
    diagramma.h \
    figure.h \
    global.h \
    imagesetting.h \
    mainwindow.h \
    markerwindow.h \
    paintscene.h \
    patientsetting.h \
    romb.h \
    square.h \
    triangle.h \  
    line.h

FORMS += \
    diagramma.ui \
    imagesetting.ui \
    mainwindow.ui \
    markerwindow.ui \
    patientsetting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
