#include "voip.h"

VoIP::VoIP() : QObject(nullptr), audiocodec(nullptr), network(nullptr), netthread(nullptr){

}

VoIP::~VoIP(){
    delete audiocodec;
    netthread->requestInterruption();
    netthread->wait();
    delete netthread;
    delete network;
}

void VoIP::initCodec(Codec::CodecType type){
    delete audiocodec;
    audiocodec = new Codec(type);
    QObject::connect(this, SIGNAL(encodedMessage(VoIPMessage*)), this, SLOT(decodeMessage(VoIPMessage*)));
}

void VoIP::initNetwork(quint16 port){
    return;
    // Setup thread
    if(netthread){
        netthread->requestInterruption();
        netthread->wait();
    } else {
        netthread = new QThread();
    }
    // Setup
    delete network;
    network = new Network(this);
    network->init(port);
    network->moveToThread(netthread);
    QObject::connect(netthread, SIGNAL(started()), network, SLOT(start()));
    QObject::connect(this, SIGNAL(encodedMessage(VoIPMessage*)), network, SLOT(sendMessage(VoIPMessage*)));
    QObject::connect(network, SIGNAL(receivedMessage(VoIPMessage*)), this, SLOT(decodeMessage(VoIPMessage*)));
    netthread->start();
}

void VoIP::connect(QHostAddress addr, zu16 port){

}

void VoIP::inputPCM(const ZArray<zs16> *data){
    ZBinary outdata;
    zu64 len = audiocodec->encode(data, outdata);
    if(len){
        //qDebug() << "Encoded:" << outdata.size();
        VoIPMessage *message = new VoIPMessage(QHostAddress::LocalHost, 7777);
        message->setType(VoIPMessage::TYPE_AUDIO_OPUS);
        message->payload().write(outdata.raw(), outdata.size());
        //emit encodedMessage(message);
        switcher(message);
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

void VoIP::switcher(VoIPMessage *message){
    if(rand.chance(0.1)){
        delete message;
    } else {
        emit encodedMessage(message);
    }
}
