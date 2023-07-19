#-------------------------------------------------
#
# Project created by QtCreator 2019-03-14T18:04:38
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AwfulSqLiteProject
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dbworker.cpp \
    addimage.cpp \
    colon.cpp \
    colonfilter.cpp \
    diagnosisfilter.cpp \
    kidney.cpp \
    kidneyfilter.cpp \
    mammalgland.cpp \
    mammalglandfilter.cpp \
    mycustomview.cpp \
    imagemarkup.cpp \
    analytics.cpp \
    patientadder.cpp \
    login.cpp \
    analyticforcyto.cpp \
    analyticforgysto.cpp \
    markupadder.cpp

HEADERS += \
        mainwindow.h \
    dbworker.h \
    addimage.h \
    colon.h \
    colonfilter.h \
    diagnosisfilter.h \
    kidney.h \
    kidneyfilter.h \
    mammalgland.h \
    mammalglandfilter.h \
    mycustomview.h \
    imagemarkup.h \
    analytics.h \
    patientadder.h \
    login.h \
    analyticforcyto.h \
    analyticforgysto.h \
    markupadder.h

FORMS += \
        mainwindow.ui \
    addimage.ui \
    colon.ui \
    colonfilter.ui \
    diagnosisfilter.ui \
    kidney.ui \
    kidneyfilter.ui \
    mammalgland.ui \
    mammalglandfilter.ui \
    imagemarkup.ui \
    analytics.ui \
    patientadder.ui \
    login.ui \
    analyticforcyto.ui \
    analyticforgysto.ui \
    markupadder.ui
