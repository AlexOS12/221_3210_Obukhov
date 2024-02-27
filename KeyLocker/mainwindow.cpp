#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::readRecords()
{
    QFile file;
    file.setFileName(homeDir + "/test.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    } else {
        QByteArray fileContent = QByteArray::fromHex(file.readAll());
        qDebug() << fileContent;
        QByteArray decryptedFile;
        Encryptor::getInstance().decrypt(fileContent, decryptedFile);
        qDebug() << "Decrypted:";
        qDebug() << decryptedFile;
        QJsonDocument jsonDoc;
        jsonDoc = QJsonDocument::fromJson(decryptedFile);
        QJsonArray jsonArray = jsonDoc.array();

        for (int i = 0; i < jsonArray.size(); i++) {
            QJsonValue value = jsonArray.at(i);
            QString site = value["site"].toString();
            QString login = value["login"].toString();
            QString pass = value["pass"].toString();

            Record record(site, login, pass);
            records.append(record);
        }
    }
    displayRecords();
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    homeDir = QDir::homePath() + "/KeyLocker";
    QObject::connect(&recordEditor, SIGNAL(sendRecord(Record)), this, SLOT(addRecord(Record)));
}

MainWindow::~MainWindow()
{
    // запись аккаунтов в json
    QJsonArray array;
    for (Record record : records) {
        array.push_back(record.toJson());
    }

    QJsonDocument jsonDoc(array);

    QFile file;
    file.setFileName(homeDir + "/test.json");


    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.close();
    } else {
        QByteArray encryptedFile;
        QByteArray json = jsonDoc.toJson(QJsonDocument::Compact);
        qDebug() << json;
        Encryptor::getInstance().encrypt(json, encryptedFile);
        qDebug() << "Encrypted";
        qDebug() << encryptedFile;
        QTextStream out(&file);
        out << encryptedFile.toHex();
        file.close();
    }

    delete ui;
}

void MainWindow::addRecord(Record record)
{
    ui->searchLine->setText(record.site);
    records.push_back(record);
}

void MainWindow::on_addRecord_clicked()
{
    recordEditor.show();
}

void MainWindow::displayRecords()
{
    for (Record record : records) {
        ui->listWidget->addItem(record.site);
    }
}


void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    uint recordId = ui->listWidget->currentRow();
    recordViewer.show();
    recordViewer.getRecord(records[recordId]);
}

