#include "authwindow.h"
#include "ui_authwindow.h"

AuthWindow::AuthWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

// Действия для кнопок
void AuthWindow::on_PinBtn1_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "1");
}


void AuthWindow::on_PinBtn2_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "2");
}


void AuthWindow::on_PinBtn3_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "3");
}


void AuthWindow::on_PinBtn4_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "4");
}


void AuthWindow::on_PinBtn5_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "5");
}


void AuthWindow::on_PinBtn6_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "6");
}




void AuthWindow::on_PinBtn7_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "7");
}


void AuthWindow::on_PinBtn8_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "8");
}




void AuthWindow::on_PinBtn9_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "9");
}


void AuthWindow::on_PinBtn0_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text() + "0");
}


void AuthWindow::on_ClrBtn_clicked()
{
    ui->PinEdit->setText(ui->PinEdit->text().remove(ui->PinEdit->text().length() - 1, 1));
}


void AuthWindow::on_EnterBtn_clicked()
{
    QString pin = ui->PinEdit->text();

    if (PIN == pin) {
        ui->EnterBtn->setText("OK");
        mainwindow.show();
        this->close();
    } else {
        ui->EnterBtn->setText("NOT OK");
    }
}

