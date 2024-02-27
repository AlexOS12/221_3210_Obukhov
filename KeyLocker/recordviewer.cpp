#include "recordviewer.h"
#include "ui_recordviewer.h"

RecordViewer::RecordViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecordViewer)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    this->loginVisible = false;
    this->passVisible = false;
}

void RecordViewer::getRecord(Record record)
{
    this->record = record;
    displayRecord();
}

RecordViewer::~RecordViewer()
{
    delete ui;
}

void RecordViewer::displayRecord()
{
    ui->siteView->setText(record.site);
    ui->loginView->setText(record.getLogin());
    ui->passView->setText(record.getPass());
}

void RecordViewer::on_showLoginBtn_clicked()
{
    if (loginVisible) {
        ui->loginView->setEchoMode(QLineEdit::Password);
    } else {
        ui->loginView->setEchoMode(QLineEdit::Normal);
    }
    loginVisible = !loginVisible;
}


void RecordViewer::on_showPassBtn_clicked()
{
    if (passVisible) {
        ui->passView->setEchoMode(QLineEdit::Password);
    } else {
        ui->passView->setEchoMode(QLineEdit::Normal);
    }
    passVisible = !passVisible;
}


void RecordViewer::on_okBtn_clicked()
{
    this->close();
}

