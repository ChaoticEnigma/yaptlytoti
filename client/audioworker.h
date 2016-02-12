#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "client.h"
#include <QObject>
#include <QByteArray>
#include <QBuffer>

#include "opus/opus.h"

class AudioWorker : public QObject {
    Q_OBJECT
public:
    explicit AudioWorker(Client *client, QObject *parent = 0);
    ~AudioWorker();

public slots:
    void run();
    void initInput();
    void initOutput();
    void inputRead();
    void inputNotified();
    void outputNotified();

private:
    Client *client;
    QIODevice *input;   // Provided by QAudioInput
    QIODevice *output;  // Provided by QAudioOutput
    QByteArray *loopbuffer;
    OpusEncoder *encode;
    OpusDecoder *decode;
};

#endif // MAINWORKER_H
