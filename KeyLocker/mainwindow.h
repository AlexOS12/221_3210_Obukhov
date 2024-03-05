#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define PIN "1234"

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDialog>
#include <QCryptographicHash>
#include "encryptor.h"
#include "record.h"
#include "recordeditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QString homeDir;
    QList<Record> records;

public:
    bool readRecords();
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addRecord(Record record);

private slots:
    void on_addRecord_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pinEdit_returnPressed();

    void on_showLoginBtn_clicked();

    void on_showPassBtn_clicked();

    void on_okBtn_clicked();

    void on_changePinBtn_clicked();

    void on_newPinEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QByteArray currPin;
    bool recordsRead = false;
    void displayRecords();
    void showRecord(uint recordId);
    bool changePinMenuOpened = false;
    RecordEditor recordEditor;
};

#endif // MAINWINDOW_H
