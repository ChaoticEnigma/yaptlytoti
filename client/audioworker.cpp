#include "audioworker.h"

#include <QDebug>
#include <QBuffer>

AudioWorker::AudioWorker(Client *client, QObject *parent) : QObject(parent), client(client){

}

void AudioWorker::run(){
    qDebug() << "Starting Audio Thread";
    QBuffer *buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    client->audioDeviceMutex.lock();

    qInfo() << "Input Devive State:" << client->audioInput->state();
    client->audioInput->start(buffer);
    client->audioInput->setNotifyInterval(10);
    connect(client->audioInput, SIGNAL(notify()), this, SLOT(inputNotified()));
    qInfo() << "Input Devive State:" << client->audioInput->state();

    qInfo() << "Output Devive State:" << client->audioOutput->state();
    client->audioOutput->start(buffer);
    client->audioOutput->setNotifyInterval(10);
    connect(client->audioOutput, SIGNAL(notify()), this, SLOT(outputNotified()));
    qInfo() << "Output Devive State:" << client->audioOutput->state();

    client->audioDeviceMutex.unlock();
}

void AudioWorker::inputNotified(){

}

void AudioWorker::outputNotified(){

}
