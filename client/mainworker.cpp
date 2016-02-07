#include "mainworker.h"

#include <QDebug>
#include <QBuffer>

MainWorker::MainWorker(Client *client, QObject *parent) : QObject(parent), client(client){

}

void MainWorker::run(){
    qDebug() << "Starting Audio Thread";
    QBuffer *buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    qInfo() << "Input Devive State:" << client->audioInput->state();
    client->audioInput->start(buffer);
    qInfo() << "Input Devive State:" << client->audioInput->state();

    qInfo() << "Output Devive State:" << client->audioOutput->state();
    client->audioOutput->start(buffer);
    qInfo() << "Output Devive State:" << client->audioOutput->state();
}
