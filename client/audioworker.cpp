#include "audioworker.h"

#include <QDebug>
#include <QBuffer>

#include "opus/opus.h"

#define INPUT_BUFFER 4096

AudioWorker::AudioWorker(Client *client, QObject *parent) : QObject(parent), client(client){

}

void AudioWorker::run(){
    qDebug() << "Starting Audio Thread";

    inputbuffer = new QByteArray(INPUT_BUFFER, 0);
    //inputdata->resize(1024);
    //inputbuffer = new QBuffer(inputdata);
    //inputbuffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    //QByteArray *outputdata = new QByteArray;
    //outputdata->resize(1024);
    //outputbuffer = new QBuffer(outputdata);
    //outputbuffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    client->audioDeviceMutex.lock();

    qInfo() << "Input Devive State:" << client->audioInput->state();
    input = client->audioInput->start();
    //client->audioInput->setNotifyInterval(NOTIFY_MS);
    //connect(client->audioInput, SIGNAL(notify()), this, SLOT(inputNotified()));
    connect(input, SIGNAL(readyRead()), this, SLOT(inputRead()));
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
    qint64 len = client->audioInput->bytesReady();
    if(len > INPUT_BUFFER) len = INPUT_BUFFER;
    qInfo() << "Read " << len;
    input->read(inputbuffer->data(), len);
    output->write(inputbuffer->data());
}

void AudioWorker::inputNotified(){
    //qDebug() << "Input " << inputbuffer->bytesAvailable() << client->audioInput->state();
}

void AudioWorker::outputNotified(){
    //qDebug() << "Output" << outputbuffer->bytesAvailable() << client->audioOutput->state();
}
