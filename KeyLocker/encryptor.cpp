#include "encryptor.h"

void Encryptor::test(QString path)
{
    QFile file;
    file.setFileName(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "File Read Error!";
        file.close();
    } else {
        QByteArray fileContent = file.readAll();
        qDebug() << "Encrypted:" << "\n" << fileContent;
        QByteArray encrypted = QByteArray::fromHex(fileContent);
        QByteArray decrypted;
        decrypt(encrypted, decrypted);
        qDebug() << "Decrypted:" << "\n" << decrypted;

    }
}

int Encryptor::encrypt(QByteArray &in, QByteArray &out)
{
    unsigned char key[32], iv[16];
    const int buffLen = 256;
    unsigned char encBuffer[buffLen] = {0},
        decBuffer[buffLen] = {0};
    int encLen, decLen;
    QDataStream encStream(in);
    QDataStream decStream(&out, QIODevice::ReadWrite);

    memcpy(key, this->keyQBA.data(), 32);
    memcpy(iv, this->ivQBA.data(), 16);

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }
    do {
        decLen = decStream.readRawData(reinterpret_cast<char*>(decBuffer), buffLen);
        if (!EVP_EncryptUpdate(ctx, encBuffer, &encLen, decBuffer, decLen)){
            EVP_CIPHER_CTX_free(ctx);
            return 1;
        }
        encStream.writeRawData(reinterpret_cast<char*>(encBuffer), encLen);
    } while (decLen > 0);

    if (!EVP_EncryptFinal_ex(ctx, encBuffer, &encLen)){
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    encStream.writeRawData(reinterpret_cast<char*>(encBuffer), encLen);
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

int Encryptor::decrypt(QByteArray &in, QByteArray &out)
{
    unsigned char key[32], iv[16];
    const int buffLen = 256;
    unsigned char encBuffer[buffLen] = {0},
        decBuffer[buffLen] = {0};
    int encLen, decLen;
    QDataStream encStream(in);
    QDataStream decStream(&out, QIODevice::ReadWrite);

    memcpy(key, this->keyQBA.data(), 32);
    memcpy(iv, this->ivQBA.data(), 16);

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }
    do {
        encLen = encStream.readRawData(reinterpret_cast<char*>(encBuffer), buffLen);
        if (!EVP_DecryptUpdate(ctx, decBuffer, &decLen, encBuffer, encLen)) {
            EVP_CIPHER_CTX_free(ctx);
            return 1;
        }
        decStream.writeRawData(reinterpret_cast<char*>(decBuffer), decLen);
    } while (encLen > 0);

    if (!EVP_DecryptFinal_ex(ctx, decBuffer, &decLen)){
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    decStream.writeRawData(reinterpret_cast<char*>(decBuffer), decLen);
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}
