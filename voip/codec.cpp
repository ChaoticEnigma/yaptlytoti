#include "codec.h"

#include <QDebug>

Codec::Codec(CodecType type){

    switch(type){
    case CodecOpus: {
        int error;
        // Opus encoder
        encode = opus_encoder_create(client->audioInput->format().sampleRate(), client->audioInput->format().channelCount(), OPUS_APPLICATION_VOIP, &error);
        if(error < 0){
            qCritical() << "Faied to create opus encoder: " << opus_strerror(error);
            return;
        }
        error = opus_encoder_ctl(encode, OPUS_SET_BITRATE(OPUS_AUTO));
        if(error < 0){
            qCritical() << "Faied to set opus bitrate: " << opus_strerror(error);
            return;
        }

        // Opus decoder
        decode = opus_decoder_create(client->audioInput->format().sampleRate(), client->audioInput->format().channelCount(), &error);
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
