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

    QJsonObject credentials;
    credentials.insert("login", login);
    credentials.insert("pass", pass);

    QJsonDocument jdoc(credentials);

    Record record(site, QString(jdoc.toJson(QJsonDocument::Compact)));

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

