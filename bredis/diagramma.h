#ifndef DIAGRAMMA_H
#define DIAGRAMMA_H


#include <QWidget>

#include <QTableView>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QLineEdit>

#include <QImage>
#include <QGraphicsItem>
#include <algorithm>
#include <vector>

//#include "rsearch.h"
namespace Ui {
class Diagramma;
}

class Diagramma : public QWidget
{
    Q_OBJECT

public:
    explicit Diagramma(QWidget *parent = 0);
    ~Diagramma();

private:
    Ui::Diagramma *di;
    QSqlTableModel* model;
    QSqlDatabase db;
    //rsearch *RSEA;
};

#endif // DIAGRAMMA_H
