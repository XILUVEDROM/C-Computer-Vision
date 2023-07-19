#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     ui->srcGrView->setScene(&scSrc);
     ui->dstGrView->setScene(&scDst);
}

QString Pert;

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    QString fp = QFileDialog::getOpenFileName();
    Pert = fp;

    gpiSrc = new MyGraphicsItem(QPixmap(fp));
    gpiDst = new MyGraphicsItem(QPixmap(fp));


    scSrc.addItem(gpiSrc);
    scDst.addItem(gpiDst);
}
void MainWindow::on_pushButton_clicked()
{
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    QString after;
    QSet <QString> colorh;
    int schn=0;

    int porog = ui->lineEdit-> text().toInt();

    bool mas[18];
    for(int i=0;i<18;i++)
        mas[i]=false;

      for (int i=0;i<tmpSrc.width();i++){
          for (int j=0;j<tmpSrc.height();j++){
              QRgb color =tmpSrc.pixel( i, j );
              QColor col =QColor(color);

              QString ng = col.name();
              QString nb = QColor(color).name();

              nb.remove(0,1);

              int d = nb.toInt(nullptr,16);
            //  cout<<"="<<ng<<endl;
              int red = qRed(tmpDst.pixel(i,j));
              int blue = qBlue(tmpDst.pixel(i,j));
              int green = qGreen(tmpDst.pixel(i,j));

              //cout << "Содержимое set: ";
              if (red<=blue+porog && green<=blue+porog )
              {
                  schn++;
                  tmpDst.setPixel(i,j,qRgb(255,0,0));
              }
              //copy(colorh.begin(), colorh.end(), ostream_iterator<QString>(cout));
              //for_each(colorh.begin(),colorh.end(),[](const auto& t){/*cout<<t.toStdString().c_str()<<' ';*/});
              //for_each(colorh.begin(),colorh.end(),[](const auto& t){if(t=="White"|| t=="Red"||t="Korichevyi"||t=="blue"||t=="black"||t=="s_grey") {Svet++;}});

             // Svet = svetu.size();
              gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

              }
        }

}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    QMatrix matrix;
    qreal m = qreal(value)/5;
    matrix.scale(m,m);
    ui->srcGrView->setMatrix(matrix);
     ui->dstGrView->setMatrix(matrix);
}
