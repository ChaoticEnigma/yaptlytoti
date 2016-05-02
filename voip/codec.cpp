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
        error = opus_encoder_ctl(encoder, OPUS_SET_INBAND_FEC(1));
        if(error < 0){
            qCritical() << "Faied to set opus bitrate: " << opus_strerror(error);
            return;
        }
        error = opus_encoder_ctl(encoder, OPUS_SET_PACKET_LOSS_PERC(10));
        if(error < 0){
            qCritical() << "Faied to set opus bitrate: " << opus_strerror(error);
            return;
        }

        // Opus decoder
        decoder = opus_decoder_create(DEFAULT_SAMPLE_RATE, DEFAULT_CHANNELS, &error);
        if(error < 0){
            qCritical() << "Failed to create opus decoder: " << opus_strerror(error);
            return;
        }
        break;
    }
    default:
        qCritical() << "Unknown Audio Codec";
        break;
    }

}

zu64 Codec::encode(const ZArray<zs16> *samples, ZBinary &outdata){
    samplebuffer.append(*samples);
    delete samples;
    int count = checkReadSampleCount(samplebuffer.size());
    //qDebug() << "Encode Samples" << count;
    if(count){
        outdata.resize(DEFAULT_ENCODE_BUFFER);
        opus_int32 bytes = opus_encode(encoder, samplebuffer.raw(), count, outdata.raw(), DEFAULT_ENCODE_BUFFER);
        if(bytes < 0){
            // error
            qCritical() << "Faied to encode opus:" << opus_strerror(bytes);
        } else {
            //qDebug() << "Encoded Bytes " << bytes;
            outdata.resize(bytes);
        }
        samplebuffer.erase(0, count);
        return bytes;
    }
    return 0;
}

void Codec::decode(const ZBinary &indata, ZArray<zs16> *outdata){
    outdata->resize(DEFAULT_DECODE_BUFFER);
    const zbyte *dptr = (indata.size() == 0 ? nullptr : indata.raw());
    //qDebug() << "Decode Bytes   " << indata.size();
    int samples = opus_decode(decoder, dptr, indata.size(), outdata->raw(), DEFAULT_DECODE_BUFFER, 0);
    if(samples < 0){
        // error
        qCritical() << "Faied to decode opus:" << opus_strerror(samples);
    } else {
        //qDebug() << "Decoded Samples" << samples;
        outdata->resize(samples);
    }
}

int Codec::checkReadSampleCount(zu16 avail){
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
