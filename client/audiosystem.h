#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "client.h"
#include <QObject>
#include <QByteArray>
#include <QBuffer>

#include "opus/opus.h"

class AudioSystem : public QObject {
    Q_OBJECT
public:
    explicit AudioSystem(Client *client, QObject *parent = 0);
    ~AudioSystem();

    void initInput();
    void initOutput();

public slots:
    void inputRead();
    void playAudio(const AudioData *data);

private:
    Client *client;
    QIODevice *inputDevice;     // Provided by QAudioInput
    QIODevice *outputDevice;    // Provided by QAudioOutput
    AudioData *loopbuffer;
};

#endif // MAINWORKER_H
