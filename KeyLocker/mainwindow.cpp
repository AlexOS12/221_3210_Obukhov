#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::readRecords()
{
    QFile file;
    file.setFileName(homeDir + "/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.close();
        return false;
    } else {
        QByteArray fileContent = QByteArray::fromHex(file.readAll());
        qDebug() << fileContent;
        QByteArray decryptedFile;
        Encryptor::getInstance().decrypt(fileContent, decryptedFile,
                                         QCryptographicHash::hash(currPin, QCryptographicHash::Sha256),
                                         QCryptographicHash::hash(currPin, QCryptographicHash::Md5));
        qDebug() << "Decrypted:";
        qDebug() << decryptedFile;
        QJsonDocument jsonDoc;
        jsonDoc = QJsonDocument::fromJson(decryptedFile);
        QJsonArray jsonArray = jsonDoc.array();

        for (int i = 0; i < jsonArray.size(); i++) {
            QJsonValue value = jsonArray.at(i);
            QString site = value["site"].toString();
            QString credentials = value["credentials"].toString();

            Record record(site, credentials);
            records.append(record);
        }
    }
    displayRecords();
    file.close();
    this->recordsRead = true;
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->wrongPassLbl->hide();
    ui->changePinResult->hide();
    ui->oldPinEdit->hide();
    ui->newPinEdit->hide();
    ui->confPinEdit->hide();
    homeDir = QDir::homePath() + "/KeyLocker";
    QObject::connect(&recordEditor, SIGNAL(sendRecord(Record)), this, SLOT(addRecord(Record)));
}

MainWindow::~MainWindow()
{
    // пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅ json
    if (recordsRead) {
        QJsonArray array;
        for (Record record : records) {
            array.push_back(record.toJson());
        }

        QJsonDocument jsonDoc(array);

        QFile file;
        file.setFileName(homeDir + "/data.txt");

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
        } else {
            QByteArray encryptedFile;
            QByteArray json = jsonDoc.toJson(QJsonDocument::Compact);
            qDebug() << json;
            Encryptor::getInstance().encrypt(json, encryptedFile,
                                             QCryptographicHash::hash(currPin, QCryptographicHash::Sha256),
                                             QCryptographicHash::hash(currPin, QCryptographicHash::Md5));
            qDebug() << "Encrypted";
            qDebug() << encryptedFile;
            QTextStream out(&file);
            out << encryptedFile.toHex();
            file.close();
        }
    }

    delete ui;
}

void MainWindow::addRecord(Record record)
{
    records.push_back(record);
    ui->listWidget->addItem(record.site);
}

void MainWindow::on_addRecord_clicked()
{
    recordEditor.show();
}

void MainWindow::displayRecords()
{
    for (Record record : records) {
        ui->listWidget->addItem(record.site);
    }
}

void MainWindow::showRecord(uint recordId)
{
    ui->siteView->setText(records[recordId].site);
    ui->loginView->setText(records[recordId].getLogin());
    ui->passView->setText(records[recordId].getPass());
}


void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    uint recordId = ui->listWidget->currentRow();
    ui->stackedWidget->setCurrentIndex(2);
    showRecord(recordId);
}


void MainWindow::on_pinEdit_returnPressed()
{
    QFile pinContainer;
    pinContainer.setFileName(homeDir + "/pin.txt");

    if (!pinContainer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->wrongPassLbl->show();
        ui->wrongPassLbl->setText("РќРµ СѓРґР°Р»РѕСЃСЊ РїСЂРѕС‡РёС‚Р°С‚СЊ С„Р°Р№Р» СЃ РїРёРЅРѕРј");
    } else {
        QByteArray pin = QByteArray::fromHex(pinContainer.readAll());
        QByteArray linePin = ui->pinEdit->text().toUtf8();
        QByteArray md5pin = QCryptographicHash::hash(linePin, QCryptographicHash::Md5);
        qDebug() << md5pin;
        qDebug() << pin;
        if (md5pin == pin) {
            ui->stackedWidget->setCurrentIndex(1);
            ui->wrongPassLbl->hide();
            currPin = ui->pinEdit->text().toUtf8();
            ui->pinEdit->setText("");
            readRecords();
        } else {
            ui->pinEdit->clear();
            ui->wrongPassLbl->setText("РќРµРІРµСЂРЅС‹Р№ РїРёРЅ-РєРѕРґ");
            ui->wrongPassLbl->show();
        }
    }
}


void MainWindow::on_showLoginBtn_clicked()
{
    if (ui->loginView->echoMode() == QLineEdit::Password) {
        ui->loginView->setEchoMode(QLineEdit::Normal);
    } else {
        ui->loginView->setEchoMode(QLineEdit::Password);
    }
}


void MainWindow::on_showPassBtn_clicked()
{
    if (ui->passView->echoMode() == QLineEdit::Password) {
        ui->passView->setEchoMode(QLineEdit::Normal);
    } else {
        ui->passView->setEchoMode(QLineEdit::Password);
    }
}


void MainWindow::on_okBtn_clicked()
{
    ui->siteView->setText("");
    ui->loginView->setText("");
    ui->loginView->setEchoMode(QLineEdit::Password);
    ui->passView->setText("");
    ui->passView->setEchoMode(QLineEdit::Password);
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_changePinBtn_clicked()
{
    if (changePinMenuOpened) {
        ui->newPinEdit->hide();
        ui->oldPinEdit->hide();
        ui->confPinEdit->hide();
        ui->changePinResult->hide();
    } else {
        ui->newPinEdit->show();
        ui->oldPinEdit->show();
        ui->confPinEdit->show();
        ui->changePinResult->hide();
    }
    changePinMenuOpened = !changePinMenuOpened;
}


void MainWindow::on_newPinEdit_returnPressed()
{
    QFile pinContainer;
    pinContainer.setFileName(homeDir + "/pin.txt");

    if (!pinContainer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->changePinResult->show();
        ui->changePinResult->setText("Не удалось открыть файл с пином");
        qDebug() << "Failed to open pin file";
        ui->changePinResult->setStyleSheet("color: rgb(255, 0, 0);");
    } else {
        QByteArray currPin = QByteArray::fromHex(pinContainer.readAll());
        pinContainer.close();
        QByteArray oldPin = ui->oldPinEdit->text().toUtf8();
        QByteArray oldMd5Pin = QCryptographicHash::hash(oldPin, QCryptographicHash::Md5);
        if (oldMd5Pin != currPin) {
            ui->changePinResult->show();
            ui->changePinResult->setText("Введен неверный старый пин!");
            qDebug() << "Old pin is wrong";
            ui->changePinResult->setStyleSheet("color: rgb(255, 0, 0);");
            return;
        }
        if (ui->newPinEdit->text() != ui->confPinEdit->text()) {
            ui->changePinResult->show();
            ui->changePinResult->setText("Новые пины различаются!");
            qDebug() << "New pins a different";
            ui->changePinResult->setStyleSheet("color: rgb(255, 0, 0);");
            return;
        }

        QByteArray newMd5Pin = QCryptographicHash::hash(ui->newPinEdit->text().toUtf8(), QCryptographicHash::Md5);
        QFile newPinWriter;
        newPinWriter.setFileName(homeDir + "/pin.txt");

        if (!newPinWriter.open(QIODevice::ReadWrite | QIODevice::Text)) {
            ui->changePinResult->show();
            ui->changePinResult->setText("Не удалось сохранить новый пин!");
            ui->changePinResult->setStyleSheet("color: rgb(255, 0, 0);");
            return;
        }

        this->currPin = ui->newPinEdit->text().toUtf8();
        ui->changePinResult->show();
        ui->changePinResult->setText("Пин успешно изменён!");
        ui->changePinResult->setStyleSheet("color: rgb(0, 0, 0);");

        ui->oldPinEdit->clear();
        ui->newPinEdit->clear();
        ui->confPinEdit->clear();

        QTextStream out(&newPinWriter);
        out << newMd5Pin.toHex();
        newPinWriter.close();
    }
}

