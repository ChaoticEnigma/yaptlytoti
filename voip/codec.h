#ifndef CODEC_H
#define CODEC_H

#include <QVector>
#include "zbinary.h"
#include "zarray.h"
#include "opus/opus.h"

using namespace LibChaos;

#define DEFAULT_CHANNELS        1
#define SAMPLE_SIZE             16
#define DEFAULT_SAMPLE_RATE     16000

#define DEFAULT_ENCODE_BUFFER   4000
#define DEFAULT_DECODE_BUFFER   4000

// https://wiki.xiph.org/OpusFAQ

class Codec {
public:
    enum CodecType {
        CODEC_OPUS
    };

public:
    Codec(CodecType type);

    zu64 encode(const ZArray<zs16> *samples, ZBinary &outdata);
    void decode(const ZBinary &indata, ZArray<zs16> *outdata);
    int checkReadSampleCount(zu16 avail);

private:
    int rate;
    ZArray<zs16> samplebuffer;
    OpusEncoder *encoder;
    OpusDecoder *decoder;
};

#endif // CODEC_H
