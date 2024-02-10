#include "initiator.h"

void Initiator::initialize(bool authResult)
{
    if (authResult) {
        authWindow.close();
        mainWindow.show();
        mainWindow.readRecords();
    }
}

Initiator::Initiator() {
    QObject::connect(&authWindow, SIGNAL(sendAuthResult(bool)), this, SLOT(initialize(bool)));
    authWindow.show();
}
