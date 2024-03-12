#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include "encryptor.h"

class Record
{
private:
    QString login;
    QString pass;
    QByteArray credentials;
public:
    Record();
    Record(QString site, QString login, QString pass);
    Record(QString site, QByteArray credentials);
    QString site;
    QString getPass(QByteArray key, QByteArray iv);
    QString getLogin(QByteArray key, QByteArray iv);
    QJsonObject toJson();
    void reEncryptCredentials(QByteArray oldKey, QByteArray oldIV, QByteArray newKey, QByteArray newIV);

};

#endif // RECORD_H
