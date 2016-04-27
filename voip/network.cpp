#include "network.h"
#include "voip.h"

Network::Network(VoIP *avoip, QObject *parent) : QObject(parent), voip(avoip), socket(nullptr){
    socket = new QUdpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPending()));
}

Network::~Network(){
    delete socket;
}

void Network::init(zu16 port){
    socket->bind(QHostAddress::LocalHost, port);
}

void Network::sendMessage(VoIPMessage *message){
    // Send datagram
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
            //return;
        }
        // Process message
        voip->decodeMessage(new VoIPMessage(host, port, datagram));
    }
}
