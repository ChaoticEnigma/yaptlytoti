#ifndef CODEC_H
#define CODEC_H

#include "opus.h"

class Codec {
public:
    enum CodecType {
        CodecOpus
    };

public:
    Codec(CodecType type);

private:
    OpusEncoder *encode;
    OpusDecoder *decode;
};

#endif // CODEC_H
