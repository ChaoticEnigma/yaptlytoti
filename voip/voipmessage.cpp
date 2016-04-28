#include "voipmessage.h"

VoIPMessage::VoIPMessage(QHostAddress host, quint16 port) :
    hostAddr(host),
    hostPort(port),
    sequence(0),
    messageType(0){

}

VoIPMessage::VoIPMessage(QHostAddress host, quint16 port, ZBinary &datagram) : VoIPMessage(host, port){
    datagram.rewind();
    zu32 sig = datagram.readbeu32();
    if(sig != MSGSIG){
        qCritical() << "Signature FAIL";
        return;
    }
    messageType = datagram.readu8();
    sequence = datagram.readbeu32();
    zu32 size = datagram.readbeu32();
    messageData.write(datagram.raw() + datagram.tell(), size);
}

ZBinary VoIPMessage::getData() const {
    ZBinary data;
    data.writebeu32(MSGSIG);
    data.writeu8(messageType);
    data.writebeu32(sequence);
    data.writebeu32(messageData.size());
    data.write(messageData.raw(), messageData.size());
    return data;
}
