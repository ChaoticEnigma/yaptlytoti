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
    void inputRead();
    void inputNotified();
    void outputNotified();

private:
    Client *client;
    //QByteArray *inputdata;
    //QBuffer *inputbuffer;
    QByteArray *inputbuffer;
    QIODevice *input;
    //QByteArray *outputdata;
    //QBuffer *outputbuffer;
    QIODevice *output;
};

#endif // MAINWORKER_H
