#include "network.h"
#include "voip.h"

Network::Network(VoIP *avoip, QObject *parent) :
        QObject(parent),
        voip(avoip),
        listenport(0),
        socket(nullptr),
        sendseq(0),
        recvseq(0){

}

Network::~Network(){
    if(socket)
        socket->close();
    delete socket;
}

void Network::init(zu16 port){
    listenport = port;
}

void Network::start(){
    socket = new QUdpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPending()));
    socket->bind(QHostAddress::LocalHost, listenport);
}

void Network::sendMessage(VoIPMessage *message){
    // Send datagram
    message->setSeq(++sendseq);
    qDebug() << "Send" << message->getSeq() << message->payload().size();
    ZBinary data = message->getData();
    qint64 len = socket->writeDatagram((char *)data.raw(), data.size(), message->getHost(), message->getPort());
    if(len != data.size()){
        qCritical() << "Failed to write all of datagram";
    }
    delete message;
}

void Network::readPending(){
    while(socket->hasPendingDatagrams()){
        QHostAddress host;
        quint16 port;
        ZBinary datagram(socket->pendingDatagramSize());
        // Receive datagram
        qint64 len = socket->readDatagram((char *)datagram.raw(), datagram.size(), &host, &port);
        if(len != datagram.size()){
            qCritical() << "Failed to read all of datagram";
            return;
        }
        // Process message
        VoIPMessage *message = new VoIPMessage(host, port, datagram);
        recvseq = message->getSeq();
        emit receivedMessage(message);
    }
}
