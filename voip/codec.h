#ifndef CODEC_H
#define CODEC_H

#include <QVector>
#include "zbinary.h"
#include "opus/opus.h"

using namespace LibChaos;

#define DEFAULT_CHANNELS        1
#define SAMPLE_SIZE             16
#define DEFAULT_SAMPLE_RATE     48000

#define DEFAULT_ENCODE_BUFFER   4000
#define DEFAULT_DECODE_BUFFER   4000

// https://wiki.xiph.org/OpusFAQ

typedef QVector<qint16> AudioData;

class Codec {
public:
    enum CodecType {
        CODEC_OPUS
    };

public:
    Codec(CodecType type);

    void encode(const AudioData *indata, int frame_size, ZBinary &outdata);
    void decode(const ZBinary &indata, AudioData *outdata);
    int checkReadSampleCount(int avail);

private:
    int rate;
    OpusEncoder *encoder;
    OpusDecoder *decoder;
};

#endif // CODEC_H
