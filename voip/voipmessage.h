#ifndef VOIPMESSAGE_H
#define VOIPMESSAGE_H

#include <QHostAddress>
#include "zbinary.h"
using namespace LibChaos;

class VoIPMessage {
public:
    static const zu32 MSGSIG = 0x77888877;
    static const zu8 TYPE_AUDIO_OPUS = 0x02;
public:
    VoIPMessage(QHostAddress host, quint16 port);
    VoIPMessage(QHostAddress host, quint16 port, ZBinary &datagram);

    QHostAddress getHost() const { return hostAddr; }
    void setHost(QHostAddress addr){ hostAddr = addr; }

    quint16 getPort() const { return hostPort; }
    void setPort(quint16 port){ hostPort = port; }

    zu8 getType() const { return messageType; }
    void setType(zu8 type){ messageType = type; }

    ZBinary &payload(){ return messageData; }

    ZBinary getData() const;

private:
    QHostAddress hostAddr;
    quint16 hostPort;
    zu8 messageType;
    ZBinary messageData;
};

#endif // VOIPMESSAGE_H
