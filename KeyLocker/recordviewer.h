#ifndef RECORDVIEWER_H
#define RECORDVIEWER_H

#include "record.h"
#include <QWidget>

namespace Ui {
class recordViewer;
}

class recordViewer : public QWidget
{
    Q_OBJECT

public:
    explicit recordViewer(QWidget *parent = nullptr);
    void getRecord(Record record);
    ~recordViewer();

private slots:
    void on_showLoginBtn_clicked();

    void on_showPassBtn_clicked();

    void on_okBtn_clicked();

private:
    bool loginVisible;
    bool passVisible;

    Ui::recordViewer *ui;
    Record record;
    void displayRecord();
};

#endif // RECORDVIEWER_H
