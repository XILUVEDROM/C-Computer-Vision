#include<locale.h>
#include "widget.h"
#include <QtWidgets/QApplication>
#include <clocale>
#include "cvservice.h"
#include <iostream>




int main(int argc, char* argv[])
{



    setlocale(LC_ALL, "RUS");

    QApplication a(argc, argv);
    Widget w;
    w.setMinimumSize(500, 500);
    w.show();


    return a.exec();
}
