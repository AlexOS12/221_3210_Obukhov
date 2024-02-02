#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    homeDir = QDir::homePath() + "/KeyLocker";

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

void MainWindow::on_addRecord_clicked()
{
    recordEditor.setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    recordEditor.show();
}

