#ifndef INITIATOR_H
#define INITIATOR_H

#include <QObject>
#include "authwindow.h"
#include "mainwindow.h"


class Initiator : public QObject
{
    Q_OBJECT

    AuthWindow authWindow;
    MainWindow mainWindow;

public slots:
    void initialize(bool authResult);
public:
    explicit Initiator();
};

#endif // INITIATOR_H
