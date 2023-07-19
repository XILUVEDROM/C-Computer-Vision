#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "global.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new Base();
    db->connectToDataBase();
    sceneImage = new QGraphicsScene();
    ui->graphicsView->setScene(sceneImage);
    sceneFindImage = new QGraphicsScene();
    ui->graphicsView_2->setScene(sceneFindImage);
    findPatientsWidget = ui->tab_6;
    findImagesWidget = ui->tab_7;
    ui->tabWidget_2->removeTab(2);
    ui->tabWidget_2->removeTab(1);

    ui->chartView->chart()->legend()->hide();
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    ui->tableWidget_4->setColumnCount(3);
    ui->tableWidget_4->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_4->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_4->horizontalHeader()->setStretchLastSection(true);

    this->setupModel();
    this->setupSearch();
    this->createUI();
}

MainWindow::~MainWindow()
{
    delete PS;
    delete IS;
    delete ui;
}

void MainWindow::setupModel()
{
    /* Производим инициализацию модели представления данных
         * с установкой имени таблицы в базе данных, по которому
         * будет производится обращение в таблице
         * */
    model = new QSqlRelationalTableModel(this);
    model->setTable(TABLE_CASESOFDISEASES);

    model->setRelation(9, QSqlRelation(TABLE_DIAGNOSIS_GYST, "id", "Diagnosis"));
    model->setRelation(10, QSqlRelation(TABLE_DIAGNOSIS_CYT, "id", "Diagnosis"));


    model->setHeaderData(1, Qt::Horizontal, "Номер карты");
    model->setHeaderData(2, Qt::Horizontal, "Фамилия");
    model->setHeaderData(3, Qt::Horizontal, "Имя");
    model->setHeaderData(4, Qt::Horizontal, "Отчество");
    model->setHeaderData(5, Qt::Horizontal, "Дата рождения");
    model->setHeaderData(6, Qt::Horizontal, "Возраст");
    model->setHeaderData(7, Qt::Horizontal, "Пол");
    model->setHeaderData(8, Qt::Horizontal, "Дата заболевания");
    model->setHeaderData(9, Qt::Horizontal, "Диагноз гистологический");
    model->setHeaderData(10, Qt::Horizontal, "Диагноз цитологический");
    model->setHeaderData(11, Qt::Horizontal, "Комментарий");
    model->setHeaderData(12, Qt::Horizontal, "Количество изображений");

    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0,Qt::AscendingOrder);

    model->select(); // Делаем выборку данных из таблицы
}

void MainWindow::setupSearch()
{

    // Разрешаем выделение строк
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableWidget_2->resizeColumnsToContents();
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->setColumnHidden(0, true);
    QSqlQuery query, querySymptom;
    ui->comboBox->addItem(QString(""));
    ui->comboBox_2->addItem(QString(""));
    query.exec("SELECT Diagnosis FROM Diagnosis_gyst");
    while (query.next()) {
        ui->comboBox->addItem(query.value(0).toString());
    }
    query.exec("SELECT Diagnosis FROM Diagnosis_cyt");
    while (query.next()) {
        ui->comboBox_2->addItem(query.value(0).toString());
    }
    query.exec("SELECT * FROM " TABLE_GROUPSYMPTOMS);
    while(query.next()){
        QStringList data;
        data.append(query.value(1).toString());
        data.append(query.value(0).toString());
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_2, data);
        querySymptom.exec("SELECT Symptom, id FROM " TABLE_SYMPTOMS " WHERE id_" TABLE_GROUPSYMPTOMS " = "
                          + query.value(0).toString());
        while (querySymptom.next()) {
            QStringList dataSymptom;
            dataSymptom.append(querySymptom.value(0).toString());
            dataSymptom.append(querySymptom.value(1).toString());
            item->addChild(new QTreeWidgetItem(item, dataSymptom));
        }
        itemsSearch.append(item);
    }
    ui->treeWidget_2->insertTopLevelItems(0, itemsSearch);
    this->on_pushButton_8_clicked();
}

void MainWindow::createUI()
{
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей

    ui->tableView->setColumnHidden(2, true);    // Скрываем колонку с id записей
    ui->tableView->setColumnHidden(3, true);    // Скрываем колонку с id записей
    ui->tableView->setColumnHidden(4, true);    // Скрываем колонку с id записей
    ui->tableView->setColumnHidden(5, true);    // Скрываем колонку с id записей


    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

}

PatientImageVisualisation * MainWindow::selectImagesInformation(int id)
{
    QSqlQuery query, queryImage, querySymptom;
    PatientImageVisualisation *PIV = nullptr;
    if(!query.exec("SELECT * FROM " TABLE_IMAGES " WHERE id = " + QString::number(id)))
        qDebug() << query.lastError();
    while (query.next()) {
        PIV = new PatientImageVisualisation();
        PIV->id = query.value(0).toInt();
        PIV->fileName = query.value(6).toString();
        PIV->number = query.value(7).toString();
        PIV->comment = query.value(8).toString();
        PIV->idPatient = query.value(1).toInt();
        PIV->color = query.value(5).toString();


        queryImage.prepare("SELECT Diagnosis FROM " TABLE_DIAGNOSIS_GYST " WHERE id = :id");
        queryImage.bindValue(":id", query.value(2).toInt());
        if(!queryImage.exec()){
            qDebug() << "3";
            qDebug() << query.lastError();
        }
        while (queryImage.next())
            PIV->diagnos_gyst = queryImage.value(0).toString();

        queryImage.prepare("SELECT Diagnosis FROM " TABLE_DIAGNOSIS_CYT " WHERE id = :id");
        queryImage.bindValue(":id", query.value(2).toInt());
        if(!queryImage.exec()){
            qDebug() << "3";
            qDebug() << query.lastError();
        }
        while (queryImage.next())
            PIV->diagnos_cyt = queryImage.value(0).toString();

        queryImage.prepare("SELECT Enlargment FROM " TABLE_ENLARGMENT " WHERE id = :id");
        queryImage.bindValue(":id", query.value(3).toInt());
        if(!queryImage.exec()){
            qDebug() << "4";
            qDebug() << query.lastError();
        }
        while (queryImage.next())
            PIV->enlargment = queryImage.value(0).toString();

        queryImage.prepare("SELECT Color FROM " TABLE_COLOR " WHERE id = :id");
        queryImage.bindValue(":id", query.value(4).toInt());
        if(!queryImage.exec()){
            qDebug() << "5";
            qDebug() << query.lastError();
        }
        while (queryImage.next())
            PIV->color = queryImage.value(0).toString();
        queryImage.exec("SELECT id_" TABLE_GROUPSYMPTOMS ", id_" TABLE_SYMPTOMS ", id, MarkerName"
                                                                                " FROM " TABLE_IMAGESYMPTOMS " WHERE id_" TABLE_IMAGES " = " +QString::number(PIV->id));
        while(queryImage.next()){
            querySymptom.exec("SELECT GroupOfSymptoms FROM " TABLE_GROUPSYMPTOMS " WHERE id = "
                              + queryImage.value(0).toString());
            int number = -1;
            while(querySymptom.next()){
                for(int i = 0; i < PIV->items.size(); i++){
                    if(PIV->items.at(i)->data(0, 0).toString() == querySymptom.value(0).toString()){
                        number = i;
                        break;
                    }
                }
                if(number == -1){
                    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(querySymptom.value(0).toString()));
                    PIV->items.append(item);
                }
            }

            querySymptom.exec("SELECT Symptom FROM " TABLE_SYMPTOMS " WHERE id = "
                              + queryImage.value(1).toString());
            while(querySymptom.next()){
                QStringList DS;
                DS.append(querySymptom.value(0).toString());
                DS.append(queryImage.value(2).toString());
                if(number == -1){
                    QTreeWidgetItem *child = new QTreeWidgetItem(PIV->items.last(), DS);
                    if(!queryImage.value(3).toString().isNull())
                        child->setIcon(0, QIcon("attend.bmp"));
                    else
                        child->setIcon(0, QIcon());
                    PIV->items.last()->addChild(child);
                }
                else{
                    QTreeWidgetItem *child = new QTreeWidgetItem(PIV->items.last(), DS);
                    if(!queryImage.value(3).toString().isNull())
                        child->setIcon(0, QIcon("attend.bmp"));
                    else
                        child->setIcon(0, QIcon());
                    PIV->items.at(number)->addChild(child);
                }
            }
        }
    }
    return PIV;
}

void MainWindow::setChart()
{
    QSqlQuery query;
    QPieSeries *series = new QPieSeries();
    int num=1;
    query.exec("SELECT COUNT(*) FROM " TABLE_CASESOFDISEASES);
    while (query.next())
        num = query.value(0).toInt();
    query.exec("SELECT COUNT(*) FROM " TABLE_CASESOFDISEASES " WHERE Gender LIKE('М')");
    while (query.next()){
        QPieSlice *slice = series->append("М " + QString::number(query.value(0).toDouble()/num*100) + "%",
                                          query.value(0).toInt());
        slice->setLabelVisible();
        qDebug() << QString::number(query.value(0).toDouble()/num*100);
    }
    query.exec("SELECT COUNT(*) FROM " TABLE_CASESOFDISEASES " WHERE Gender LIKE('Ж')");
    while (query.next()){
        QPieSlice *slice = series->append("Ж " + QString::number(query.value(0).toDouble()/num*100) + "%",
                                          query.value(0).toInt());
        slice->setLabelVisible();
        qDebug() << QString::number(query.value(0).toDouble()/num);
    }
    ui->chartView->chart()->removeAllSeries();
    ui->chartView->chart()->addSeries(series);
}

void MainWindow::setDiagnozChart()
{
    //для гистологического
    QSqlQuery query, queryProcent;
    int num=1;
    query.exec("SELECT COUNT(*) FROM " TABLE_CASESOFDISEASES);
    while (query.next())
        num = query.value(0).toInt();
    query.exec("SELECT * FROM " TABLE_DIAGNOSIS_GYST);
    int id = 0;
    while (query.next()){
        ui->tableWidget_4->setRowCount(id+1);
        ui->tableWidget_4->setItem(id, 0, new QTableWidgetItem(query.value(1).toString()));
        queryProcent.exec("SELECT COUNT(*) FROM " TABLE_CASESOFDISEASES " WHERE id_" TABLE_DIAGNOSIS_GYST " = " +
                          query.value(0).toString());
        while (queryProcent.next()){
            ui->tableWidget_4->setItem(id, 1, new QTableWidgetItem(queryProcent.value(0).toString()));
            ui->tableWidget_4->setItem(id++, 2,
                                       new QTableWidgetItem(QString::number(queryProcent.value(0).toDouble()/num*100) + "%"));
        }
    }
    ui->tableWidget_4->resizeColumnsToContents();
}

void MainWindow::on_pushButton_clicked()
{
    //для гистологического
    if(PS != nullptr)
        return;
    PS = new PatientSetting(TABLE_DIAGNOSIS_GYST,TABLE_DIAGNOSIS_CYT);
    connect(PS, SIGNAL(PatientData(const QVariantList &)), this, SLOT(setNewPatientData(const QVariantList &)));
    connect(PS, SIGNAL(closeWindow()), this, SLOT(closePatientSetting()));
    PS->show();
}

void MainWindow::setNewPatientData(const QVariantList &data)
{
    if(!db->addNewPatient(data)){
        PS->show();
        return;
    }
    model->select();
    this->closePatientSetting();
}

void MainWindow::UpdatePatient(const QVariantList &data)
{
    if(!db->updatePatient(id_Patient, data)){
        PS->show();
        return;
    }
    model->select();
    this->closePatientSetting();
}

void MainWindow::closePatientSetting()
{
    delete PS;
    PS = nullptr;
}

void MainWindow::setNewImage(const QVariantList &data, const QList<QPair<int, int > > &dataSymptoms)
{
    if(!db->addNewImage(id_Patient, data, dataSymptoms)){
        IS->show();
        return;
    }
    this->closeImageSetting();
}

void MainWindow::updateImage(const QVariantList &data, const QList<QPair<int, int> > &dataSymptoms)
{
    if(!db->updateImage(id_Image, data, dataSymptoms)){
        IS->show();
        return;
    }
    this->closeImageSetting();
}

void MainWindow::closeImageSetting()
{
    id_Image = -1;
    patientImage.clear();
    ui->tableWidget->clear();
    this->on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
    delete IS;
    IS = nullptr;
}

void MainWindow::closeMarkerWindow()
{
    delete Marker;
    Marker = nullptr;
}

void MainWindow::on_pushButton_2_clicked()
{
    //для гистологического
    if(PS != nullptr)
        return;
    if(id_Patient == -1)
        return;
    PS = new PatientSetting(TABLE_DIAGNOSIS_GYST, TABLE_DIAGNOSIS_CYT, db->GetPatientInformation(id_Patient));
    connect(PS, SIGNAL(PatientData(const QVariantList &)), this, SLOT(UpdatePatient(const QVariantList &)));
    connect(PS, SIGNAL(closeWindow()), this, SLOT(closePatientSetting()));
    PS->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    db->removePatient(id_Patient);
    id_Patient = -1;
    model->select();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    id_Patient = model->data(model->index(index.row(), 0)).toInt();
    qDebug() << id_Patient;
    ui->tableWidget->clear();
    patientImage.clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    if(id_Patient == -1)
        return;
    if(IS != nullptr || PS != nullptr)
        return;
    IS = new ImageSetting(TABLE_DIAGNOSIS_GYST, TABLE_DIAGNOSIS_CYT, TABLE_ENLARGMENT, TABLE_COLOR, TABLE_GROUPSYMPTOMS, TABLE_SYMPTOMS);
    connect(IS, SIGNAL(ImageData(const QVariantList &, const QList<QPair<int, int > > &)),
            this, SLOT(setNewImage(const QVariantList &, const QList<QPair<int, int > > &)));
    connect(IS, SIGNAL(closeWindow()), this, SLOT(closeImageSetting()));
    IS->show();
}

void MainWindow::on_pushButton_10_clicked()
{
    qDebug()<< "Tw";
    PatientImageVisualisation *PIV = nullptr;

        PIV = new PatientImageVisualisation();

    ui->label_3->setText(PIV->number);
    ui->label_7->setText(PIV->color);
    qDebug()<<"Color" << PIV->color<<endl;
    ui->label_9->setText(PIV->enlargment);
    //не добавлено цитологического описания
    ui->label_35->setText(PIV->diagnos_cyt);
    ui->label_11->setText(PIV->diagnos_gyst);
    ui->label_13->setText(PIV->comment);
    markerView = nullptr;
    sceneImage->clear();
    QPixmap pm(PIV->fileName);

    qDebug()<<"String"<< PIV->fileName<<endl;

    sceneImage->setSceneRect(0, 0, pm.width(), pm.height());
    sceneImage->addPixmap(pm);
    while (ui->treeWidget->topLevelItemCount())
        ui->treeWidget->takeTopLevelItem(0);
    ui->treeWidget->insertTopLevelItems(0, PIV->items);
    id_Image = PIV->id;
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 1:
        if(patientImage.empty()){
            ui->treeWidget->clear();
            ui->tableWidget->clear();
            markerView = nullptr;
            sceneImage->clear();
            ui->label_3->clear();
            ui->label_5->clear();
            ui->label_7->clear();
            ui->label_9->clear();
            ui->label_11->clear();
            ui->label_13->clear();
            ui->label_35->clear();
            id_Image = -1;
            QSqlQuery query;
            if(!query.exec("SELECT id, idColor FROM " TABLE_IMAGES " WHERE id_" TABLE_CASESOFDISEASES " = " +
                           QString::number(id_Patient)))

                 break;
            while (query.next())
                patientImage.append(selectImagesInformation(query.value(0).toInt()));
            query.exec("SELECT NumberOfImages FROM " TABLE_CASESOFDISEASES " WHERE id = " + QString::number(id_Patient));
            while (query.next())
                ui->label_5->setText(query.value(0).toString());
            QTableWidgetItem *TWI;
            ui->tableWidget->setRowCount(1);
            ui->tableWidget->setColumnCount(patientImage.size());
            int id = 0;
            for(PatientImageVisualisation *PIV : patientImage){
                TWI = new QTableWidgetItem();
                TWI->setData(Qt::DecorationRole, QPixmap(PIV->fileName).scaled(200, 200));
                ui->tableWidget->setItem(0, id++, TWI);
                qApp->processEvents();
            }
        }
        break;

    case 3:
        ui->label_15->setText(QString::number(db->analitic(TABLE_CASESOFDISEASES)));
        ui->label_17->setText(QString::number(db->analitic(TABLE_IMAGES)));
        ui->label_19->setText(QString::number(db->analitic(TABLE_SYMPTOMS)));
        ui->label_21->setText(QString::number(db->analitic(TABLE_DIAGNOSIS_GYST)));
        ui->label_25->setText(QString::number(db->analitic(TABLE_GROUPSYMPTOMS)));
        this->setChart();
        this->setDiagnozChart();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if(id_Image == -1 || id_Patient == -1)
        return;
    db->removeImage(id_Patient, id_Image);
    patientImage.clear();
    this->on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
    id_Image = -1;
}

void MainWindow::on_pushButton_5_clicked()
{
    if(id_Image == -1 || id_Patient == -1)
        return;
    IS = new ImageSetting(TABLE_DIAGNOSIS_GYST, TABLE_DIAGNOSIS_CYT, TABLE_ENLARGMENT, TABLE_COLOR, TABLE_GROUPSYMPTOMS, TABLE_SYMPTOMS,
                          db->GetImageInformation(id_Image), db->GetImageSymptoms(id_Image));
    connect(IS, SIGNAL(ImageData(const QVariantList &, const QList<QPair<int, int > > &)),
            this, SLOT(updateImage(const QVariantList &, const QList<QPair<int, int > > &)));
    connect(IS, SIGNAL(closeWindow()), this, SLOT(closeImageSetting()));
    IS->show();
}




void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->childCount() || Marker != nullptr)
        return;
    Marker = new MarkerWindow(patientImage.at(ui->tableWidget->currentColumn())->fileName, item->data(1, 0).toInt());
    connect(Marker, SIGNAL(closeWindow()), this, SLOT(closeMarkerWindow()));
    Marker->show();
    Q_UNUSED(column);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->childCount())
        return;
    if(markerView != nullptr){
        sceneImage->removeItem(markerView);
        delete markerView;
        markerView = nullptr;
    }
    if(item->icon(0).cacheKey() != QIcon().cacheKey()){
        markerView = new QGraphicsPixmapItem("MarkersDir/" + item->data(1, 0).toString() + ".png");
        qDebug()<<"Marker"<<markerView;
        sceneImage->addItem(markerView);
    }
    Q_UNUSED(column);
}

void MainWindow::on_pushButton_7_clicked()
{
    qDebug() << "Поиск";
    if(ui->tabWidget_2->count() == 2)
        ui->tabWidget_2->removeTab(1);
    QSqlQuery query;
    if(ui->radioButton_3->isChecked()){
        qDebug() << "по пациенту";
        QString Name("%");
        QString Surname("%");
        QString Patronymic("%");
        QString NumberCard("%");
        QString Sex("%");
        QString idDiagnos_gyst("%");
        QString idDiagnos_cyt("%");
        if(!ui->lineEdit_4->text().isEmpty())
            NumberCard = ui->lineEdit_4->text();
        if(!ui->lineEdit_3->text().isEmpty())
            Surname = ui->lineEdit_3->text();
        if(!ui->lineEdit_2->text().isEmpty())
            Name = ui->lineEdit_2->text();
        if(!ui->lineEdit->text().isEmpty())
            Patronymic = ui->lineEdit->text();
        if(ui->radioButton->isChecked())
            Sex = QString("М");
        if(ui->radioButton_2->isChecked())
            Sex = QString("Ж");
        if(ui->comboBox->currentIndex())
            idDiagnos_gyst = QString::number(ui->comboBox->currentIndex());
        if(ui->comboBox_2->currentIndex())
            idDiagnos_cyt = QString::number(ui->comboBox_2->currentIndex());
       query.exec("SELECT * FROM " TABLE_CASESOFDISEASES " WHERE NumberCard LIKE'" + NumberCard + "' AND Surname LIKE'"
                   + Surname + "' AND Name LIKE'" + Name + "' AND Patronymic LIKE'" + Patronymic + "' AND "
                                                                                                   "Gender LIKE'" + Sex +"' AND id_Diagnosis LIKE'" + idDiagnos_gyst +"' AND id_Diagnosis LIKE'" + idDiagnos_cyt +"' AND "
                                                                                                                                                                                                                  "DateOfBirth BETWEEN '" + ui->dateEdit->date().toString("yyyy.MM.dd") + "' AND '" + ui->dateEdit_2->date().toString("yyyy.MM.dd") + "' AND "
                                                                   "DateOfDisease BETWEEN '" + ui->dateEdit_3->date().toString("yyyy.MM.dd") + "' AND '" +
                   ui->dateEdit_4->date().toString("yyyy.MM.dd") +"'");
       // query.exec("SELECT * FROM " TABLE_CASESOFDISEASES);
        int numRow = 0;
        ui->tableWidget_2->setRowCount(numRow);
        while (query.next()){
            ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
            ui->tableWidget_2->setItem(numRow, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget_2->setItem(numRow, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget_2->setItem(numRow, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget_2->setItem(numRow, 3, new QTableWidgetItem(query.value(3).toString()));
            ui->tableWidget_2->setItem(numRow, 4, new QTableWidgetItem(query.value(4).toString()));
            ui->tableWidget_2->setItem(numRow, 5, new QTableWidgetItem(query.value(5).toString()));
            ui->tableWidget_2->setItem(numRow, 6, new QTableWidgetItem(query.value(6).toString()));
            ui->tableWidget_2->setItem(numRow, 7, new QTableWidgetItem(query.value(7).toString()));
            ui->tableWidget_2->setItem(numRow, 8, new QTableWidgetItem(query.value(8).toString()));
            //ui->tableWidget_2->setItem(numRow, 8, new QTableWidgetItem(query.value(9).toString()));
            QSqlQuery queryDiagnos_gyst;
            queryDiagnos_gyst.exec("SELECT Diagnosis FROM " TABLE_DIAGNOSIS_GYST " WHERE id = " + query.value(9).toString());
            while (queryDiagnos_gyst.next())
                ui->tableWidget_2->setItem(numRow, 9, new QTableWidgetItem(queryDiagnos_gyst.value(0).toString()));
            QSqlQuery queryDiagnos_cyt;
            queryDiagnos_cyt.exec("SELECT Diagnosis FROM " TABLE_DIAGNOSIS_CYT " WHERE id = " + query.value(10).toString());
            while (queryDiagnos_cyt.next())
                ui->tableWidget_2->setItem(numRow, 10, new QTableWidgetItem(queryDiagnos_gyst.value(0).toString()));
                ui->tableWidget_2->setItem(numRow, 11, new QTableWidgetItem(query.value(12).toString()));
                ui->tableWidget_2->setItem(numRow++, 12, new QTableWidgetItem(query.value(11).toString()));
        }
        // Устанавливаем размер колонок по содержимому
        ui->tableWidget_2->resizeColumnsToContents();
        ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
        ui->tabWidget_2->addTab(findPatientsWidget, QString("Результат поиска"));
    } else{
        std::vector<int> ids;
        if(ui->radioButton_5->isChecked()){
            qDebug() << "по признаку 'И'";
            int newRequest = 1;
            for(QTreeWidgetItem *item: itemsSearch)
                for(int i = 0; i < item->childCount(); i++)
                    if(item->child(i)->icon(0).cacheKey() != QIcon().cacheKey()){
                        query.exec("SELECT id_" TABLE_IMAGES " FROM " TABLE_IMAGESYMPTOMS " WHERE "
                                                                                          "id_" TABLE_SYMPTOMS " = " + item->child(i)->data(1, 0).toString());
                        std::vector<int> findIds;
                        while (query.next())
                            findIds.push_back(query.value(0).toInt());
                        if(newRequest){
                            ids.swap(findIds);
                            newRequest--;
                        } else {
                            std::vector<int> repeatingIds;
                            for(int i: findIds){
                                auto result = std::find(ids.begin(), ids.end(), i);
                                if(result != ids.end())
                                    repeatingIds.push_back(i);
                            }
                            ids.swap(repeatingIds);
                        }
                    }
        } else {
            qDebug() << "по признаку 'ИЛИ'";
            QString idsSymptom;
            for(QTreeWidgetItem *item: itemsSearch)
                for(int i = 0; i < item->childCount(); i++)
                    if(item->child(i)->icon(0).cacheKey() != QIcon().cacheKey())
                    {
                        if(!idsSymptom.isEmpty())
                            idsSymptom.append(", ");
                        idsSymptom.append(item->child(i)->data(1, 0).toString());
                    }
            qDebug() << idsSymptom;
            query.exec("SELECT DISTINCT id_" TABLE_IMAGES " FROM " TABLE_IMAGESYMPTOMS " WHERE "
                                                                                       "id_" TABLE_SYMPTOMS " IN(" + idsSymptom + ")");
            while (query.next())
                ids.push_back(query.value(0).toInt());
            qDebug() << "ids633";
            qDebug() << ids;
        }
        //вставленный для построения диаграммы рейтинговой кусочек
        qDebug() << "Rows_amount";
        qDebug() << Rows_amount[7];
        QSqlQuery queryS;
        std::vector<int> itog_case;
        std::vector<int> itog_diag;
        std::vector<int> case_dis;
        std::vector<int> case_diag;
        qDebug() << "case_dis1";
        qDebug() << case_dis;
        int index_case = 0;
        qDebug() << "razmer_vectora";
        int ids_sI =ids.size();
        qDebug() << ids_sI;
        qDebug() << "znachenia vectora";
        qDebug() <<ids;
        for (int m =0; m < (ids_sI); m++)
        {
            index_case= ids[m];
            qDebug() << "m=";
            qDebug() << "case_dis2";
            qDebug() << case_dis;
            qDebug() << m;
            qDebug() << "index_case =";
            qDebug() << index_case;
            queryS.exec("SELECT id_" TABLE_CASESOFDISEASES ", id_" TABLE_DIAGNOSIS_CYT " FROM " TABLE_IMAGES " WHERE id = " + QString::number(index_case));
            while (queryS.next())
            {
                case_dis.push_back(queryS.value(0).toInt());
                case_diag.push_back(queryS.value(1).toInt());
                qDebug() << "case_dis3";
                qDebug() << case_dis;
                qDebug() << "case_diag";
                qDebug() << case_diag;
            }
        }
        qDebug() << "posle zaprosa vseh strok iz ids s povtorom nomera sluchay";
        qDebug() << "case_dis";
        qDebug() << case_dis;
        qDebug() << "case_diag";
            qDebug() << case_diag;
            int stroc_povtor_case_dis = case_dis.size();
            qDebug() << "stroc_povtor_case_dis=";
            qDebug() << stroc_povtor_case_dis;

        qDebug() << "podschet diagnozov dly diagrammi";
        for (int p = 0; p < stroc_povtor_case_dis; p++)
        {
            case_dis[stroc_povtor_case_dis]=0;
            qDebug() << "p=";
            qDebug() << p;
            qDebug() << "case_dis[p]=";
            qDebug() << case_dis[p];
            qDebug() << "case_diag[p]=";
            qDebug() << case_diag[p];
            qDebug() << "case_dis[p+1]=";
            qDebug() << case_dis[p+1];
               if (case_dis[p] != case_dis[p+1])
               {
                   itog_case.push_back(case_dis[p]);
                   itog_diag.push_back(case_diag[p]);
               }
        }
        qDebug() << "itog_case=";
        qDebug() << itog_case;
        qDebug() << "itog_diag=";
        qDebug() << itog_diag;
        int itog_case_qual =itog_case.size();
        qDebug() << "itog_case_qual=";
        qDebug() << itog_case_qual;
        int Rows[25]= {0};
        for (int h =0; h < itog_case_qual; h++)
        {
            qDebug() << "709stroka mainwindow";
            qDebug() << h;
            qDebug() << "itog_diag[h]";
            qDebug() << itog_diag[h];// eto sum dly diagrammi
            //if (itog_diag[h]==h+1)
            /*{ Rows[h]=Rows[h]+1;
                         qDebug() << "ROWS[h]";
                         qDebug() << Rows[h];// schetchik nosology
                   Rows_amount[h]=Rows[h];
                   qDebug() << Rows_amount[h];
                   }*/
            if (itog_diag[h]==1)
            {
                Rows[0]=Rows[0]+1;

                // Rows_amount[0]=8;
            }
            if (itog_diag[h]==2)
            {
                Rows[1] ++;
 qDebug() <<Rows[1];
            }
            if (itog_diag[h]==3)
            {
                Rows[2] ++;
                qDebug() <<Rows[2];
            }
            if (itog_diag[h]==4)
            {
                Rows[3] ++;
                qDebug() <<Rows[3];
            }
            if (itog_diag[h]==5)
            {
                Rows[4]++;
                qDebug() <<Rows[4];
            }
            if (itog_diag[h]==6)
            {
                Rows[5]= Rows[5]+1;
                qDebug() <<Rows[5];
            }
            if (itog_diag[h]==7)
            {
                Rows[6]= Rows[6]+1;
                qDebug() <<Rows[6];
            }
            if (itog_diag[h]==8)
            {
                Rows[7]= Rows[7]+1;
                qDebug() <<Rows[7];
            }
            if (itog_diag[h]==9)
            {
                Rows[8]= Rows[8]+1;
                qDebug() <<Rows[8];

            }
            if (itog_diag[h]==10)
            {
                Rows[9]= Rows[9]+1;
            }
            if (itog_diag[h]==11)
            {
                Rows[10]= Rows[10]+1;
            }
            if (itog_diag[h]==12)
            {
                Rows[11]= Rows[11]+1;
            }
            if (itog_diag[h]==13)
            {
                Rows[12]= Rows[12]+1;
            }
            if (itog_diag[h]==14)
            {
                Rows[13]= Rows[13]+1;
            }
            if (itog_diag[h]==15)
            {
                Rows[14]= Rows[14]+1;
            }
            if (itog_diag[h]==16)
            {
                Rows[15]= Rows[15]+1;
            }
            if (itog_diag[h]==17)
            {
                Rows[16]= Rows[16]+1;
            }
            if (itog_diag[h]==18)
            {
                Rows[17]= Rows[17]+1;
            }
            if (itog_diag[h]==19)
            {
                Rows[18]= Rows[18]+1;
            }
            if (itog_diag[h]==20)
            {
                Rows[19]= Rows[19]+1;
            }
            if (itog_diag[h]==21)
            {
                Rows[20]= Rows[20]+1;
            }
            if (itog_diag[h]==22)
            {
                Rows[21]= Rows[21]+1;
            }
            if (itog_diag[h]==23)
            {
                Rows[22]= Rows[22]+1;
            }
            if (itog_diag[h]==24)
            {
                Rows[23]= Rows[23]+1;
            }
            if (itog_diag[h]==25)
            {
                Rows[24]= Rows[24]+1;
            }
            qDebug() <<"Rows[g] for h";
            for (int g=0; g< 25; g++)
               {   qDebug() <<Rows[g];}
        }
        for (int g=0; g< 25; g++)
           { Rows_amount[g]=Rows[g];}


                  /* for (int g=0; g< 25; g++)
                   {
                       qDebug()<<"g =";
                       qDebug()<< g;
                       qDebug()<<"Rows";
                       qDebug()<<Rows[g];
                      // qDebug()<<"Rows_amount";
                       qDebug() << Rows_amount[g];

                   }*/
             //  }
                   DI =new Diagramma;
                   DI->show();

       // ///////////////////////////////////////////////////////////////////////////////////////
       findImage.clear();
        ui->tableWidget_3->clear();
        ui->label_51->clear();
        ui->label_45->clear();
        ui->label_50->clear();
        ui->label_56->clear();
        markerFindView = nullptr;
        sceneFindImage->clear();
        while (ui->treeWidget_3->topLevelItemCount())
            ui->treeWidget_3->takeTopLevelItem(0);
        for (int i: ids)
            findImage.append(selectImagesInformation(i));
        QTableWidgetItem *TWI;
        ui->tableWidget_3->setRowCount(1);
        ui->tableWidget_3->setColumnCount(findImage.size());
        int id = 0;
        for(PatientImageVisualisation *PIV : findImage){
            TWI = new QTableWidgetItem();
            TWI->setData(Qt::DecorationRole, QPixmap(PIV->fileName).scaled(200, 200));
            ui->tableWidget_3->setItem(0, id++, TWI);
            qApp->processEvents();
        }
        ui->tabWidget_2->addTab(findImagesWidget, QString("Результат поиска"));
    }
    ui->tabWidget_2->setCurrentIndex(1);
}


void MainWindow::on_pushButton_8_clicked()
{
    ui->comboBox->setCurrentIndex(0);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->dateEdit->setDate(QDate(1900, 1, 1));
    ui->dateEdit_2->setDate(QDate().currentDate());
    ui->dateEdit_3->setDate(QDate(1900, 1, 1));
    ui->dateEdit_4->setDate(QDate().currentDate());
    for(QTreeWidgetItem *item: itemsSearch)
        for(int i = 0; i < item->childCount(); i++)
            item->child(i)->setIcon(0, QIcon());
    this->on_radioButton_3_clicked();
}

void MainWindow::on_radioButton_3_clicked()
{
    ui->radioButton_4->setChecked(false);
    ui->groupBox_3->setMaximumWidth(1);
    ui->groupBox_3->setMinimumWidth(0);
    ui->groupBox_3->hide();
    ui->groupBox_2->setMinimumWidth(600);
    ui->groupBox_2->setMaximumWidth(16777215);
    ui->groupBox_2->show();
}

void MainWindow::on_radioButton_4_clicked()
{
    ui->radioButton_3->setChecked(false);
    ui->groupBox_2->setMaximumWidth(1);
    ui->groupBox_2->setMinimumWidth(0);
    ui->groupBox_2->hide();
    ui->groupBox_3->setMinimumWidth(600);
    ui->groupBox_3->setMaximumWidth(16777215);
    ui->groupBox_3->show();
}

void MainWindow::on_treeWidget_2_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(!item->childCount()){
        if(item->icon(column).cacheKey() == QIcon().cacheKey())
            item->setIcon(column, QIcon("check.bmp"));
        else
            item->setIcon(column, QIcon());
    }
}

void MainWindow::on_tableWidget_2_clicked(const QModelIndex &index)
{
    patientImage.clear();
    id_Patient = ui->tableWidget_2->item(index.row(), 0)->data(0).toInt();
}

void MainWindow::on_tableWidget_3_clicked(const QModelIndex &index)
{
    qDebug()<< "QModelIndex"<< index;
    PatientImageVisualisation *PIV = findImage.at(index.column());
    ui->label_51->setText(PIV->color);
    ui->label_45->setText(PIV->enlargment);
    //для гистологического
    ui->label_50->setText(PIV->diagnos_gyst);
    ui->label_56->setText(PIV->comment);
    markerFindView = nullptr;
    sceneFindImage->clear();
    QPixmap pm(PIV->fileName);
    sceneFindImage->setSceneRect(0, 0, pm.width(), pm.height());
    sceneFindImage->addPixmap(pm);
    while (ui->treeWidget_3->topLevelItemCount())
        ui->treeWidget_3->takeTopLevelItem(0);
    ui->treeWidget_3->insertTopLevelItems(0, PIV->items);
}

void MainWindow::on_pushButton_9_clicked()
{

    if(ui->tableWidget_3->currentIndex().column() == -1)
        return;
    id_Patient = findImage.at(ui->tableWidget_3->currentIndex().column())->idPatient;
    patientImage.clear();
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_treeWidget_3_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->childCount())
        return;
    if(markerFindView != nullptr){
        sceneFindImage->removeItem(markerFindView);
        delete markerFindView;
        markerFindView = nullptr;
    }
    if(item->icon(0).cacheKey() != QIcon().cacheKey()){
        markerFindView = new QGraphicsPixmapItem("MarkersDir/" + item->data(1, 0).toString() + ".png");
        sceneFindImage->addItem(markerFindView);
    }
    Q_UNUSED(column);
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    qDebug()<< "Tw";
    PatientImageVisualisation *PIV = patientImage.at(index.column());

    ui->label_3->setText(PIV->number);
    ui->label_7->setText(PIV->color);
    qDebug()<<"Color" << PIV->color<<endl;
    ui->label_9->setText(PIV->enlargment);
    //не добавлено цитологического описания
    ui->label_35->setText(PIV->diagnos_cyt);
    ui->label_11->setText(PIV->diagnos_gyst);
    ui->label_13->setText(PIV->comment);
    markerView = nullptr;
    sceneImage->clear();
    QPixmap pm(PIV->fileName);

    qDebug()<<"String"<< PIV->fileName<<endl;

    sceneImage->setSceneRect(0, 0, pm.width(), pm.height());
    sceneImage->addPixmap(pm);
    while (ui->treeWidget->topLevelItemCount())
        ui->treeWidget->takeTopLevelItem(0);
    ui->treeWidget->insertTopLevelItems(0, PIV->items);
    id_Image = PIV->id;
}

void MainWindow::test()
{
    qDebug()<< "Tw";
}

int MainWindow::serega()
{
int a;
a=3;

qDebug()<<"Yu";
PatientImageVisualisation *PIV ;

ui->label_3->setText(PIV->number);
ui->label_7->setText(PIV->color);
qDebug()<<"Color" << PIV->color<<endl;
ui->label_9->setText(PIV->enlargment);
//не добавлено цитологического описания
ui->label_35->setText(PIV->diagnos_cyt);
ui->label_11->setText(PIV->diagnos_gyst);
ui->label_13->setText(PIV->comment);
markerView = nullptr;
sceneImage->clear();
QPixmap pm(PIV->fileName);

qDebug()<<"String"<< PIV->fileName<<endl;

sceneImage->setSceneRect(0, 0, pm.width(), pm.height());
sceneImage->addPixmap(pm);
while (ui->treeWidget->topLevelItemCount())
    ui->treeWidget->takeTopLevelItem(0);
ui->treeWidget->insertTopLevelItems(0, PIV->items);
id_Image = PIV->id;

return a;
}


