#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "client.h"
#include "inputreceiver.h"
#include <QObject>
#include <QByteArray>
#include <QBuffer>

class AudioSystem : public QObject {
    Q_OBJECT
public:
    explicit AudioSystem(Client *client, QObject *parent = 0);
    ~AudioSystem();

    void initInput();
    void initOutput();

public slots:
    void inputRead();
    void playAudio(const ZArray<zs16> *data);

private:
    Client *client;
    QIODevice *inputDevice;     // Provided by QAudioInput
    QIODevice *outputDevice;    // Provided by QAudioOutput
    ZArray<zs16> *loopbuffer;
    InputReceiver *receiver;

};

#endif // MAINWORKER_H
