#include "audioworker.h"

#include <QDebug>
#include <QBuffer>

#define INPUT_BUFFER 4096

AudioWorker::AudioWorker(Client *client, QObject *parent) : QObject(parent), client(client), input(nullptr), output(nullptr), loopbuffer(nullptr){

}

AudioWorker::~AudioWorker(){
    delete loopbuffer;
}

void AudioWorker::run(){
    qDebug() << "Starting Audio Thread";

    delete loopbuffer;
    loopbuffer = new QByteArray(INPUT_BUFFER, 0);
    //inputdata->resize(1024);
    //inputbuffer = new QBuffer(inputdata);
    //inputbuffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    //QByteArray *outputdata = new QByteArray;
    //outputdata->resize(1024);
    //outputbuffer = new QBuffer(outputdata);
    //outputbuffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    int error;
    // Opus encoder
    encode = opus_encoder_create(client->audioInput->format().sampleRate(), client->audioInput->format().channelCount(), OPUS_APPLICATION_VOIP, &error);
    if(error < 0){
        qCritical() << "Faied to create opus encoder: " << opus_strerror(error);
        return;
    }
    error = opus_encoder_ctl(encode, OPUS_SET_BITRATE(OPUS_AUTO));
    if(error < 0){
        qCritical() << "Faied to set opus bitrate: " << opus_strerror(error);
        return;
    }

    // Opus decoder
    decode = opus_decoder_create(client->audioInput->format().sampleRate(), client->audioInput->format().channelCount(), &error);
    if(error < 0){
        qCritical() << "Faied to create opus decoder: " << opus_strerror(error);
        return;
    }

    client->audioDeviceMutex.lock();

    qInfo() << "Input Devive State:" << client->audioInput->state();
    input = client->audioInput->start();
    connect(input, SIGNAL(readyRead()), this, SLOT(inputRead()));
    //client->audioInput->setNotifyInterval(NOTIFY_MS);
    //connect(client->audioInput, SIGNAL(notify()), this, SLOT(inputNotified()));
    qInfo() << "Input Devive State:" << client->audioInput->state();

    qInfo() << "Output Devive State:" << client->audioOutput->state();
    //client->audioOutput->start(input);
    output = client->audioOutput->start();
    //client->audioOutput->setNotifyInterval(NOTIFY_MS);
    //connect(client->audioOutput, SIGNAL(notify()), this, SLOT(outputNotified()));
    qInfo() << "Output Devive State:" << client->audioOutput->state();

    client->audioDeviceMutex.unlock();
}

void AudioWorker::inputRead(){
    client->audioDeviceMutex.lock();
    qint64 len = client->audioInput->bytesReady();
    // Limit read to buffer size
    if(len > INPUT_BUFFER) len = INPUT_BUFFER;
    qInfo() << "Read " << len;
    // Read PCM data
    input->read(loopbuffer->data(), len);

    QByteArray odata(INPUT_BUFFER, 0);
    opus_int32 olen = opus_encode(encode, (opus_int16 *)loopbuffer->data(), 0, (unsigned char *)odata.data(), INPUT_BUFFER);

    output->write(loopbuffer->data());
    client->audioDeviceMutex.unlock();
}

void AudioWorker::inputNotified(){
    //qDebug() << "Input " << inputbuffer->bytesAvailable() << client->audioInput->state();
}

void AudioWorker::outputNotified(){
    //qDebug() << "Output" << outputbuffer->bytesAvailable() << client->audioOutput->state();
}
