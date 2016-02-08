#include "audioworker.h"

#include <QDebug>
#include <QBuffer>

AudioWorker::AudioWorker(Client *client, QObject *parent) : QObject(parent), client(client){

}

void AudioWorker::run(){
    qDebug() << "Starting Audio Thread";

    inputdata = new QByteArray;
    inputdata->resize(1024);
    inputbuffer = new QBuffer(inputdata);
    inputbuffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    outputdata = new QByteArray;
    outputdata->resize(1024);
    outputbuffer = new QBuffer(outputdata);
    outputbuffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    client->audioDeviceMutex.lock();

    qInfo() << "Input Devive State:" << client->audioInput->state();
    client->audioInput->start(inputbuffer);
    client->audioInput->setNotifyInterval(10);
    connect(client->audioInput, SIGNAL(notify()), this, SLOT(inputNotified()));
    qInfo() << "Input Devive State:" << client->audioInput->state();

    qInfo() << "Output Devive State:" << client->audioOutput->state();
    client->audioOutput->start(outputbuffer);
    client->audioOutput->setNotifyInterval(10);
    connect(client->audioOutput, SIGNAL(notify()), this, SLOT(outputNotified()));
    qInfo() << "Output Devive State:" << client->audioOutput->state();

    client->audioDeviceMutex.unlock();
}

void AudioWorker::inputNotified(){
    qDebug() << "Input " << inputbuffer->bytesAvailable() << client->audioInput->state();
}

void AudioWorker::outputNotified(){
    qDebug() << "Output" << outputbuffer->bytesAvailable() << client->audioOutput->state();
}
