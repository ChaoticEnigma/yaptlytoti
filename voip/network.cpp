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
    QByteArray datagram = message->getData();
    socket->writeDatagram(datagram, message->host(), message->port());
    delete message;
}

void Network::readPending(){
    while(socket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 sport;
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &sport);

        voip->decodeMessage(new VoIPMessage(datagram, sender, sport));
    }
}
