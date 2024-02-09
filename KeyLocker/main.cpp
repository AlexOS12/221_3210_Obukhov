#include "authwindow.h"
#include "mainwindow.h"

#include <QObject>
#include <QApplication>

class Initiator {
    Q_OBJECT
private:
    AuthWindow authWindow;

public slots:
    void initialize(bool authResult) {
        if (authResult) {
            MainWindow mainWindow;
            mainWindow.show();
        } else {
            qDebug() << "wrong pin";
        }
    }

public:
    Initiator() {
        QObject::connect(&authWindow, SIGNAL(sendAuthResult(bool)), this, SLOT(initialize(bool)));
        authWindow.show();
    }

};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Initiator initiator;

    return a.exec();
}
