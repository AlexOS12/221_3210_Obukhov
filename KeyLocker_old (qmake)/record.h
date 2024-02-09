#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QJsonObject>

class Record
{
private:
    QString login;
    QString pass;
public:
    Record(QString site, QString login, QString pass);
    QString site;
    QString getPass();
    QString getLogin();
    QJsonObject toJson();

};

#endif // RECORD_H
