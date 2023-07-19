#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
     IplImage* image=0;
    QString base_folder_2 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString Linefound = QFileDialog::getOpenFileName(this,
                                                tr("Выбор Изображения"),
                                                base_folder_2,
                                                tr("Таблицы (*.jpg)"),nullptr, QFileDialog::DontResolveSymlinks);

            std::string temp(Linefound.toStdString());
           // имя картинки задаётся первым параметром

          const char* filename = temp.c_str();
          const char* image_filename = temp.c_str();
         image = cvLoadImage(image_filename,1);
           // получаем картинку (в градациях серого)
    // имя картинки задаётся первым параметром
    // получаем картинку

    image = cvLoadImage(filename,1);

    assert( image != 0 );
    // создаём одноканальные картинки
    gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    bin = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    // клонируем
    dst = cvCloneImage(image);
    // окно для отображения картинки
    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("binary",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("contours",CV_WINDOW_AUTOSIZE);
    // преобразуем в градации серого
    cvCvtColor(image, gray, CV_RGB2GRAY);
    // преобразуем в двоичное
    cvInRangeS(gray, cvScalar(40), cvScalar(150), bin); // atoi(argv[2])
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours=0;
    // находим контуры
    int contoursCont = cvFindContours( bin, storage,&contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
    // нарисуем контуры
    for(CvSeq* seq0 = contours;seq0!=0;seq0 = seq0->h_next){
        cvDrawContours(dst, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); // рисуем контур
    }
    // показываем картинки
    cvShowImage("original",image);
    cvShowImage("binary", bin);
    cvShowImage("contours", dst);
    // ждём нажатия клавиши
    cvWaitKey(0);
    // освобождаем ресурсы
    cvReleaseImage(&image);
    cvReleaseImage(&gray);
    cvReleaseImage(&bin);
    cvReleaseImage(&dst);
    // удаляем окна
    cvDestroyAllWindows();
}

