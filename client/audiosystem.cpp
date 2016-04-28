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
        loopbuffer(nullptr),
        receiver(nullptr){
    loopbuffer = new ZArray<zs16>(INPUT_BUFFER);
}

AudioSystem::~AudioSystem(){
    delete loopbuffer;
}

void AudioSystem::initInput(){
    qDebug() << "Input Device State:" << client->audioInput->state();
    receiver = new InputReceiver(client->audioInput->format(), client->voip, this);
    connect(receiver, SIGNAL(audioReady(const ZArray<zs16>*)), client->voip, SLOT(inputPCM(const ZArray<zs16>*)));
    receiver->start();
    client->audioInput->start(receiver);

    //client->audioInput->setNotifyInterval(10);
    //qDebug() << "NI" << client->audioInput->notifyInterval();
    //connect(client->audioInput, SIGNAL(notify()), this, SLOT(inputRead()));

    //inputDevice = client->audioInput->start();
    //connect(inputDevice, SIGNAL(readyRead()), this, SLOT(inputRead()));
    qDebug() << "Input Device State:" << client->audioInput->state();
}

void AudioSystem::initOutput(){
    qDebug() << "Output Device State:" << client->audioOutput->state();
    outputDevice = client->audioOutput->start();
    connect(client->voip, SIGNAL(decodedAudio(const ZArray<zs16> *)), this, SLOT(playAudio(const ZArray<zs16> *)));
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
    qint64 bytes = inputDevice->read((char *)loopbuffer->raw(), samples * 2);
    if(bytes < 0){
        qCritical() << "Error reading from IO device";
    } else if(bytes < samples * 2){
        qWarning() << "Read underrun";
    }
    //qDebug() << "Rec" << samples;

    // Send to libvoip
    ZArray<zs16> *sdata = new ZArray<zs16>(*loopbuffer);
    sdata->resize(samples);
    client->voip->inputPCM(sdata);
}

void AudioSystem::playAudio(const ZArray<zs16> *data){
    //qDebug() << "Play" << data->size();

    const qint16 *ptr = data->raw();
    qint64 size = data->size() * 2;
    // Wait for device to finish writing
    //outputDevice->waitForBytesWritten(-1);

    // Write PCM data (signed 16-bit int) as bytes
    qint64 bytes = outputDevice->write((const char *)ptr, size);
    if(bytes < 0){
        qCritical() << "Error writing to IO device";
    } else if(bytes < size){
        qWarning() << "Write underrun";
        //ptr += bytes / 2;
        //size -= bytes;
        //continue;
    }
    delete data;
}
