#ifndef VOIPMESSAGE_H
#define VOIPMESSAGE_H

#include <QByteArray>
#include <QHostAddress>

class VoIPMessage {
public:
    VoIPMessage();
    VoIPMessage(QByteArray data, QHostAddress host, quint16 _port);

    QByteArray getData();

    QHostAddress host() const { return hostAddr; }
    quint16 port() const { return hostPort; }

private:
    QByteArray messageData;
    QHostAddress hostAddr;
    quint16 hostPort;
};

#endif // VOIPMESSAGE_H
