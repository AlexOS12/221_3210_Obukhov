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
#include <QClipboard>
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
    void reEncryptRecords(QString oldPin, QString newPin);

    void changePin();

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

    void on_okBtn_clicked();

    void on_changePinBtn_clicked();

    void on_newPinEdit_returnPressed();

    void on_addNewRecBtn_clicked();

    void on_copyLoginBtn_clicked();

    void on_copyPassBtn_clicked();

    void on_changeBtn_clicked();

private:
    Ui::MainWindow *ui;
    QByteArray currPin;
    bool recordsRead = false;
    void displayRecords();
    void showRecord(uint recordId);
    bool changePinMenuOpened = false;
    bool addRecordMenuOpened = false;
    RecordEditor recordEditor;
    uint selectedRecord = -1;
};

#endif // MAINWINDOW_H
