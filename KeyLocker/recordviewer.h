#ifndef RECORDVIEWER_H
#define RECORDVIEWER_H

#include "record.h"
#include <QWidget>

namespace Ui {
class RecordViewer;
}

class RecordViewer : public QWidget
{
    Q_OBJECT

public:
    explicit RecordViewer(QWidget *parent = nullptr);
    void getRecord(Record record);
    ~RecordViewer();

private slots:
    void on_showLoginBtn_clicked();

    void on_showPassBtn_clicked();

    void on_okBtn_clicked();

private:
    bool loginVisible;
    bool passVisible;

    Ui::RecordViewer *ui;
    Record record;
    void displayRecord();
};

#endif // RECORDVIEWER_H
