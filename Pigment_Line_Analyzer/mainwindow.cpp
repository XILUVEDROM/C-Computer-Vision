#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowState(Qt::WindowMaximized);
    //this->createMenu();
    this->createTool();
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    regen = new QCPBars(ui->widget->xAxis, ui->widget->yAxis);
    colorScale = new QCPColorScale(ui->widget);
    colorScale->setType(QCPAxis::atBottom);
    colorScale->axis()->setTicks(false);
    colorScale->setDataRange(QCPRange(0, 255));
    ui->widget->plotLayout()->addElement(1, 0, colorScale);
    cp = new QCustomPlot(this);
    cp->hide();
    colorMap = new QCPColorMap(cp->xAxis, cp->yAxis);
    colorMap->setColorScale(colorScale);
    colorMap->setGradient(QCPColorGradient::gpSpectrum);
    colorMap->keyAxis()->setTicks(false);
    colorMap->valueAxis()->setTicks(false);
    db = new Base();
    form = new Form();
    db->connectToDataBase();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(process()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(removeSmall()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(findSegments()));
    connect(this, SIGNAL(loopCount(int)), this, SLOT(changeProgressBar(int)));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(saveFeatures()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(prepareHistogram()));
    connect(ui->widget, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)),
            this, SLOT(mousePressOnPlot(QCPAbstractPlottable*, int, QMouseEvent*)));
    ui->comboBox->addItems(QStringList() << "Длина" << "Толщина" << "Угол наклона" <<
                           "Минимум R" << "Средний R" << "Максимум R" <<
                           "Минимум G" << "Средний G" << "Максимум G" <<
                           "Минимум B" << "Средний B" << "Максимум B");

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

    if (!QDir().exists("base/"))
        QDir().mkpath("base/");
    if (!QFile().exists("base/statistics.csv"))
    {
        QFile f ("base/statistics.csv");
        f.open(QIODevice::WriteOnly);
        QTextStream stream_f(&f);
        stream_f << "image_file_name,sigmaD,sigmaI,threshold,min_element_size;\n"
                    << "sector's_number,segments'_count,length_EV,length_SD,thickness_EV,thickness_SD,"
                         "minR_EV,minR_SD,meanR_EV,meanR_SD,maxR_EV,maxR_SD,"
                         "minG_EV,minG_SD,meanG_EV,meanG_SD,maxG_EV,maxG_SD,"
                         "minB_EV,minB_SD,meanB_EV,meanB_SD,maxB_EV,maxB_SD;\n"
                    << "total,length_EV,length_SD,thickness_EV,thickness_SD,"
                        "minR_EV,minR_SD,meanR_EV,meanR_SD,maxR_EV,maxR_SD,"
                        "minG_EV,minG_SD,meanG_EV,meanG_SD,maxG_EV,maxG_SD,"
                        "minB_EV,minB_SD,meanB_EV,meanB_SD,maxB_EV,maxB_SD;";
        f.close();
    }
}

MainWindow::~MainWindow()
{
    delete db;
    delete form;
    for (int i = 0; i < ui->widget->plottableCount(); ++i)
        ui->widget->removePlottable(0);
    for (int i = 0; i < cp->plottableCount(); ++i)
        cp->removePlottable(0);
    delete ui;
}

void MainWindow::createMenu()
{
    QFont font;
    //font.setWeight(QFont::Medium);
    font.setPointSize(12);
    QMenu* mn_file = new QMenu("Файл", this);
    mn_file->setFont(font);
    QAction *mn_action = new QAction("Открыть",this);
    mn_action->setFont(font);
    mn_action->setShortcut(tr("Ctrl+O"));
    connect(mn_action, SIGNAL(triggered()), this, SLOT(openImage()));
    mn_file->addAction(mn_action);
    ui->menuBar->addMenu(mn_file);
}

void MainWindow::createTool()
{
    QFont font;
    font.setPointSize(12);

    QAction *mn_action = new QAction("Открыть",this);
    mn_action->setFont(font);
    connect(mn_action, SIGNAL(triggered()), this, SLOT(openImage()));
    ui->mainToolBar->addAction(mn_action);

    QAction *show_contour = new QAction("Показать контуры", this);
    show_contour->setFont(font);
    show_contour->setCheckable(true);
    show_contour->setEnabled(false);
    connect(show_contour, SIGNAL(triggered(bool)), this, SLOT(showContour(bool)));
    ui->mainToolBar->addAction(show_contour);

    QAction *show_mask = new QAction("Показать маску", this);
    show_mask->setFont(font);
    show_mask->setCheckable(true);
    show_mask->setEnabled(false);
    connect(show_mask, SIGNAL(triggered(bool)), this, SLOT(showMask(bool)));
    ui->mainToolBar->addAction(show_mask);

    QAction *show_map = new QAction("Показать распределение величин", this);
    show_map->setFont(font);
    show_map->setCheckable(true);
    show_map->setEnabled(false);
    connect(show_map, SIGNAL(triggered(bool)), this, SLOT(showMap(bool)));

    QAction *search_form = new QAction("Поиск", this);
    search_form->setFont(font);
    ui->mainToolBar->addAction(show_map);
    connect(search_form, SIGNAL(triggered()), this, SLOT(openForm()));
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(search_form);
}

void MainWindow::calculateSymmetry()
{
    vector<QPoint> centers;
    vector<int> xs;
    vector<int> ys;

    //поиск центров сегментов
    for (QString &segm_point: points)
    {
        vector<Point> sp;
        QStringList lst = segm_point.split(";");
        for (QString &str: lst)
            sp.push_back(Point(str.split(",").at(1).toInt(), str.split(",").at(0).toInt()));
        RotatedRect rect = minAreaRect(sp);
        centers.push_back(QPoint(int(rect.center.x), int(rect.center.y)));
    }
    for (QPoint &p : centers)
    {
        xs.push_back(p.x());
        ys.push_back(p.y());
    }

    int w = dst.width(); int h = dst.height();
    //QPoint center (accumulate(xs.begin(), xs.end(), 0)/xs.size(), accumulate(ys.begin(), ys.end(), 0)/ys.size());
    QPoint center (w/2, h/2);

    //разбиение изображения на 8 секторов
    vector<QPolygon> pies;
    pies.push_back(QPolygon() << QPoint(center.x(), 0) << QPoint(w, center.y() - w + center.x()) << center);
    pies.push_back(QPolygon() << QPoint(w, center.y() - w + center.x()) << QPoint(w, center.y()) << center);
    pies.push_back(QPolygon() << QPoint(w, center.y()) << QPoint(w, center.y() + w - center.x()) << center);
    pies.push_back(QPolygon() << QPoint(w, center.y() + w - center.x()) << QPoint(center.x(), h) << center);
    pies.push_back(QPolygon() << QPoint(center.x(), h) << QPoint(0, center.y() + center.x()) << center);
    pies.push_back(QPolygon() << QPoint(0, center.y() + center.x()) << QPoint(0, center.y()) << center);
    pies.push_back(QPolygon() << QPoint(0, center.y()) << QPoint(0, center.y() - center.x()) << center);
    pies.push_back(QPolygon() << QPoint(0, center.y() - center.x()) << QPoint(center.x(), 0) << center);

    //расчет по каждому сектору среднего и СКО значний каждого признака
    vector<vector<pair<int, int>>> result_vector;
    vector<int> segm_count (8, 0);
    size_t c = 0;
    for (QPolygon &p : pies)
    {
//        scene->addPolygon(p, QPen(Qt::yellow));
        vector<vector<int>> points_mean (12);                  // 12 - number of features
        for (size_t i = 0; i < centers.size(); ++i)
            if (p.containsPoint(centers[i], Qt::WindingFill))
            {
                segm_count.at(c) += 1;
                for (size_t j = 0; j < features[i].size(); ++j)
                    points_mean.at(j).push_back(features[i][j]);
            }
        vector<pair<int, int>> vp;
        for (size_t i = 0; i < points_mean.size(); ++i)
        {
            if (points_mean[i].size() != 0)
            {
                long long sm = accumulate(points_mean[i].begin(), points_mean[i].end(), 0);
                int m = int(sm/points_mean[i].size());
                int s = 0;
                for (int &a : points_mean[i])
                    s += (a-m)*(a-m);
                s /= points_mean[i].size();
                vp.push_back(pair<int, int>(m, int(sqrt(s))));
            }
            else
                vp.push_back(pair<int, int>(-1, -1));
        }
        ++c;
        result_vector.push_back(vp);
    }
//    qDebug() << segm_count;
//    qDebug() << endl;
//    for (vector<pair<int, int>> &vp : result_vector)
//        qDebug() << vp;
//    qDebug() << endl;

    //расчет среднего по всем секторам для каждого признака
    vector<vector<int>> averages (12);                          //12 - number of features
    for (size_t i = 0; i < result_vector.size(); ++i)
        for (size_t j = 0; j < result_vector[i].size(); ++j)
            averages[j].push_back(result_vector[i][j].first);
    vector<pair<int, int>> summary;
    for (vector<int> &v : averages)
    {
        int sm = accumulate(v.begin(), v.end(), 0);
        int m = int(sm/v.size());
        int s = 0;
        for (int &k : v)
            s += (k-m)*(k-m);
        s /= v.size();
        summary.push_back(pair<int, int>(m, int(sqrt(s))));
    }
//    qDebug() << summary;
//    qDebug() << endl;

    //сохранение в файл
    QStringList sl = file_name.split("/");
    QFile f ("base/statistics.csv");
    f.open(QIODevice::Append);
    QTextStream stream_f(&f);
    stream_f << "\n\n"+*(sl.end()-1)+","+ui->sigmaD->text()+","+ui->sigmaI->text()+","+ui->threshold->text()+","+ui->minSize->text()+";";
    for (size_t i = 0; i < result_vector.size(); ++i)
    {
        stream_f << "\n" << i+1 << "," << QString::number(segm_count[i]) << ",";
        for (size_t j = 0; j < result_vector[i].size(); ++j)
            if (j != 2)
            {
                stream_f << result_vector[i][j].first << "," << result_vector[i][j].second;
                if (j != result_vector[i].size() - 1)
                    stream_f << ",";
                else
                    stream_f << ";";
            }
    }
    stream_f << "\ntotal,";
    for (size_t i = 0; i < summary.size(); ++i)
        if (i != 2)
        {
            stream_f << summary[i].first << "," << summary[i].second;
            if (i != summary.size() - 1)
                stream_f << ",";
            else
                stream_f << ";";
        }
    f.close();
}

void MainWindow::openImage()
{
    scene->clear();
    reprocess = false;
    ui->threshold->clear();
    //ui->widget->clearPlottables();
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->mainToolBar->actions().at(1)->setEnabled(false);
    ui->mainToolBar->actions().at(2)->setEnabled(false);
    ui->mainToolBar->actions().at(3)->setEnabled(false);
    file_name = QFileDialog::getOpenFileName();
    if (!file_name.isEmpty())
    {
        QImage image(file_name);
        if (image.isNull())
            QMessageBox::information(this, tr("Предупреждение"), tr("Невозможно загрузить изображение %1.").arg(file_name));
        else
        {
            img.load(file_name);
            img = img.convertToFormat(QImage::Format_RGB888);
            proc = img; proc_s = img; buf = img; dst = img;
            scene->addPixmap(QPixmap::fromImage(img));
            ui->pushButton->setEnabled(true);
        }
    }
}

/*!
 * \brief Предобработка изображения
 *
 * Изображение последовательно обрабатывается фильтрами, бинаризуется, скелетизируется.
 * Результаты сохраняются в уферные изображения.
 */
void MainWindow::process()
{
    ImageMatrix m1 (img);
    Processing pr;
    pr.bilateralFilter(m1, 2, ui->sigmaD->text().toInt(), ui->sigmaI->text().toInt());
    changeProgressBar(25);
    pr.matchedFiler(m1);
    changeProgressBar(50);
    int t;
    if (!reprocess)
    {
        pr.otsuThreshold(m1, t);
        ui->threshold->setText(QString::number(t));
        changeProgressBar(75);
    }
    else
    {
        pr.threshold(m1, ui->threshold->text().toInt());
        changeProgressBar(75);
    }
    pr.dilatation(m1, 1);
    pr.erosion(m1, 1);
    pr.erosion(m1, 1);
    if (m1.isWhite())
    {
        reprocess = true;
        QMessageBox::information(this, tr("Предупреждение"), tr("Пожалуйста, измените порог бинаризации и попробуйте снова, "
                                                                "пока результат не будет удовлетворительным."));
        m1.to_image(proc);
        pr.cutOrigin(buf, proc);
        scene->addPixmap(QPixmap::fromImage(buf));
        return;
    }
    reprocess = true;
    m1.to_image(proc);
    proc_s = proc;
    QImage tmp (proc);
    pr.laplace(m1);
    m1.to_image(tmp);
    SkiletizationFilter skelet;
    skelet.doFilter(proc_s);
    changeProgressBar(100);
    buf = img;
    for (int i = 0; i < buf.width(); ++i)
        for (int j = 0; j < buf.height(); ++j)
            if (tmp.pixelColor(i, j) == QColor(255,255,255))
                buf.setPixelColor(i, j, QColor(255,255,255));
    for (int i = 0; i < buf.width(); ++i)
        for (int j = 0; j < buf.height(); ++j)
            if (proc_s.pixelColor(i, j) == QColor(255,255,255))
                buf.setPixelColor(i, j, QColor(0,0,0));
    scene->addPixmap(QPixmap::fromImage(buf));
    ui->mainToolBar->actions().at(1)->setEnabled(true);
    ui->mainToolBar->actions().at(1)->setChecked(true);
    ui->pushButton_5->setEnabled(true);
}

/*!
 * \brief Удаление мелких (шумовых) сегментов
 */
void MainWindow::removeSmall()
{
    ui->progressBar->setValue(0);
    ImageMatrix m1 (proc);
    ImageMatrix m2 (proc_s);
    Processing pr;
    pr.removeSmallOnMask(m1, m2, ui->minSize->text().toInt());
    QImage tmp (proc);
    QImage tmp_2 (proc);
    m1.to_image(tmp);
    dst = img;
    pr.cutOrigin(dst, tmp);
    tmp.save(file_name.split(".").at(0)+"_mask_2.bmp");
    dst.save(file_name.split(".").at(0)+"_mask.bmp");
    pr.laplace(m1);
    m1.to_image(tmp);
    m2.to_image(tmp_2);
    buf = img;
    for (int i = 0; i < buf.width(); ++i)
        for (int j = 0; j < buf.height(); ++j)
            if (tmp.pixelColor(i, j) == QColor(255,255,255))
                buf.setPixelColor(i, j, QColor(255,255,255));
    for (int i = 0; i < buf.width(); ++i)
        for (int j = 0; j < buf.height(); ++j)
            if (tmp_2.pixelColor(i, j) == QColor(255,255,255))
                buf.setPixelColor(i, j, QColor(0,0,0));
    scene->addPixmap(QPixmap::fromImage(buf));
    pr.removeCross(m2);
    pr.removeSmallOnSkelet(m2, 5);
    m2.to_image(tmp_2);
    tmp_2.save(file_name.split(".").at(0)+"_skelet_without_small.bmp");
    ui->mainToolBar->actions().at(1)->setChecked(true);
    ui->mainToolBar->actions().at(2)->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}

/*!
 * \brief Сегментирование линий и расчет характеристик найденных сегментов
 */
void MainWindow::findSegments()
{
    cv::Mat src = cv::imread(file_name.split(".").at(0).toStdString()+"_mask_2.bmp");
    cv::Mat dist = cv::imread(file_name.split(".").at(0).toStdString()+"_skelet_without_small.bmp");
    cv::Mat origg = cv::imread(file_name.split(".").at(0).toStdString()+"_mask.bmp");
    cv::Mat bw_1;
    cv::cvtColor(dist, bw_1, CV_BGR2GRAY);
    cv::Mat dist_8u;
    bw_1.convertTo(dist_8u, CV_8U);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int ncomp = contours.size();

    int Kolca = ui->lineEdit->text().toFloat();
    int Sektor = ui->lineEdit_2->text().toFloat();
    int masSch[4][8];
    int mas[4][8] ;
    int Sch1=0,Sch2=0, Sx=0, Sy=0, IUg=0, Tfa=0;
    float Ugol=0, ObobSr=0, ObobOtkl =0, Obobk=0;

    for(int i=0;i<4;i++){
        for(int j=0;j<8;j++)
        {
            mas[i][j]=0;
        masSch[i][j]=0;
        }
    }


    cv::Mat markers = cv::Mat::zeros(bw_1.size(), CV_32SC1);
    for (int i = 0; i < ncomp; i++)
        cv::drawContours(markers, contours, i, cv::Scalar::all(i+1), -1);
    for (int i = 0; i < src.rows; ++i)
        for (int j = 0; j < src.cols; ++j)
            if (src.at<cv::Vec3b>(i,j) == cv::Vec3b(0,0,0))
                markers.at<int>(i,j) = -1;
    cv::watershed(src, markers);
    for (int i = 0; i < src.rows; ++i)
        for (int j = 0; j < src.cols; ++j)
            if (src.at<cv::Vec3b>(i,j) == cv::Vec3b(0,0,0))
                markers.at<int>(i,j) = -1;

    vector<vector<Point>> segms (ncomp);
    for (int i = 0; i < markers.rows; ++i)
        for (int j = 0; j < markers.cols; ++j)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= ncomp)
                segms.at(index-1).push_back(Point(i,j));
        }
    points.clear(); features.clear();
    int c = 2;
    for (vector<Point> segm : segms)
        if (segm.size()>0)
        {
            int min_x = (*(std::min_element(segm.begin(), segm.end(), [](Point p1, Point p2){return p1.x<p2.x;}))).x - 5;
            int min_y = (*(std::min_element(segm.begin(), segm.end(), [](Point p1, Point p2){return p1.y<p2.y;}))).y - 5;
            int max_x = (*(std::max_element(segm.begin(), segm.end(), [](Point p1, Point p2){return p1.x<p2.x;}))).x + 5;
            int max_y = (*(std::max_element(segm.begin(), segm.end(), [](Point p1, Point p2){return p1.y<p2.y;}))).y + 5;
            QString pois = "";
            Mat s = Mat::zeros(max_x-min_x+1, max_y-min_y+1, CV_8UC3);
            Sch1 =0; Sch2=0;
            for (Point p : segm)
            {
                s.at<Vec3b>(p.x-min_x, p.y-min_y) = origg.at<Vec3b>(p.x, p.y);
                pois += QString::number(p.x) + "," + QString::number(p.y) + ";";
                Sch1 +=p.x;
                Sch2 +=p.y;
            }

            int CenX = Sch2/segm.size();
            int CenY = Sch1/segm.size();


            Sx= sqrt ((centrx-CenX)*(centrx-CenX)+(centry-CenY)*(centry-CenY));
            qDebug()<< "centrx = " << centrx << "CenX = "<< CenX << "Centry = "<< centry<< "CenY = "<< CenY;
            qDebug()<< "Sx =" << Sx<< "RadiusCircle = " <<RadiusCircle;
            Sy= Sx/RadiusCircle;  // индекс кольца
            if (Sy >=Kolca)
            {
                Sy =Kolca-1;
            }
            if (centrx-CenX != 0)
            {
                Ugol=atan(float(centry-CenY)/float(centrx-CenX));

               Ugol = Ugol*180/3.1415926;
                if(centrx-CenX>0)
                {
                    Ugol+=180;
                }
            }
            else
            {
                if (centrx-CenX < 0)
                {
                    Ugol = 90;
                }
                else
                {
                    Ugol = 270;
                }
            }
            qDebug()<<"Sy = " << Sy;
            IUg = int(Ugol*Sektor/(float)360); // индекс уголового сектора
            if (IUg >=Sektor)
            {
                IUg =Sektor-1;
            }

            std::string p = pois.toStdString();
            p.erase(p.end()-1);
            pois = QString::fromStdString(p);
            Features f;

            vector<int> feat;
            f.calculate(s, feat);
            qDebug()<<"Ugol = "<< Ugol;
            mas[Sy][IUg] += feat[0];
            qDebug()<< "mas = "<< mas[Sy][IUg]<< "Sy = "<< Sy<< "IUg = "<< IUg;
            masSch[Sy][IUg] +=1;

            ObobSr += feat[0];
            ObobOtkl += feat[0]*feat[0];


            points.push_back(pois);
            features.push_back(feat);
            ++c;
            emit loopCount(c*100/segms.size());
            Tfa++;
        }
    ObobSr = ObobSr/Tfa;
    ObobOtkl =sqrt( -(ObobSr*ObobSr)+(ObobOtkl/Tfa));
    Obobk = ObobOtkl/ObobSr;

    float Kcircle[4];
    for (int i=0;i<4;i++)
    {
        Kcircle[i] =0.0;
    }
    float Kas=0;

    for (int i=0;i<Kolca;i++)
    {
        for (int j=0;j<Sektor;j++)
        {
            if(masSch[i][j]!=0)
            {
            mas[i][j] = mas[i][j]/masSch[i][j];
            }
            else
            {
                mas[i][j]=0;
            }
        }
    }

    for (int i=0; i<Kolca; i++)
    {
        float spo =0, sr = 0;

        for (int j=0; j<Sektor; j++)
       {
            sr = sr+mas[i][j];
        }
        sr=sr/Sektor;
        for(int j=0;j<Sektor;j++)
        {
            spo=spo+((mas[i][j]-sr)*(mas[i][j]-sr));
        }
        spo=sqrt(spo);
        if(sr!=0)
        {
           Kcircle[i]= spo/sr;
        }
        else
        {
            Kcircle[i] =0;
        }
    }

    for(int i=0;i<Kolca;i++)
    {
        Kas+=Kcircle[i];
    }

    Kas= Kas/Kolca;

    ui->textEdit->append("Коэффициент ассиметрии = " + QString::number(Kas));

    ui->textEdit->append("по кольцам = ");

    for (int i=0;i<Kolca;i++)
    {
        ui->textEdit->append(QString::number(Kcircle[i]));
    }

    ui->textEdit->append("Обобщенный коэффициент = "+ QString::number(Obobk));
    ui->progressBar->setValue(0);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    QFile(file_name.split(".").at(0)+"_mask.bmp").remove();
    QFile(file_name.split(".").at(0)+"_mask_2.bmp").remove();
    QFile(file_name.split(".").at(0)+"_skelet_without_small.bmp").remove();

}

/*!
 * \brief Сохранение расчитанных характеристик
 *
 * Сохранение в базу данных работает дольше, для проведения экспериментального исследования неудобно,
 * поэтому реалиpовано сохранение в csv-таблицы.
 */
void MainWindow::saveFeatures()
{
    calculateSymmetry();

    QStringList sl = file_name.split("/");
    QString tmp = *(sl.end()-1);
    img.save("base/"+tmp);
    if (!QDir().exists("base/"+tmp.split(".").at(0)))
        QDir().mkpath("base/"+tmp.split(".").at(0));
    QString result = ui->sigmaD->text()+"_"+ui->sigmaI->text()+"_"+ui->threshold->text()+"_"+ui->minSize->text();
    //img.save("base/"+tmp.split(".").at(0)+"/"+result+".bmp");
    dst.save("base/"+tmp.split(".").at(0)+"/"+result+"_mask.bmp");
    buf.save("base/"+tmp.split(".").at(0)+"/"+result+"_cont.bmp");
    QFile f ("base/"+tmp.split(".").at(0)+"/"+result+"_features.csv");
    QFile s ("base/"+tmp.split(".").at(0)+"/"+result+"_segments.csv");
    db->insertIntoDataBase(tmp, result+".bmp");
    f.open(QIODevice::WriteOnly);
    s.open(QIODevice::WriteOnly);
    QTextStream stream_f(&f);
    QTextStream stream_s(&s);
    stream_f << "length,thickness,minR,meanR,maxR,minG,meanG,maxG,minB,meanB,maxB;";
    stream_s << "points;";
    f.close(); s.close();
    f.open(QIODevice::Append); s.open(QIODevice::Append);
    for (size_t i = 0; i < points.size(); ++i)
    {
        stream_f << "\n"+QString::number(features[i][0])+","+QString::number(features[i][1])+","+
                QString::number(features[i][3])+","+QString::number(features[i][4])+","+
                QString::number(features[i][5])+","+QString::number(features[i][6])+","+
                QString::number(features[i][7])+","+QString::number(features[i][8])+","+
                QString::number(features[i][9])+","+QString::number(features[i][10])+","+
                QString::number(features[i][11])+";";
        QString tmp_str = points[i];
        tmp_str.replace(';','+');
        stream_s << "\n"+tmp_str+";";
        emit loopCount(i*100/points.size());
    }
    f.close(); s.close();
    ui->progressBar->setValue(0);
}

/*!
 * \brief Функция отрисовки гистограммы
 */
void MainWindow::prepareHistogram()
{
    int parameter = ui->comboBox->currentIndex();
    int step = ui->spinBox->value();
    vector<int> meanings;
    for (vector<int> v : features)
        meanings.push_back(v[parameter]);
    int minim = *(min_element(meanings.begin(), meanings.end()));
    int maxim = *(max_element(meanings.begin(), meanings.end()));
    size_t size = size_t(ceil((maxim-minim+1)/(step+1)));
    blocks.clear();
    ranges.clear();
    blocks.resize(size);
    ranges.resize(size);
    QVector<double> ticks;
    QVector<QString> labels;
    int tmp = minim;
    for (size_t i = 0; i < size; ++i)
    {
        for (int j = 0; j < step+1; ++j)
        {
            ranges[i].push_back(tmp);
            for (size_t k = 0; k < meanings.size(); ++k)
                if (meanings[k] == tmp)
                    blocks[i].push_back(points[k]);
            ++tmp;
        }
        ticks << i;
        labels << QString::number(*ranges[i].begin())+"-"+QString::number(*(ranges[i].end()-1));
    }
    int min_y = (*(min_element(blocks.begin(), blocks.end(),
                               [](vector<QString> s1, vector<QString> s2){return s1.size()<s2.size();}))).size();
    int max_y = (*(max_element(blocks.begin(), blocks.end(),
                               [](vector<QString> s1, vector<QString> s2){return s1.size()<s2.size();}))).size();
    regen->setAntialiased(false);
    regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    regen->setBrush(QColor(0, 168, 140));
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->widget->xAxis->setTicker(textTicker);
    ui->widget->xAxis->setTickLabelRotation(-90);
    ui->widget->xAxis->setSubTicks(false);
    ui->widget->xAxis->setTickLength(0, 0);
    QFont font;
    font.setPointSize(10);
    ui->widget->xAxis->setTickLabelFont(font);
    ui->widget->xAxis->setRange(minim-step, maxim+step);
    ui->widget->xAxis->setBasePen(QPen(Qt::black));
    ui->widget->xAxis->setTickPen(QPen(Qt::black));
    ui->widget->xAxis->grid()->setVisible(true);
    ui->widget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->widget->xAxis->setTickLabelColor(Qt::black);
    ui->widget->xAxis->setLabelColor(Qt::black);

    ui->widget->yAxis->setRange(min_y-10, max_y+10);
    ui->widget->yAxis->setLabel("Количество сегментов");
    ui->widget->yAxis->setBasePen(QPen(Qt::black));
    ui->widget->yAxis->setTickPen(QPen(Qt::black));
    ui->widget->yAxis->setSubTickPen(QPen(Qt::black));
    ui->widget->yAxis->grid()->setSubGridVisible(true);
    ui->widget->yAxis->setTickLabelColor(Qt::black);
    ui->widget->yAxis->setTickLabelFont(font);
    ui->widget->yAxis->setLabelColor(Qt::black);
    ui->widget->yAxis->setLabelFont(font);
    ui->widget->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->widget->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    QVector<double> regenData;
    for (vector<QString> &str : blocks)
        regenData << str.size();
    regen->setData(ticks, regenData);

    ui->widget->legend->setVisible(true);
    ui->widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->widget->legend->setBrush(QColor(0, 0, 0, 100));
    ui->widget->legend->setBorderPen(Qt::NoPen);
//    QFont legendFont;
//    legendFont.setPointSize(14);
//    ui->widget->legend->setFont(legendFont);
//    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->widget->rescaleAxes();
    ui->widget->replot();

    ui->mainToolBar->actions().at(3)->setEnabled(true);

}

void MainWindow::changeProgressBar(int i)
{
    ui->progressBar->setValue(i);
    qApp->processEvents();
}

/*!
 * \brief Обработка сигнала нажатия на столбец диаграммы
 * \param dataIndex - индекс выбранного столбца гистограммы
 */
void MainWindow::mousePressOnPlot(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event )
{
    ui->mainToolBar->actions().at(1)->setChecked(false);
    ui->mainToolBar->actions().at(2)->setChecked(false);
    ui->mainToolBar->actions().at(3)->setChecked(false);
    QImage img_2 (dst);
    for (QString &str : blocks[dataIndex])
    {
        QStringList lst = str.split(";");
        for (QString &s : lst)
        {
            int x = s.split(",").at(0).toInt();
            int y = s.split(",").at(1).toInt();
            img_2.setPixelColor(y, x, Qt::white);
        }
    }
    scene->addPixmap(QPixmap::fromImage(img_2));

}

void MainWindow::showContour(bool flag)
{
    if (flag)
    {
        ui->mainToolBar->actions().at(2)->setChecked(false);
        ui->mainToolBar->actions().at(3)->setChecked(false);
        scene->addPixmap(QPixmap::fromImage(buf));
    }
    else
        scene->addPixmap(QPixmap::fromImage(img));
}

void MainWindow::showMask(bool flag)
{
    if (flag)
    {
        ui->mainToolBar->actions().at(1)->setChecked(false);
        ui->mainToolBar->actions().at(3)->setChecked(false);
        scene->addPixmap(QPixmap::fromImage(dst));
    }
    else
        scene->addPixmap(QPixmap::fromImage(img));
}

void MainWindow::showMap(bool flag)
{
    if (flag)
    {
        ui->mainToolBar->actions().at(1)->setChecked(false);
        ui->mainToolBar->actions().at(2)->setChecked(false);
        int w = dst.width();
        int h = dst.height();
        cp->setGeometry(0, 0, w, h);
        colorMap->data()->setSize(w, h);
        colorMap->data()->setRange(QCPRange(0, w), QCPRange(0, h));
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j)

                colorMap->data()->setCell(i, j, 0);
        int i = 10;

        for (const vector<QString> &vect : blocks)
        {
            for (const QString &str : vect)
            {
                QStringList lst = str.split(";");
                for (const QString &s : lst)
                {
                    int x = s.split(",").at(0).toInt();
                    int y = s.split(",").at(1).toInt();
                    colorMap->data()->setCell(y, h-x, i);
                }
            }
            i+=9;
        }
        colorMap->rescaleDataRange();
        colorMap->setInterpolate(false);
        cp->rescaleAxes();
        cp->replot();
        QPixmap cmap = cp->toPixmap();
        scene->addPixmap(cmap);
    }
    else
        scene->addPixmap(QPixmap::fromImage(img));
}

void MainWindow::openForm()
{
    QString tmp = form->openFromBase();
    QStringList sl = tmp.split("/");
    tmp = sl.at(sl.size()-1);
    //do search
    std::vector<QString> result;
    db->selectFromResults(tmp, result);
    form->createForm(result);
    form->show();
}

void MainWindow::Proxodpofunc()
{
    ImageMatrix m1 (proc);
    ImageMatrix m2 (proc_s);
    QImage tmp (proc);
    QImage tmp_2 (proc);
    dst = img;
    int xmin=buf.width(),xmax=0,ymin=buf.height(),ymax=0;
    int Kolca = ui->lineEdit->text().toInt();

    buf = img;
    for (int i = 0; i < buf.width(); ++i)
    {
        for (int j = 0; j < buf.height(); ++j)
        {
            if (tmp.pixelColor(i, j) != QColor(0,0,0))
            {
                if(ymin>j)
                {
                    ymin = j;
                }
                if(ymax<j)
                {
                    ymax = j;
                }
                if(xmin>i)
                {
                    xmin = i;
                }
                if(xmax<i)
                {
                    xmax = i;
                }
            }
        }
    }
    int xcentr = (xmax+xmin)/2;
    int ycentr = (ymax+ymin)/2;

    centrx = xcentr;
    centry = ycentr;

    int Radius =0;
    int Rony = ymax-ycentr;
    int Ronx = xmax-xcentr;

    if(Rony>Ronx)
    {
        Radius = Ronx;
    }
    else
    {
        Radius = Rony;
    }

    int RCirclre = Radius/Kolca;
    RadiusCircle = RCirclre;



qDebug()<<"xmin = "<< xmin << "xmax =" << xmax;
qDebug()<<"ymin = "<< ymin << "ymax =" << ymax;
qDebug()<< "x = "<< xcentr<< "y = "<<ycentr;
}

void MainWindow::on_pushButton_6_clicked()
{
    Proxodpofunc();
}
