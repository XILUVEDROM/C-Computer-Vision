/* #include "analyticforcyto.h"
#include "ui_analyticforcyto.h"
#include "mainwindow.h"
#include <QPieSlice>
#include <QPieSeries>
#include <QChartView>
#include <QLabel>

QT_CHARTS_USE_NAMESPACE

AnalyticForCyto::AnalyticForCyto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyticForCyto)
{
    ui->setupUi(this);
    QStringList namesForCyto = MainWindow::getCytoDiagnosisForAnalytic();
    QVector<double> percForCyto = MainWindow::getCytoPercentageDiagnosisForAnalytics();

    QPieSeries *series3 = new QPieSeries();
    series3->setName("Аналитика по диагнозам");
    for (int i=0;i<namesForCyto.size();i++)
    {
        series3->append(namesForCyto.value(i),percForCyto.value(i));
    }


    int counter3=0;
    for(auto slice : series3->slices()){

        slice->setLabel(namesForCyto.value(counter3) + " " + QString("%1%").arg(100*slice->percentage(), 0, 'f', 1));
        counter3++;

    }

    //series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);
    series3->setLabelsVisible();



    QChart *chart3 = new QChart();
    chart3->setAnimationOptions(QChart::AllAnimations);
    chart3->addSeries(series3);
    chart3->legend()->setAlignment(Qt::AlignRight);


    QChartView *chartView3 = new QChartView(chart3);
    chartView3->setRenderHint(QPainter::Antialiasing);
    ui->scrollArea->setWidget(chartView3);
    chartView3->show();

}

AnalyticForCyto::~AnalyticForCyto()
{
    delete ui;
}
