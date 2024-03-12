#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::reEncryptRecords(QString oldPin, QString newPin)
{
    QByteArray oldPinSha256 = QCryptographicHash::hash(oldPin.toUtf8(), QCryptographicHash::Sha256);
    QByteArray oldPinMd5 = QCryptographicHash::hash(oldPin.toUtf8(), QCryptographicHash::Md5);
    QByteArray newPinSha256 = QCryptographicHash::hash(newPin.toUtf8(), QCryptographicHash::Sha256);
    QByteArray newPinMd5 = QCryptographicHash::hash(newPin.toUtf8(), QCryptographicHash::Md5);

    for (int i = 0; i < records.size(); i++) {
        records[i].reEncryptCredentials(oldPinSha256, oldPinMd5, newPinSha256, newPinMd5);
    }
}

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
            QByteArray credentials = value["credentials"].toString().toUtf8();

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
    ui->resultLabel->hide();
    ui->oldPinEdit->hide();
    ui->newPinEdit->hide();
    ui->confPinEdit->hide();
    ui->newRecSite->hide();
    ui->newRecLogin->hide();
    ui->newRecPass->hide();
    ui->addNewRecBtn->hide();
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
    // recordEditor.show();
    this->addRecordMenuOpened = !this->addRecordMenuOpened;

    if (this->addRecordMenuOpened) {
        ui->newRecSite->show();
        ui->newRecLogin->show();
        ui->newRecPass->show();
        ui->addNewRecBtn->show();
    } else {
        ui->newRecSite->hide();
        ui->newRecLogin->hide();
        ui->newRecPass->hide();
        ui->addNewRecBtn->hide();
    }
}

void MainWindow::displayRecords()
{
    for (Record record : records) {
        ui->listWidget->addItem(record.site);
    }
}

void MainWindow::showRecord(uint recordId)
{
    this->selectedRecord = recordId;
    ui->siteView->setText(records[recordId].site);
    ui->loginView->setText("********");
    ui->passView->setText("********");
    // ui->loginView->setText(records[recordId].getLogin(QCryptographicHash::hash(currPin, QCryptographicHash::Sha256), QCryptographicHash::hash(currPin, QCryptographicHash::Md5)));
    // ui->passView->setText(records[recordId].getPass(QCryptographicHash::hash(currPin, QCryptographicHash::Sha256), QCryptographicHash::hash(currPin, QCryptographicHash::Md5)));
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
        ui->resultLabel->hide();
    } else {
        ui->newPinEdit->show();
        ui->oldPinEdit->show();
        ui->confPinEdit->show();
        ui->resultLabel->hide();
    }
    changePinMenuOpened = !changePinMenuOpened;
}

void MainWindow::on_newPinEdit_returnPressed()
{
    qDebug() << "pin change";
    qDebug() << currPin;
    QFile pinContainer;
    pinContainer.setFileName(homeDir + "/pin.txt");

    if (!pinContainer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->resultLabel->show();
        ui->resultLabel->setText("Не удалось открыть файл с пином");
        qDebug() << "Failed to open pin file";
        ui->resultLabel->setStyleSheet("color: rgb(255, 0, 0);");
    } else {
        QByteArray currPin = QByteArray::fromHex(pinContainer.readAll());
        pinContainer.close();
        QByteArray oldPin = ui->oldPinEdit->text().toUtf8();
        QByteArray oldMd5Pin = QCryptographicHash::hash(oldPin, QCryptographicHash::Md5);
        if (oldMd5Pin != currPin) {
            ui->resultLabel->show();
            ui->resultLabel->setText("Введен неверный старый пин!");
            qDebug() << "Old pin is wrong";
            ui->resultLabel->setStyleSheet("color: rgb(255, 0, 0);");
            return;
        }
        if (ui->newPinEdit->text() != ui->confPinEdit->text()) {
            ui->resultLabel->show();
            ui->resultLabel->setText("Новые пины различаются!");
            qDebug() << "New pins a different";
            ui->resultLabel->setStyleSheet("color: rgb(255, 0, 0);");
            return;
        }

        QByteArray newMd5Pin = QCryptographicHash::hash(ui->newPinEdit->text().toUtf8(), QCryptographicHash::Md5);
        QFile newPinWriter;
        newPinWriter.setFileName(homeDir + "/pin.txt");

        if (!newPinWriter.open(QIODevice::ReadWrite | QIODevice::Text)) {
            ui->resultLabel->show();
            ui->resultLabel->setText("Не удалось сохранить новый пин!");
            ui->resultLabel->setStyleSheet("color: rgb(255, 0, 0);");
            return;
        }

        qDebug() << this->currPin;
        reEncryptRecords(QString(this->currPin), ui->newPinEdit->text());
        this->currPin.clear();
        this->currPin.append(ui->newPinEdit->text().toUtf8());
        qDebug() << this->currPin;
        ui->resultLabel->show();
        ui->resultLabel->setText("Пин успешно изменён!");
        ui->resultLabel->setStyleSheet("color: rgb(0, 0, 0);");

        ui->oldPinEdit->clear();
        ui->newPinEdit->clear();
        ui->confPinEdit->clear();

        QTextStream out(&newPinWriter);
        out << newMd5Pin.toHex();
        newPinWriter.close();
    }
}


void MainWindow::on_addNewRecBtn_clicked()
{
    QString site = ui->newRecSite->text();
    QString login = ui->newRecLogin->text();
    QString pass = ui->newRecPass->text();

    QJsonObject credits;
    credits.insert("login", login);
    credits.insert("pass", pass);

    QJsonDocument jdoc(credits);

    QByteArray qbaCredts, encCredits;
    qbaCredts = jdoc.toJson(QJsonDocument::Compact);

    Encryptor::getInstance().encrypt(qbaCredts, encCredits, QCryptographicHash::hash(currPin, QCryptographicHash::Sha256),
                                     QCryptographicHash::hash(currPin, QCryptographicHash::Md5));

    Record record(site, encCredits.toHex());

    this->records.push_back(record);
    ui->listWidget->addItem(record.site);

    ui->newRecSite->clear();
    ui->newRecSite->hide();
    ui->newRecLogin->clear();
    ui->newRecLogin->hide();
    ui->newRecPass->clear();
    ui->newRecPass->hide();
    ui->addNewRecBtn->hide();
}


void MainWindow::on_copyLoginBtn_clicked()
{
    QString login;
    login = records[selectedRecord].getLogin(QCryptographicHash::hash(currPin, QCryptographicHash::Sha256),
                                             QCryptographicHash::hash(currPin, QCryptographicHash::Md5));

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(login);
    qDebug() << login;
}


void MainWindow::on_copyPassBtn_clicked()
{
    QString pass;
    pass = records[selectedRecord].getPass(QCryptographicHash::hash(currPin, QCryptographicHash::Sha256),
                                             QCryptographicHash::hash(currPin, QCryptographicHash::Md5));

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(pass);
    qDebug() << pass;
}

