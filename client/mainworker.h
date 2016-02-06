#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "client.h"
#include <QObject>

class MainWorker : public QObject {
    Q_OBJECT
public:
    explicit MainWorker(Client *client, QObject *parent = 0);

public slots:
    void run();

private:
    Client *client;
};

#endif // MAINWORKER_H
