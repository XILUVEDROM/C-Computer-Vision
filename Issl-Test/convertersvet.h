#ifndef CONVERTERSVET_H
#define CONVERTERSVET_H
#include <QColor>
#include <iostream>
#include "mainwindow.h"

class ConverterSvet
{
public:
    ConverterSvet();
    QColor resolveColor(const QString & name);
    void Converter();

    MyGraphicsItem *gpiDst;
};

#endif // CONVERTERSVET_H
