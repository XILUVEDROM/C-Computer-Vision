/*#include "analytics.h"
#include "ui_analytics.h"
#include "mainwindow.h"
#include <QPieSlice>
#include <QPieSeries>
#include <QChartView>
#include <QLabel>

QT_CHARTS_USE_NAMESPACE

Analytics::Analytics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Analytics)
{
    ui->setupUi(this);

    QStringList namesForFeatues = MainWindow::getFeatureForAnalytics();
    QVector<double> percForFeatures = MainWindow::getPercentageFeatureForAnalytics();

    ui->scrollArea->setWidgetResizable(true);



    if(namesForFeatues.size()>20){
        QMessageBox::critical(0,"Слишком много признаков", "При большом количестве признаков "
                                                           "они могут отображаться некорректно, "
                                                           "пожалуйста, используете текстовый аналог на главном экране");
    }


       //for specs

       QPieSeries *series = new QPieSeries();
       series->setName("Аналитика по признакам");
       for (int i=0;i<namesForFeatues.size();i++)
       {
           series->append(namesForFeatues.value(i),percForFeatures.value(i));
       }


       int counter=0;
       for(auto slice : series->slices()){

           slice->setLabel(namesForFeatues.value(counter) + " " + QString("%1%").arg(100*slice->percentage(), 0, 'f', 1));
           counter++;

       }

       //series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);
       series->setLabelsVisible();



       QChart *chart = new QChart();
       chart->setAnimationOptions(QChart::AllAnimations);
       chart->addSeries(series);
       chart->legend()->setAlignment(Qt::AlignRight);


       QChartView *chartView = new QChartView(chart);
       chartView->setRenderHint(QPainter::Antialiasing);
       ui->scrollArea->setWidget(chartView);
       chartView->show();



}

Analytics::~Analytics()
{
    delete ui;
}
