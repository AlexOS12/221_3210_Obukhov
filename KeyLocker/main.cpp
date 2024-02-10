#include "initiator.h"

#include <QObject>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Initiator initiator;

    return a.exec();
}
