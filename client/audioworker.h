#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "client.h"
#include <QObject>
#include <QByteArray>
#include <QBuffer>

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
    QByteArray *inputdata;
    QBuffer *inputbuffer;
    QByteArray *outputdata;
    QBuffer *outputbuffer;
};

#endif // MAINWORKER_H
