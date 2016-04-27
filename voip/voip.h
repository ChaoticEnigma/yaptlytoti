#ifndef VOIP_H
#define VOIP_H

#include <QObject>
#include "codec.h"
#include "network.h"

class VoIP : public QObject {
    Q_OBJECT
public:
    VoIP();
    ~VoIP();

    void sendAudio(const AudioData *data, int length);

    // Network message received callback
    void decodeMessage(VoIPMessage *message);

    void initCodec(Codec::CodecType type);
    void initNetwork(quint16 port);

    Codec *codec(){ return audiocodec; }

signals:
    void decodedAudio(const AudioData *data);

private:
    Codec *audiocodec;
    Network *network;
};

#endif // VOIP_H
