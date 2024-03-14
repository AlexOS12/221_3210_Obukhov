#include "record.h"

Record::Record() {}

Record::Record(QString site, QString login, QString pass) {
    this->site = site;
    this->login = login;
    this->pass = pass;
}

Record::Record(QString site, QByteArray credentials)
{
    this->site = site;
    this->credentials = QByteArray::fromHex(credentials);
}

QString Record::getPass(QByteArray key, QByteArray iv)
{
    QByteArray decrypted;

    Encryptor::getInstance().decrypt(this->credentials, decrypted, key, iv);
    QJsonDocument credits;
    credits = QJsonDocument::fromJson(decrypted);
    return credits["pass"].toString();
}

QString Record::getLogin(QByteArray key, QByteArray iv)
{
    QByteArray decrypted;

    Encryptor::getInstance().decrypt(this->credentials, decrypted, key, iv);

    QJsonDocument credits;
    credits = QJsonDocument::fromJson(decrypted);
    return credits["login"].toString();
}

QJsonObject Record::toJson()
{
    QJsonObject json;

    json.insert("site", this->site);
    json.insert("credentials", QString(this->credentials.toHex()));

    return json;
}

void Record::reEncryptCredentials(QByteArray oldKey, QByteArray oldIV, QByteArray newKey, QByteArray newIV)
{
    QByteArray newCredentials;
    QByteArray decryptedCredits;

    Encryptor::getInstance().decrypt(this->credentials, decryptedCredits, oldKey, oldIV);
    Encryptor::getInstance().encrypt(decryptedCredits, newCredentials, newKey, newIV);

    this->credentials = newCredentials;
    qDebug() << this->credentials;
}
