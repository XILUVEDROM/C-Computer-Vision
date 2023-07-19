#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QImage>

class Analys : public QObject
{
    Q_OBJECT
public:
    Analys(const QImage& img):
        bin(img), filePath(QString()) {}
    Analys(const QString& path):
        bin(QImage()), filePath(path) {}

    void select();

private:
    QImage bin;
    QString filePath;
};

#endif // CORE_H
