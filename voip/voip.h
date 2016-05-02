#ifndef VOIP_H
#define VOIP_H

#include "codec.h"
#include "network.h"

#include <QObject>
#include <QThread>

#include "zrandom.h"

class VoIP : public QObject {
    Q_OBJECT
public:
    VoIP();
    ~VoIP();

    //! Initialize codec with \a type.
    void initCodec(Codec::CodecType type);
    //! Initialize networking listening on \a port.
    void initNetwork(quint16 port);

    //! Connect to server.
    void connect(QHostAddress addr, zu16 port);

    Codec *codec(){ return audiocodec; }

public slots:
    void inputPCM(const ZArray<zs16> *data);
    void decodeMessage(VoIPMessage *message);

signals:
    void encodedMessage(VoIPMessage *message);
    void decodedAudio(const ZArray<zs16> *data);

private:
    void switcher(VoIPMessage *message);

private:
    Codec *audiocodec;
    Network *network;
    QThread *netthread;
    ZRandom rand;
};

#endif // VOIP_H
