#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDialog>
#include "record.h"
#include "recordeditor.h"
#include "recordviewer.h"

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

private:
    Ui::MainWindow *ui;

    void displayRecords();
    RecordEditor recordEditor;
    recordViewer recordViewer;
};

#endif // MAINWINDOW_H
