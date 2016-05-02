#ifndef NETWORK_H
#define NETWORK_H

#include <QUdpSocket>
#include "zqueue.h"
#include "voipmessage.h"

using namespace LibChaos;

class VoIP;

class Network : public QObject {
    Q_OBJECT
public:
    explicit Network(VoIP *voip, QObject *parent = 0);
    ~Network();

    void init(zu16 port);

public slots:
    void start();
    void sendMessage(VoIPMessage *message);

signals:
    void receivedMessage(VoIPMessage *message);

private slots:
    void readPending();

private:
    VoIP *voip;
    zu16 listenport;
    QUdpSocket *socket;
    ZQueue<VoIPMessage *> queue;
    zu32 sendseq;
    zu32 recvseq;
};

#endif // NETWORK_H
