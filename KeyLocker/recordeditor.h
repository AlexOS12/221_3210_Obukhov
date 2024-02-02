#ifndef RECORDEDITOR_H
#define RECORDEDITOR_H

#include <QWidget>
#include "record.h"

namespace Ui {
class RecordEditor;
}

class RecordEditor : public QWidget
{
    Q_OBJECT

public:
    explicit RecordEditor(QWidget *parent = nullptr);
    ~RecordEditor();

private slots:
    void on_cancelBtn_clicked();

    void on_addBtn_clicked();

private:
    Ui::RecordEditor *ui;

    void clear();
};

#endif // RECORDEDITOR_H
