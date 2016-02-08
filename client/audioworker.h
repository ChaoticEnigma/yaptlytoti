#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "client.h"
#include <QObject>

class AudioWorker : public QObject {
    Q_OBJECT
public:
    explicit AudioWorker(Client *client, QObject *parent = 0);

public slots:
    void run();
    void inputNotified();
    void outputNotified();

private:
    Client *client;
};

#endif // MAINWORKER_H
