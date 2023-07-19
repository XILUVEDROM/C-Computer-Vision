#ifndef Obrabotka_H
#define Obrabotka_H

#include <QPoint>
#include <QVector>

struct Obrabotka
{
    quint64 id;
    QVector<QPoint> Points;
    QVector<QPoint> CPoints;
    Obrabotka() = default;

    explicit Obrabotka(quint64 number): id(number), Points(0), CPoints(0){}

    Obrabotka& operator=(const Obrabotka& obj)
    {
        if (this == &obj)
            return *this;
        id = obj.id;
        Points = obj.Points;
        CPoints = obj.CPoints;
        return *this;
    }

    void add_main(QPoint p)
    {
        Points.push_back(p);
    }

    void add_cont(QPoint p)
    {
        CPoints.push_back(p);
    }
};

#endif // Obrabotka_H
