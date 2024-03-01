#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::readRecords()
{
    QFile file;
    file.setFileName(homeDir + "/test.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    } else {
        QByteArray fileContent = QByteArray::fromHex(file.readAll());
        qDebug() << fileContent;
        QByteArray decryptedFile;
        Encryptor::getInstance().decrypt(fileContent, decryptedFile);
        qDebug() << "Decrypted:";
        qDebug() << decryptedFile;
        QJsonDocument jsonDoc;
        jsonDoc = QJsonDocument::fromJson(decryptedFile);
        QJsonArray jsonArray = jsonDoc.array();

        for (int i = 0; i < jsonArray.size(); i++) {
            QJsonValue value = jsonArray.at(i);
            QString site = value["site"].toString();
            QString login = value["login"].toString();
            QString pass = value["pass"].toString();

            Record record(site, login, pass);
            records.append(record);
        }
    }
    displayRecords();
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->wrongPassLbl->hide();
    homeDir = QDir::homePath() + "/KeyLocker";
    QObject::connect(&recordEditor, SIGNAL(sendRecord(Record)), this, SLOT(addRecord(Record)));
}

MainWindow::~MainWindow()
{
    // ������ ��������� � json
    QJsonArray array;
    for (Record record : records) {
        array.push_back(record.toJson());
    }

    QJsonDocument jsonDoc(array);

    QFile file;
    file.setFileName(homeDir + "/test.json");


    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.close();
    } else {
        QByteArray encryptedFile;
        QByteArray json = jsonDoc.toJson(QJsonDocument::Compact);
        qDebug() << json;
        Encryptor::getInstance().encrypt(json, encryptedFile);
        qDebug() << "Encrypted";
        qDebug() << encryptedFile;
        QTextStream out(&file);
        out << encryptedFile.toHex();
        file.close();
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
    QFile PinContainer;
    PinContainer.setFileName(homeDir + "/pin.txt");

    if (!PinContainer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->wrongPassLbl->show();
        ui->wrongPassLbl->setText("Не удалось прочитать файл с пином");
    } else {
        QByteArray pin = QByteArray::fromHex(PinContainer.readAll());
        QByteArray linePin = ui->pinEdit->text().toUtf8();
        QByteArray md5pin = QCryptographicHash::hash(linePin, QCryptographicHash::Md5);
        qDebug() << md5pin;
        qDebug() << pin;
        if (md5pin == pin) {
            ui->stackedWidget->setCurrentIndex(1);
            ui->wrongPassLbl->hide();
        } else {
            ui->pinEdit->clear();
            ui->wrongPassLbl->setText("Неверный пин-код");
            ui->wrongPassLbl->show();
        }
    }

    // if (PIN == ui->pinEdit->text()) {
    //     ui->stackedWidget->setCurrentIndex(1);
    //     ui->wrongPassLbl->hide();
    //     this->readRecords();
    // } else {
    //     ui->pinEdit->clear();
    //     ui->wrongPassLbl->show();
    // }
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

