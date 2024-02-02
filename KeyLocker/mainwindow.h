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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addRecord(Record record);

private slots:
    void on_addRecord_clicked();

private:
    Ui::MainWindow *ui;

    RecordEditor recordEditor;
};

#endif // MAINWINDOW_H
