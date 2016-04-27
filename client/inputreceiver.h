#ifndef INPUTRECEIVER_H
#define INPUTRECEIVER_H

#include <QIODevice>
#include <QAudioFormat>

class InputReceiver : public QIODevice {
    Q_OBJECT
public:
    InputReceiver(const QAudioFormat &format, QObject *parent = 0);
    ~InputReceiver();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    QAudioFormat format;
};

#endif // INPUTRECEIVER_H
