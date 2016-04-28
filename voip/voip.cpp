#include "voip.h"

VoIP::VoIP() : audiocodec(nullptr), network(nullptr){

}

VoIP::~VoIP(){
    delete audiocodec;
    delete network;
}

void VoIP::inputPCM(const ZArray<zs16> *data){
    //qDebug() << "Send" << length;
    ZBinary outdata;
    zu64 len = audiocodec->encode(data, outdata);
    if(len){
        //qDebug() << "Encoded:" << outdata.size();
        VoIPMessage *message = new VoIPMessage(QHostAddress::LocalHost, 7777);
        message->setType(VoIPMessage::TYPE_AUDIO_OPUS);
        message->payload().write(outdata.raw(), outdata.size());
        network->sendMessage(message);
    }
}

void VoIP::decodeMessage(VoIPMessage *message){
    qDebug() << "Recv" << message->getSeq() << message->getHost() << message->getPort() << message->payload().size();
    ZArray<zs16> *outdata = new ZArray<zs16>;
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
