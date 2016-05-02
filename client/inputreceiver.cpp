#include "inputreceiver.h"
#include "codec.h"

InputReceiver::InputReceiver(const QAudioFormat &aformat, VoIP *avoip, QObject *parent) : QIODevice(parent), format(aformat), voip(avoip){

}

InputReceiver::~InputReceiver(){

}

void InputReceiver::start(){
    open(QIODevice::WriteOnly);
}

void InputReceiver::stop(){
    close();
}

qint64 InputReceiver::readData(char *data, qint64 maxlen){
    return 0;
}

qint64 InputReceiver::writeData(const char *data, qint64 len){
    int channelBytes = format.sampleSize() / 8;
    int sampleBytes = format.channelCount() * channelBytes;
    int numSamples = len / sampleBytes;
    const qint16 *ptr = reinterpret_cast<const qint16 *>(data);

    //qDebug() << "Rec" << numSamples;

    ZArray<zs16> *samples = new ZArray<zs16>(ptr, numSamples);
    emit audioReady(samples);

    return len;
}
