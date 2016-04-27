#include "codec.h"

#include <QDebug>

Codec::Codec(CodecType type) : rate(DEFAULT_SAMPLE_RATE), encoder(nullptr), decoder(nullptr){

    switch(type){
    case CODEC_OPUS: {
        int error;
        // Opus encoder
        encoder = opus_encoder_create(DEFAULT_SAMPLE_RATE, DEFAULT_CHANNELS, OPUS_APPLICATION_VOIP, &error);
        if(error < 0){
            qCritical() << "Faied to create opus encoder: " << opus_strerror(error);
            return;
        }
        error = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(OPUS_AUTO));
        if(error < 0){
            qCritical() << "Faied to set opus bitrate: " << opus_strerror(error);
            return;
        }

        // Opus decoder
        decoder = opus_decoder_create(DEFAULT_SAMPLE_RATE, DEFAULT_CHANNELS, &error);
        if(error < 0){
            qCritical() << "Faied to create opus decoder: " << opus_strerror(error);
            return;
        }
        break;
    }
    default:
        qCritical() << "Unknown Audio Codec";
        break;
    }

}

void Codec::encode(const AudioData *indata, int frame_size, QByteArray &outdata){
    outdata.resize(DEFAULT_ENCODE_BUFFER);
    opus_int32 bytes = opus_encode(encoder, indata->data(), frame_size, (unsigned char *)outdata.data(), DEFAULT_ENCODE_BUFFER);
    if(bytes < 0){
        // error
        qCritical() << "Faied to encode opus:" << opus_strerror(bytes);
    } else {
        //qDebug() << "Encoded Bytes:" << bytes;
        outdata.resize(bytes);
    }
}

void Codec::decode(const QByteArray &indata, AudioData *outdata){
    outdata->resize(DEFAULT_DECODE_BUFFER);
    const unsigned char *dptr = (indata.size() == 0 ? nullptr : (const unsigned char *)indata.data());
    int samples = opus_decode(decoder, dptr, indata.size(), outdata->data(), DEFAULT_DECODE_BUFFER, 0);
    if(samples < 0){
        // error
        qCritical() << "Faied to decode opus:" << opus_strerror(samples);
    } else {
        //qDebug() << "Decoded Samples:" << samples;
        outdata->resize(samples);
    }
}

int Codec::checkReadAvailBytes(int avail){
    if(avail >= rate * 60 / 1000)
        return (rate * 60) / 1000;
    if(avail >= rate * 40 / 1000)
        return (rate * 40) / 1000;
    if(avail >= rate * 20 / 1000)
        return (rate * 20) / 1000;
    if(avail >= rate * 10 / 1000)
        return (rate * 10) / 1000;
    if(avail >= rate * 5 / 1000)
        return (rate * 5) / 1000;
    if(avail >= rate * 5 / 2000)
        return (rate * 5) / 2000;
    return 0;
}
