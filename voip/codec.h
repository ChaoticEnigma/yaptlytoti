#ifndef CODEC_H
#define CODEC_H

#include "opus.h"
#include "ztypes.h"
using namespace LibChaos;

class Codec {
public:
    enum CodecType {
        CODECOPUS
    };

public:
    Codec(int samplert, int channels);

    virtual zu64 encoder(zs16 *samples, zu64 count, zu8 *encoded, zu64 max) = 0;
    virtual CodecType type() = 0;

protected:
    int samplerate;
    int channelcount;
};

#endif // CODEC_H
