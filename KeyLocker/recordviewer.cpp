#include "recordviewer.h"
#include "ui_recordviewer.h"

recordViewer::recordViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::recordViewer)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    this->loginVisible = false;
    this->passVisible = false;
}

void recordViewer::getRecord(Record record)
{
    this->record = record;
    displayRecord();
}

recordViewer::~recordViewer()
{
    delete ui;
}

void recordViewer::displayRecord()
{
    ui->siteView->setText(record.site);
    ui->loginView->setText(record.getLogin());
    ui->passView->setText(record.getPass());
}

void recordViewer::on_showLoginBtn_clicked()
{
    if (loginVisible) {
        ui->loginView->setEchoMode(QLineEdit::Password);
    } else {
        ui->loginView->setEchoMode(QLineEdit::Normal);
    }
    loginVisible = !loginVisible;
}


void recordViewer::on_showPassBtn_clicked()
{
    if (passVisible) {
        ui->passView->setEchoMode(QLineEdit::Password);
    } else {
        ui->passView->setEchoMode(QLineEdit::Normal);
    }
    passVisible = !passVisible;
}


void recordViewer::on_okBtn_clicked()
{
    this->close();
}

