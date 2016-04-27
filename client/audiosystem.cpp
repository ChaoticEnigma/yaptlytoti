#include "audiosystem.h"
#include "voipmessage.h"

#include <QDebug>
#include <QBuffer>

#define INPUT_BUFFER 4000

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
    int samples = client->audioInput->bytesReady() / 2;
    // Limit read to buffer size
    if(samples > INPUT_BUFFER)
        samples = INPUT_BUFFER;
    // Limit read to correct frame size
    samples = client->voip->codec()->checkReadSampleCount(samples);
    // Do nothing if size is 0
    if(!samples)
        return;

    // Read PCM data (signed 16-bit int) as bytes
    qint64 bytes = inputDevice->read((char *)loopbuffer->data(), samples * 2);
    if(bytes < 0){
        qCritical() << "Error reading from IO device";
    } else if(bytes < samples * 2){
        qWarning() << "Read underrun";
    }
    qDebug() << "Rec" << samples;

    // Send to libvoip
    client->voip->sendAudio(loopbuffer, samples);
}

void AudioSystem::playAudio(const AudioData *data){
    qDebug() << "Play" << data->size();

    const qint16 *ptr = data->data();
    qint64 size = data->size() * 2;
    while(true){
        // Wait for device to finish writing
        outputDevice->waitForBytesWritten(-1);

        // Write PCM data (signed 16-bit int) as bytes
        qint64 bytes = outputDevice->write((const char *)ptr, size);
        if(bytes < 0){
            qCritical() << "Error writing to IO device";
        } else if(bytes < size){
            qWarning() << "Write underrun";
            ptr += bytes / 2;
            size -= bytes;
            continue;
        }
        break;
    }
    delete data;
}
