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

void MainWindow::changePin()
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
            ui->resultLabel->setText("Введен неверный текущий пин!");
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
        ui->resultLabel->setText("Пин успешно изменен!");
        ui->resultLabel->setStyleSheet("color: rgb(0, 0, 0);");

        ui->oldPinEdit->clear();
        ui->newPinEdit->clear();
        ui->confPinEdit->clear();

        ui->oldPinEdit->hide();
        ui->newPinEdit->hide();
        ui->confPinEdit->hide();
        ui->changeBtn->hide();

        this->changePinMenuOpened = false;

        QTextStream out(&newPinWriter);
        out << newMd5Pin.toHex();
        newPinWriter.close();
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
    file.close();
    this->recordsRead = true;
    displayRecords();
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
    ui->changeBtn->hide();
    ui->confPinEdit->hide();
    ui->newRecSite->hide();
    ui->newRecLogin->hide();
    ui->newRecPass->hide();
    ui->addNewRecBtn->hide();
    homeDir = QDir::homePath() + "/KeyLocker";
}

MainWindow::~MainWindow()
{
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

void MainWindow::copyRecordPass(int recordId)
{
    QString pass;
    pass = records[recordId].getPass(QCryptographicHash::hash(currPin, QCryptographicHash::Sha256),
                                             QCryptographicHash::hash(currPin, QCryptographicHash::Md5));

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(pass);
    qDebug() << pass;
}

void MainWindow::copyRecordLogin(int recordId)
{
    QString login;
    login = records[recordId].getLogin(QCryptographicHash::hash(currPin, QCryptographicHash::Sha256),
                                             QCryptographicHash::hash(currPin, QCryptographicHash::Md5));

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(login);
    qDebug() << login;
}

void MainWindow::deleteRecord(int recordId)
{
    this->records.remove(recordId);
    this->displayRecords();
}

void MainWindow::on_addRecord_clicked()
{
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
    QString searchFor = ui->searchLine->text();

    ui->listWidget->clear();

    for (int i = 0; i < this->records.size(); i++) {
        if (searchFor.isEmpty() || this->records[i].site.contains(searchFor)) {
            qDebug() << i;

            QListWidgetItem* item = new QListWidgetItem();

            recordWiget* recordwidget = new recordWiget(ui->listWidget, i, records[i].site);

            item->setSizeHint(recordwidget->size());

            QObject::connect(recordwidget, SIGNAL(copyRecordLogin(int)), this, SLOT(copyRecordLogin(int)));
            QObject::connect(recordwidget, SIGNAL(copyRecordPass(int)), this, SLOT(copyRecordPass(int)));
            QObject::connect(recordwidget, SIGNAL(deleteRecord(int)), this, SLOT(deleteRecord(int)));

            ui->listWidget->addItem(item);
            ui->listWidget->setItemWidget(item, recordwidget);
        }
    }
}

void MainWindow::showRecord(uint recordId)
{
    this->selectedRecord = recordId;
    ui->siteView->setText(records[recordId].site);
    ui->loginView->setText("********");
    ui->passView->setText("********");
}



void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
}


void MainWindow::on_pinEdit_returnPressed()
{
    QFile pinContainer;
    pinContainer.setFileName(homeDir + "/pin.txt");

    if (!pinContainer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->wrongPassLbl->show();
        ui->wrongPassLbl->setText("Не удалось прочитать файл с пином");
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
            ui->wrongPassLbl->setText("Неверный пин-код");
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
        ui->changeBtn->hide();
    } else {
        ui->newPinEdit->show();
        ui->oldPinEdit->show();
        ui->confPinEdit->show();
        ui->changeBtn->show();
        ui->resultLabel->hide();
    }
    changePinMenuOpened = !changePinMenuOpened;
}



void MainWindow::on_newPinEdit_returnPressed()
{
    changePin();
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

    QListWidgetItem* item = new QListWidgetItem();

    recordWiget* recordwidget = new recordWiget(ui->listWidget, records.size() - 1, site);
    QObject::connect(recordwidget, SIGNAL(copyRecordLogin(int)), this, SLOT(copyRecordLogin(int)));
    QObject::connect(recordwidget, SIGNAL(copyRecordPass(int)), this, SLOT(copyRecordPass(int)));

    item->setSizeHint(recordwidget->size());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, recordwidget);

    ui->newRecSite->clear();
    ui->newRecSite->hide();
    ui->newRecLogin->clear();
    ui->newRecLogin->hide();
    ui->newRecPass->clear();
    ui->newRecPass->hide();
    ui->addNewRecBtn->hide();

    this->addRecordMenuOpened = false;
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

void MainWindow::on_changeBtn_clicked()
{
    changePin();
}


void MainWindow::on_searchLine_textEdited(const QString &arg1)
{

    displayRecords();
}



