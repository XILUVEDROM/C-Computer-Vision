#include "convertersvet.h"
#include "mainwindow.h"
#include <QSet>

using namespace std;

ConverterSvet::ConverterSvet()
{

}
QColor ConverterSvet::resolveColor(const QString & name)
{
    QColor color;
    if (QColor::isValidColor(name))
        color.setNamedColor(name);
    else
    {
        // trying to parse "#RRGGBBAA" color
        if (name.length() == 9)
        {
            QString solidColor = name.left(7);
            if (QColor::isValidColor(solidColor))
            {
                color.setNamedColor(solidColor);
                int alpha = name.right(2).toInt(0, 16);
                color.setAlpha(alpha);
            }
        }
    }

    if (!color.isValid())
        cout << QString("[ImageManager::resolveColor] Can\'t parse color: %1").arg(name).toStdString().c_str();

    return color;
}


void ConverterSvet::Converter()
{
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    QString after;
    QSet <QString> colorH;

    bool mas[18];
    for(int i=0;i<18;i++)
        mas[i]=false;

      for (int i=0;i<tmpSrc.width();i++){
          for (int j=0;j<tmpSrc.height();j++){
              QRgb color =tmpSrc.pixel( i, j );
              QColor col =QColor(color);

 for(int i=0;i<255;i++)
 {
              if(col==QColor(Qt::white).lighter(i))
              {
                  colorH.insert("White");
              }
              else if (col==QColor(Qt::black).lighter(i)) {
                  colorH.insert("Black");
              }
              else if (col==QColor(Qt::red).lighter(i)) {
                  colorH.insert("Red");
              }else if (col==QColor(Qt::darkRed).lighter(i)) {
                  colorH.insert("DarkRed");
              }else if (col==QColor(Qt::green).lighter(i)) {
                  colorH.insert("Green");
              }else if (col==QColor(Qt::darkGreen).lighter(i)) {
                  colorH.insert("DarkGreen");
              }else if (col==QColor(Qt::blue).lighter(i)) {
                  colorH.insert("Blue");
              }else if (col==QColor(Qt::darkBlue).lighter(i)) {
                  colorH.insert("DarkBlue");
              }else if (col==QColor(Qt::cyan).lighter(i)) {
                  colorH.insert("cyan");
              }else if (col==QColor(Qt::darkCyan).lighter(i)) {
                  colorH.insert("DarkCyan");
              }else if (col==QColor(Qt::magenta).lighter(i)) {
                  colorH.insert("Magenta");
              }else if (col==QColor(Qt::darkMagenta).lighter(i)) {
                  colorH.insert("DarkMagenta");
              }else if (col==QColor(Qt::yellow).lighter(i)) {
                  colorH.insert("Yellow");
              }else if (col==QColor(Qt::darkYellow).lighter(i)) {
                  colorH.insert("DarkYellow");
              }else if (col==QColor(Qt::gray).lighter(i)) {
                  colorH.insert("Grey");
              }else if (col==QColor(Qt::darkGray).lighter(i)) {
                  colorH.insert("DarkGrey");
              }else if(col==QColor(Qt::lightGray).lighter(i))
              {
                  colorH.insert("DarkGrey");
              }
              cout << "Содержимое set: ";

              //copy(colorh.begin(), colorh.end(), ostream_iterator<QString>(cout));
              //for_each(colorh.begin(),colorh.end(),[](const auto& t){cout<<t.toStdString().c_str()<<' ';});
              //for_each(colorh.begin(),colorh.end(),[](const auto& t){if(t=="White"|| t=="Red"||t="Korichevyi"||t=="blue"||t=="black"||t=="s_grey") {Svet++;}});
              }
        }
}
}
