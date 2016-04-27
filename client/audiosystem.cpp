#include "audiosystem.h"
#include "voipmessage.h"

#include <QDebug>
#include <QBuffer>

#define INPUT_BUFFER 40000

AudioSystem::AudioSystem(Client *aclient, QObject *parent) :
        QObject(parent),
        client(aclient),
        inputDevice(nullptr),
        outputDevice(nullptr),
        loopbuffer(nullptr){
    loopbuffer = new AudioData(INPUT_BUFFER);
}

AudioSystem::~AudioSystem(){
    delete loopbuffer;
}

void AudioSystem::initInput(){
    qDebug() << "Input Device State:" << client->audioInput->state();
    inputDevice = client->audioInput->start();
    //client->audioInput->setNotifyInterval(10);
    //qDebug() << "NI" << client->audioInput->notifyInterval();
    //connect(client->audioInput, SIGNAL(notify()), this, SLOT(inputRead()));
    connect(inputDevice, SIGNAL(readyRead()), this, SLOT(inputRead()));
    qDebug() << "Input Device State:" << client->audioInput->state();
}

void AudioSystem::initOutput(){
    qDebug() << "Output Device State:" << client->audioOutput->state();
    outputDevice = client->audioOutput->start();
    connect(client->voip, SIGNAL(decodedAudio(const AudioData*)), this, SLOT(playAudio(const AudioData*)));
    qDebug() << "Output Device State:" << client->audioOutput->state();
}

void AudioSystem::inputRead(){
    int len = client->audioInput->bytesReady();
    // Limit read to buffer size
    if(len > INPUT_BUFFER)
        len = INPUT_BUFFER;
    len = client->voip->codec()->checkReadAvailBytes(len / 2);
    // Do nothing if len is 0
    if(!len)
        return;

    // Read PCM data
    inputDevice->read((char *)loopbuffer->data(), len * 2);
    //qDebug() << "Rec" << len;

    // Send to libvoip
    client->voip->sendAudio(loopbuffer, len);
}

void AudioSystem::playAudio(const AudioData *data){
    //qDebug() << "Play" << data->size();
    // Write PCM data
    while(outputDevice->bytesToWrite());
    outputDevice->write((const char *)data->data(), data->size() * 2);
    delete data;
}
