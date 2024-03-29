#include "mainwindow.h"
#include "ui_mainwindow.h"


bool MainWindow::readRecords()
{
    QFile file;
    file.setFileName(homeDir + "/test.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    } else {
        QJsonDocument jsonDoc;
        jsonDoc = QJsonDocument::fromJson(file.readAll());
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
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    homeDir = QDir::homePath() + "/KeyLocker";
    readRecords();
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
        file.Truncate;
        QTextStream out(&file);
        out << jsonDoc.toJson(QJsonDocument::Compact);
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
    recordEditor.setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    recordEditor.show();
}

