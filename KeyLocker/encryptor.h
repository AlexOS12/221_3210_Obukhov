#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <QByteArray>
#include <QDataStream>
#include <QTextStream>
#include <QIODevice>
#include <QFile>
#include <QDebug>
#include <openssl/evp.h>

class Encryptor
{
public:
    static Encryptor& getInstance()
    {
        static Encryptor singleton;
        return singleton;
    }
    void test(QString path);

    // Функция шифровщик
    int encrypt(QByteArray &in, QByteArray &out);
    // Функция дешифровщик
    int decrypt(QByteArray &in, QByteArray &out);
private:
    const QByteArray keyQBA = QByteArray::fromHex("03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4");
    const QByteArray  ivQBA = QByteArray::fromHex("81dc9bdb52d04dc20036dbd8313ed055");

    Encryptor(){};
    Encryptor(const Encryptor& root) = delete;
    Encryptor& operator=(const Encryptor&) = delete;
};

#endif // ENCRYPTOR_H
