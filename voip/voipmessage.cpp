#include "voipmessage.h"

VoIPMessage::VoIPMessage()
{

}

VoIPMessage::VoIPMessage(QByteArray data, QHostAddress host, quint16 port) : messageData(data), hostAddr(host), hostPort(port){

}

QByteArray VoIPMessage::getData(){
    return messageData;
}
