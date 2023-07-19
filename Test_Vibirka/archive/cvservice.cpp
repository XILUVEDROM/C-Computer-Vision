#include "cvservice.h"
#include <QtCore/QTextStream>//<qtextstream.h>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QFile>
#include <QtCore/QDir>



QVector<cv::Mat> CvServices::readMat(QString filename, float testSize)
{
    QFile inputFile(filename);
    inputFile.open(QFile::ReadOnly);

    QTextStream input(&inputFile);

    QVector<QString> lines;
    //QVector<QString> names;

    QString line;
    while (input.readLineInto(&line))//Читает одну строку текста из потока в строку/Если строка равна 0, строка чтения не сохраняется
        lines.append(line);//заносим строку в вектор / C:\Users\vaski_000\Desktop\Клетки\выборка\B - тип\Эозинофил\narezannoe(19)\10225-Эозинофил-{0a9323a1-a14b-4ac1-911c-5bba5be32281}(size_43x43)(Смещение0;0).bmp	0,00298749515786767	12,5299634933472	6,17497158050537	0,317947864532471	0,0125000001862645	0,150641053915024	0,485220342874527	0,00688730133697391	6,6083345413208	5,43042850494385	0,391849637031555	0,0205128211528063	0,171153903007507	0,46227440237999	0,020264646038413	1,78814077377319	4,26339387893677	0,572424292564392	0,0552884601056576	0,304326891899109	0,458874195814133

    inputFile.close();

    int rowsCount = lines.size() - 1;//кол - во строк
    int colsCount = lines[0].split('\t').size() - 2;//кол - во столбцов

    int trainRowsCount = rowsCount * (1 - testSize);//кол - во строк обучающей выборки
    int testRowsCount = rowsCount - trainRowsCount;//кол - во строк тестовой выборки

    cv::Mat resultTrain(trainRowsCount, colsCount, CV_32F);
    cv::Mat resultTest(testRowsCount, colsCount, CV_32F);

    //for (int i = 1; i < lines.size(); ++i)
    //{
    //	auto tokens = lines[i].split('\t');//результат разделения строки по табуляциям
    //	for (int j = 1; j < tokens.size() - 1; ++j)
    //	{
    //		tokens[j].replace(',', '.');
    //		result.at<double>(i - 1, j - 1) = tokens[j].toDouble();//доступ к элементу матрицы с индексами i - 1 , j - 1 и присваивание ему значения из файла
    //	}
    //}

    for (int i = 1; i <= trainRowsCount; ++i)
    {
        auto tokens = lines[i].split('\t');//результат разделения строки по табуляциям
        for (int j = 1; j < tokens.size() - 1; ++j)
        {
            tokens[j].replace(',', '.');
            auto val = tokens[j].toFloat();
            resultTrain.at<float>(i - 1, j - 1) = val;//доступ к элементу матрицы с индексами i - 1 , j - 1 и присваивание ему значения из файла
        }
    }
    for (int i = trainRowsCount + 1; i <= rowsCount; ++i)//тестовая
    {
        auto tokens = lines[i].split('\t');//результат разделения строки по табуляциям
        for (int j = 1; j < tokens.size() - 1; ++j)
        {
            tokens[j].replace(',', '.');
            auto val = tokens[j].toFloat();
            resultTest.at<float>(i - 1 - trainRowsCount, j - 1) = tokens[j].toFloat();//доступ к элементу матрицы с индексами i - 1 , j - 1 и присваивание ему значения из файла
        }
    }
    //----------------------
    //for (int i = trainRowsCount + 1; i <= rowsCount; ++i)
    //{
    //	auto tokens = lines[i].split('\t');//результат разделения строки по табуляциям
    //	for (int j = 0; j < tokens.size() - 1; ++j)
    //	{
    //		//names.append(tokens[0]);
    //
    //	}
    //}
    //------------------------

    QVector<cv::Mat> result;
    result.append(resultTrain);
    result.append(resultTest);
    //stdvector = names.toStdVector();


    return result;
}


QVector<QString> CvServices::readName(QString filename, float testSize)
{
    QFile inputFile(filename);
    inputFile.open(QFile::ReadOnly);

    QTextStream input(&inputFile);

    QVector<QString> lines;
    //QVector<QString> names;

    QString line;
    while (input.readLineInto(&line))//Читает одну строку текста из потока в строку/Если строка равна 0, строка чтения не сохраняется
        lines.append(line);//заносим строку в вектор / C:\Users\vaski_000\Desktop\Клетки\выборка\B - тип\Эозинофил\narezannoe(19)\10225-Эозинофил-{0a9323a1-a14b-4ac1-911c-5bba5be32281}(size_43x43)(Смещение0;0).bmp	0,00298749515786767	12,5299634933472	6,17497158050537	0,317947864532471	0,0125000001862645	0,150641053915024	0,485220342874527	0,00688730133697391	6,6083345413208	5,43042850494385	0,391849637031555	0,0205128211528063	0,171153903007507	0,46227440237999	0,020264646038413	1,78814077377319	4,26339387893677	0,572424292564392	0,0552884601056576	0,304326891899109	0,458874195814133

    inputFile.close();


    int rowsCount = lines.size() - 1 ;//кол - во строк// -1
    int colsCount = lines[0].split('\t').size() - 2;//кол - во столбцов

    int trainRowsCount = rowsCount * (1 - testSize);//кол - во строк обучающей выборки
    int testRowsCount = rowsCount - trainRowsCount;//кол - во строк тестовой выборки

    QVector<QString> names;

    for (int i = trainRowsCount + 1; i <= rowsCount  ; ++i)//int i = trainRowsCount + 1
        {
            auto tokens = lines[i].split('\t');//результат разделения строки по табуляциям

            names.append(tokens[0]);

            //for (int j = 0; j < 1  /*tokens.size() - 1*/; ++j)
            //{
            //	names.append(tokens[0]);
            //
            //}
        }

    return names;

}



QVector<cv::Mat> CvServices::createLabelsMat(int label, int size, float testSize)
{
    int trainSize = size * (1 - testSize);
    cv::Mat resultTrain(trainSize, 1, CV_32S);//CV_32S
    for (int i = 0; i < trainSize; ++i)
        resultTrain.at<int>(i, 0) = label;

    testSize = size - trainSize;
    cv::Mat resultTest(testSize, 1, CV_32S);//CV_32S
    for (int i = 0; i < testSize; ++i)
        resultTest.at<int>(i, 0) = label;

    QVector<cv::Mat> result;
    result.append(resultTrain);
    result.append(resultTest);

    return result;
}

QVector<cv::Mat> CvServices::readMatWithLabels(QString filename, int label, float testSize)
{
    QVector<cv::Mat> res;
    QVector<cv::Mat> data = readMat(filename, testSize);
    QVector<cv::Mat> labels = createLabelsMat(label, data[0].size().height + data[1].size().height, testSize);

    res.append(data);
    res.append(labels);

    return res;
}

QVector<QString> CvServices::readAllFilesName(QString path, float testSize)
{
    //dataNames.clear;
    QDir dir(path);// обеспечивает доступ к структурам каталогов и их содержимому.Создает QDir с путем path

    QFileInfoList files = dir.entryInfoList(QDir::Files);//Возвращает список объектов QFileInfo для всех файлов и каталогов в каталоге, упорядоченный в соответствии с фильтрами имен и атрибутов, ранее установленных с помощью setNameFilters () и setFilter (), и отсортированными в соответствии с флагами, установленными с помощью setSorting ().


    QVector<QString> dataNames;
    /*= readName(files[i].absoluteFilePath(), testSize);*/
    for (int i = 0; i < files.size(); ++i)
    {


        //QVector<QString> dataNames; /*= readName(files[i].absoluteFilePath(), testSize);*/
        QVector<QString> vector1 = readName(files[i].absoluteFilePath(), testSize);
        for (int j = 0; j < vector1.size() ; ++j)
        {
            QString name12 = vector1.value(j);//at
            //dataNames.append(name12);
            dataNames.append(name12);
        }
    }

    return dataNames;
}

QVector<cv::Mat> CvServices::readAllFiles(QString path, float testSize)//вызвать метод , передав путь к папке с файлами и передать значения матриц в обуч и тест данные
{
    labelNames.clear();

    QVector<cv::Mat> result;

    QDir dir(path);// обеспечивает доступ к структурам каталогов и их содержимому.Создает QDir с путем path

    QFileInfoList files = dir.entryInfoList(QDir::Files);//Возвращает список объектов QFileInfo для всех файлов и каталогов в каталоге, упорядоченный в соответствии с фильтрами имен и атрибутов, ранее установленных с помощью setNameFilters () и setFilter (), и отсортированными в соответствии с флагами, установленными с помощью setSorting ().

    cv::Mat dataTrain;
    cv::Mat dataTest;
    cv::Mat labelsTrain;
    cv::Mat labelsTest;

    for (int i = 0; i < files.size(); ++i)
    {
        QVector<cv::Mat> dataLabels = readMatWithLabels(files[i].absoluteFilePath(), i, testSize);
        labelNames.append(files[i].baseName());

        if (i == 0)
        {
            int colNumber = dataLabels[0].size().width;
            dataTrain = cv::Mat(0, colNumber, CV_32F);
            dataTest = cv::Mat(0, colNumber, CV_32F);
            labelsTrain = cv::Mat(0, 1, CV_32S);//CV_32S
            labelsTest = cv::Mat(0, 1, CV_32S);//CV_32S
        }

        dataTrain.push_back(dataLabels[0]);
        dataTest.push_back(dataLabels[1]);
        labelsTrain.push_back(dataLabels[2]);
        labelsTest.push_back(dataLabels[3]);
    }

    QVector<cv::Mat> res;
    res.append(dataTrain);
    res.append(dataTest);
    res.append(labelsTrain);
    res.append(labelsTest);

    return res;
}

QString CvServices::getLabelName(int label)
{
    if (label < labelNames.size() && label >= 0)
        return labelNames[label];

    return "";
}

QVector<QString> CvServices::labelNames;
