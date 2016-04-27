#ifndef CODEC_H
#define CODEC_H

#include <QByteArray>
#include "opus/opus.h"

#define DEFAULT_CHANNELS    1
#define SAMPLE_SIZE 16
#define DEFAULT_SAMPLE_RATE 16000

#define DEFAULT_ENCODE_BUFFER 4000
#define DEFAULT_DECODE_BUFFER 4000

typedef QVector<qint16> AudioData;

class Codec {
public:
    enum CodecType {
        CODEC_OPUS
    };

public:
    Codec(CodecType type);

    void encode(const AudioData *indata, int frame_size, QByteArray &outdata);
    void decode(const QByteArray &indata, AudioData *outdata);
    int checkReadAvailBytes(int avail);

private:
    int rate;
    OpusEncoder *encoder;
    OpusDecoder *decoder;
};

#endif // CODEC_H
