#include "recordeditor.h"
#include "ui_recordeditor.h"

RecordEditor::RecordEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecordEditor)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::SplashScreen, true);
}

RecordEditor::~RecordEditor()
{
    ui->siteEdit->setText("");
    ui->loginEdit->setText("");
    ui->passEdit->setText("");

    delete ui;
}

void RecordEditor::on_cancelBtn_clicked()
{
    clear();
    this->close();
}

void RecordEditor::on_addBtn_clicked()
{
    QString site = ui->siteEdit->text();
    QString login = ui->loginEdit->text();
    QString pass = ui->passEdit->text();

    Record record(site, login, pass);

    clear();

    emit sendRecord(record);

    this->close();
}

void RecordEditor::clear()
{
    ui->siteEdit->setText("");
    ui->loginEdit->setText("");
    ui->passEdit->setText("");
}

