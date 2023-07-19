#include "imagesetting.h"
#include "ui_imagesetting.h"

#include "QAbstractItemModel"

ImageSetting::ImageSetting(const QString &diagnosis_gyst, const QString &diagnosis_cyt, const QString &enlargment, const QString &color,
                           const QString &group, const QString &symptom, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ImageSetting)
{
    ui->setupUi(this);

    modelDiadnosis_gyst = new QSqlQueryModel(this);
    modelDiadnosis_gyst->setQuery("SELECT Diagnosis FROM "+diagnosis_gyst);
    ui->comboBox->setModel(modelDiadnosis_gyst);

    modelDiadnosis_cyt = new QSqlQueryModel(this);
    modelDiadnosis_cyt->setQuery("SELECT Diagnosis FROM "+diagnosis_cyt);
    ui->comboBox_4->setModel(modelDiadnosis_cyt);

    modelEnlargment = new QSqlQueryModel(this);
    modelEnlargment->setQuery("SELECT Enlargment FROM "+enlargment);
    ui->comboBox_3->setModel(modelEnlargment);
    modelColor = new QSqlQueryModel(this);
    modelColor->setQuery("SELECT Color FROM "+color);
    ui->comboBox_2->setModel(modelColor);

    createTree(group, symptom);
}

ImageSetting::ImageSetting(const QString &diagnosis_gyst, const QString &diagnosis_cyt, const QString &enlargment, const QString &color,
                           const QString &group, const QString &symptom,
                           const QVariantList &dataImage, QList<int> &&symptoms, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ImageSetting)
{
    ui->setupUi(this);

    modelDiadnosis_gyst = new QSqlQueryModel(this);
    modelDiadnosis_gyst->setQuery("SELECT Diagnosis FROM "+diagnosis_gyst);
    ui->comboBox->setModel(modelDiadnosis_gyst);

    modelDiadnosis_cyt = new QSqlQueryModel(this);
    modelDiadnosis_cyt->setQuery("SELECT Diagnosis FROM "+diagnosis_cyt);
    ui->comboBox_4->setModel(modelDiadnosis_cyt);

    modelEnlargment = new QSqlQueryModel(this);
    modelEnlargment->setQuery("SELECT Enlargment FROM "+enlargment);
    ui->comboBox_3->setModel(modelEnlargment);
    modelColor = new QSqlQueryModel(this);
    modelColor->setQuery("SELECT Color FROM "+color);
    ui->comboBox_2->setModel(modelColor);

    ui->pushButton->setHidden(true);
    ui->comboBox->setCurrentIndex(dataImage.value(0).toInt()-1);
    ui->comboBox_4->setCurrentIndex(dataImage.value(1).toInt()-1);
    ui->comboBox_2->setCurrentIndex(dataImage.value(2).toInt()-1);
    ui->comboBox_3->setCurrentIndex(dataImage.value(3).toInt()-1);
    QPixmap pm =QPixmap(dataImage.value(4).toString());
    ui->label->setPixmap(pm.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio));
    ui->lineEdit->setText(dataImage.value(5).toString());

    createTree(group, symptom);
    for(QTreeWidgetItem *item : items){
        for(int i = 0; i < item->childCount(); i++){
            if(symptoms.isEmpty())
                break;
            if(item->child(i)->data(1, 0) == symptoms.front()){
                item->child(i)->setIcon(0, QIcon("check.bmp"));
                symptoms.pop_front();
            }
        }
    }
}

ImageSetting::~ImageSetting()
{
    delete ui;
}

void ImageSetting::on_pushButton_2_clicked()
{
    QVariantList ImgData;
    ImgData.append(fileName);
    ImgData.append(ui->comboBox->currentIndex()+1);
    ImgData.append(ui->comboBox_4->currentIndex()+1);
    ImgData.append(ui->comboBox_2->currentIndex()+1);
    ImgData.append(ui->comboBox_3->currentIndex()+1);
    ImgData.append(ui->lineEdit->text());

    QList<QPair<int, int > > symptoms;

    for(QTreeWidgetItem *item : items){
        for(int i = 0; i < item->childCount(); i++)
            if(item->child(i)->icon(0).cacheKey() != QIcon().cacheKey()){
                symptoms.append(QPair<int, int >(item->data(1, 0).toInt(), item->child(i)->data(1, 0).toInt()));
            }
    }
    emit ImageData(ImgData, symptoms);
}

void ImageSetting::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg *.bmp)"));
    ui->label->setPixmap(QPixmap(fileName).scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio));
}

void ImageSetting::on_pushButton_3_clicked()
{
    emit closeWindow();
}

void ImageSetting::createTree(const QString &group, const QString &symptom)
{
    QSqlQuery query, querySymptom;
    query.exec("SELECT * FROM " + group);
    while(query.next()){
        QStringList data;
        data.append(query.value(1).toString());
        data.append(query.value(0).toString());
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget, data);
        querySymptom.exec("SELECT Symptom, id FROM " + symptom + " WHERE id_" + group + " = " + query.value(0).toString());
        while (querySymptom.next()) {
            QStringList dataSymptom;
            dataSymptom.append(querySymptom.value(0).toString());
            dataSymptom.append(querySymptom.value(1).toString());
            item->addChild(new QTreeWidgetItem(item, dataSymptom));
            item->child(item->childCount()-1)->setIcon(0, QIcon());
        }
        items.append(item);
    }
    ui->treeWidget->insertTopLevelItems(0, items);
}

void ImageSetting::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    QWidget::closeEvent(event);
}

void ImageSetting::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->childCount())
        return;
    if(item->icon(0).cacheKey() == QIcon().cacheKey()){
        item->setIcon(column, QIcon("check.bmp"));
    }
    else{
        item->setIcon(column, QIcon());
    }
}
