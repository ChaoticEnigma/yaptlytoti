#ifndef INPUTRECEIVER_H
#define INPUTRECEIVER_H

#include <QIODevice>
#include <QAudioFormat>
#include "zarray.h"
#include "voip.h"

using namespace LibChaos;

class InputReceiver : public QIODevice {
    Q_OBJECT
public:
    InputReceiver(const QAudioFormat &format, VoIP *voip, QObject *parent = 0);
    ~InputReceiver();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

signals:
    void audioReady(const ZArray<zs16> *samples);

private:
    QAudioFormat format;
    VoIP *voip;
};

#endif // INPUTRECEIVER_H
