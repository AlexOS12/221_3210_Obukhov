#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <QByteArray>
#include <openssl/evp.h>

class Encryptor
{
public:
    static Encryptor& getInstance()
    {
        static Encryptor singleton;
        return singleton;
    }

private:
    const QByteArray key = QByteArray::fromHex("03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4");
    const QByteArray  iv = QByteArray::fromHex("81dc9bdb52d04dc20036dbd8313ed055");

    // Функция шифровщик
    QByteArray encrypt(QByteArray &in);
    // Функция дешифровщик
    QByteArray decrypt(QByteArray &in);

    Encryptor(){};
    Encryptor(const Encryptor& root) = delete;
    Encryptor& operator=(const Encryptor&) = delete;
};

#endif // ENCRYPTOR_H
