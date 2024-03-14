#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidgetItem>
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
#include "recordwiget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QString homeDir;
    QList<Record> records;
    QList<recordWiget*> recordWidgets;
    void reEncryptRecords(QString oldPin, QString newPin);

    void changePin();

public:
    bool readRecords();
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void copyRecordPass(int recordId);
    void copyRecordLogin(int recordId);
    void deleteRecord(int recordId);

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

    void on_searchLine_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QByteArray currPin;
    bool recordsRead = false;
    void displayRecords();
    void showRecord(uint recordId);
    bool changePinMenuOpened = false;
    bool addRecordMenuOpened = false;
    uint selectedRecord = -1;
};

#endif // MAINWINDOW_H
