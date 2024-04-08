#include "record.h"

Record::Record(QString site, QString login, QString pass) {
    this->site = site;
    this->login = login;
    this->pass = pass;
}

QString Record::getPass()
{
    return this->pass;
}

QString Record::getLogin()
{
    return this->login;
}

QJsonObject Record::toJson()
{
    QJsonObject json;

    json.insert("site", this->site);
    json.insert("login", this->login);
    json.insert("pass", this->pass);

    return json;
}
