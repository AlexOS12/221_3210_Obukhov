#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class Record
{
private:
    QString login;
    QString pass;
    QString credentials;
public:
    Record();
    Record(QString site, QString login, QString pass);
    Record(QString site, QString credentials);
    QString site;
    QString getPass();
    QString getLogin();
    QJsonObject toJson();

};

#endif // RECORD_H
