/*#include "analyticforgysto.h"
#include "ui_analyticforgysto.h"
#include "mainwindow.h"
#include <QPieSlice>
#include <QPieSeries>
#include <QChartView>
#include <QLabel>

QT_CHARTS_USE_NAMESPACE

AnalyticForGysto::AnalyticForGysto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyticForGysto)
{
    ui->setupUi(this);

    QStringList namesForGysto = MainWindow::getGystoDiagnosisForAnalytic();
    QVector<double> percForGysto = MainWindow::getGystoPercentageDiagnosisForAnalytics();

    QPieSeries *series2 = new QPieSeries();
    series2->setName("Аналитика по диагнозам");
    for (int i=0;i<namesForGysto.size();i++)
    {
        series2->append(namesForGysto.value(i),percForGysto.value(i));
    }


    int counter2=0;
    for(auto slice : series2->slices()){

        slice->setLabel(namesForGysto.value(counter2) + " " + QString("%1%").arg(100*slice->percentage(), 0, 'f', 1));
        counter2++;

    }

    //series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);
    series2->setLabelsVisible();



    QChart *chart2 = new QChart();
    chart2->setAnimationOptions(QChart::AllAnimations);
    chart2->addSeries(series2);
    chart2->legend()->setAlignment(Qt::AlignRight);


    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    ui->scrollArea->setWidget(chartView2);
    chartView2->show();

}

AnalyticForGysto::~AnalyticForGysto()
{
    delete ui;
}
