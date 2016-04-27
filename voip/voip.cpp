#include "voip.h"

VoIP::VoIP() : audiocodec(nullptr), network(nullptr){

}

VoIP::~VoIP(){
    delete audiocodec;
    delete network;
}

void VoIP::sendAudio(const AudioData *data, int length){
    //qDebug() << "Send" << length;
    QByteArray outdata;
    audiocodec->encode(data, length, outdata);
    //qDebug() << "Encoded:" << outdata.size();
    network->sendMessage(new VoIPMessage(outdata, QHostAddress::LocalHost, 7777));
}

void VoIP::decodeMessage(VoIPMessage *message){
    qDebug() << "Recv:" << message->host() << message->port() << message->getData().size();
    QByteArray data = message->getData();
    delete message;
    AudioData *outdata = new AudioData;
    audiocodec->decode(data, outdata);
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
