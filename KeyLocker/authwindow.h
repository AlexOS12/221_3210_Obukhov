#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#define PIN "1234"

#include <QMainWindow>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class AuthWindow;
}
QT_END_NAMESPACE

class AuthWindow : public QMainWindow
{
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

private slots:
    void on_PinBtn1_clicked();

    void on_PinBtn2_clicked();

    void on_PinBtn3_clicked();

    void on_PinBtn4_clicked();

    void on_PinBtn5_clicked();

    void on_PinBtn6_clicked();

    void on_PinBtn7_clicked();

    void on_PinBtn8_clicked();

    void on_PinBtn9_clicked();

    void on_PinBtn0_clicked();

    void on_ClrBtn_clicked();

    void on_EnterBtn_clicked();

signals:
    void sendAuthResult(bool authResult);

private:
    Ui::AuthWindow *ui;
};
#endif // AUTHWINDOW_H
