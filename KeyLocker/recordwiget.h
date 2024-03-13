#ifndef RECORDWIGET_H
#define RECORDWIGET_H

#include <QWidget>

namespace Ui {
class recordWiget;
}

class recordWiget : public QWidget
{
    Q_OBJECT

    int recordId = -1;

public:
    QString site;
    explicit recordWiget(QWidget *parent = nullptr, int recordId = -1, QString site = "");
    ~recordWiget();

signals:
    void copyRecordLogin(int recordId);
    void copyRecordPass(int recordId);

private slots:
    void on_copyLogin_clicked();

    void on_copyPass_clicked();

private:
    Ui::recordWiget *ui;
};

#endif // RECORDWIGET_H
