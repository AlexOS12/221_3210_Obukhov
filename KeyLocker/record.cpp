#include "record.h"

Record::Record() {}

Record::Record(QString site, QString login, QString pass) {
    this->site = site;
    this->login = login;
    this->pass = pass;
}

Record::Record(QString site,QString credentials)
{
    this->site = site;
    this->credentials = credentials;
}

QString Record::getPass()
{
    QJsonDocument credits;
    credits = QJsonDocument::fromJson(this->credentials.toUtf8());
    return credits["pass"].toString();
}

QString Record::getLogin()
{
    QJsonDocument credits;
    credits = QJsonDocument::fromJson(this->credentials.toUtf8());
    return credits["login"].toString();
}

QJsonObject Record::toJson()
{
    QJsonObject json;

    json.insert("site", this->site);
    json.insert("credentials", this->credentials);

    return json;
}
