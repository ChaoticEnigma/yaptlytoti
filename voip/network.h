#ifndef NETWORK_H
#define NETWORK_H

#include <QUdpSocket>
#include "ztypes.h"
#include "voipmessage.h"

using namespace LibChaos;

class VoIP;

class Network : public QObject {
    Q_OBJECT
public:
    explicit Network(VoIP *voip, QObject *parent = 0);
    ~Network();

    void init(zu16 port);

    void sendMessage(VoIPMessage *message);

private slots:
    void readPending();

private:
    VoIP *voip;
    QUdpSocket *socket;
};

#endif // NETWORK_H
