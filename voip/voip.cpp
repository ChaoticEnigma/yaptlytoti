#include "voip.h"

VoIP::VoIP() : audiocodec(nullptr), network(nullptr){

}

VoIP::~VoIP(){
    delete audiocodec;
    delete network;
}

void VoIP::sendAudio(const AudioData *data, int length){
    //qDebug() << "Send" << length;
    ZBinary outdata;
    audiocodec->encode(data, length, outdata);
    //qDebug() << "Encoded:" << outdata.size();
    VoIPMessage *message = new VoIPMessage(QHostAddress::LocalHost, 7777);
    message->setType(VoIPMessage::TYPE_AUDIO_OPUS);
    message->payload().write(outdata.raw(), outdata.size());
    network->sendMessage(message);
}

void VoIP::decodeMessage(VoIPMessage *message){
    qDebug() << "Recv:" << message->getHost() << message->getPort() << message->payload().size();
    AudioData *outdata = new AudioData;
    audiocodec->decode(message->payload(), outdata);
    delete message;
    //qDebug() << "Decoded:" << outdata->size();
    emit decodedAudio(outdata);
}

void VoIP::initCodec(Codec::CodecType type){
    delete audiocodec;
    audiocodec = new Codec(type);
}

void VoIP::initNetwork(quint16 port){
    delete network;
    network = new Network(this);
    network->init(port);
}
