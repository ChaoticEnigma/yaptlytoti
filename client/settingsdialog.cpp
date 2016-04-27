#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QAudioDeviceInfo>

#include "audiosystem.h"

#define DEFAULT_DEVICE "Default"

SettingsDialog::SettingsDialog(Client *aclient, QWidget *parent) : QDialog(parent), client(aclient), ui(new Ui::SettingsDialog){
    ui->setupUi(this);

    // Get available audio input devices
    ui->inputDeviceComboBox->addItem(DEFAULT_DEVICE);
    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)){
        ui->inputDeviceComboBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }
    // Get available audio output devices
    ui->outputDeviceComboBox->addItem(DEFAULT_DEVICE);
    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)){
        ui->outputDeviceComboBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }

    // Select input device from settings
    if(client->settings.value(AUDIO_DEFAULTINPUT, false).toBool()){
        // Select default device
        ui->inputDeviceComboBox->setCurrentIndex(0);
    } else {
        // Select device by name
        QString inputName = client->settings.value(AUDIO_INPUTDEVICE, DEFAULT_DEVICE).toString();
        if(ui->inputDeviceComboBox->findText(inputName) != -1){
            ui->inputDeviceComboBox->setCurrentText(inputName);
        } else {
            // Requested device not in list
            ui->inputDeviceComboBox->setCurrentIndex(0);
        }
    }
    inputDeviceChanged(ui->inputDeviceComboBox->currentIndex());
    connect(ui->inputDeviceComboBox, SIGNAL(activated(int)), SLOT(inputDeviceChanged(int)));

    // Select output device from settings
    if(client->settings.value(AUDIO_DEFAULTOUTPUT, false).toBool()){
        // Select default device
        ui->outputDeviceComboBox->setCurrentIndex(0);
    } else {
        QString outputName = client->settings.value(AUDIO_OUTPUTDEVICE, DEFAULT_DEVICE).toString();
        if(ui->outputDeviceComboBox->findText(outputName) != -1){
            ui->outputDeviceComboBox->setCurrentText(outputName);
        } else {
            // Requested device not in list
            ui->outputDeviceComboBox->setCurrentIndex(0);
        }
    }
    outputDeviceChanged(ui->outputDeviceComboBox->currentIndex());
    connect(ui->outputDeviceComboBox, SIGNAL(activated(int)), SLOT(outputDeviceChanged(int)));

    // Select capture codec
    QString codecstr = client->settings.value(CAPTURE_CODEC, CAPTURE_CODEC_DEFAULT).toString();
    ui->captureCodecComboBox->setCurrentText(codecstr);
    captureCodecChanged(codecstr);
    connect(ui->captureCodecComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(captureCodecChanged(QString)));

    // Select port
    QString portstr = client->settings.value(NETWORK_PORT, NETWORK_PORT_DEFAULT).toString();
    ui->netPortLineEdit->setText(portstr);
    netPortChanged(portstr);
    connect(ui->netPortLineEdit, SIGNAL(textEdited(QString)), this, SLOT(netPortChanged(QString)));
}

SettingsDialog::~SettingsDialog(){
    delete ui;
}

void SettingsDialog::inputDeviceChanged(int idx){
    // Check if the default device is selected
    if(ui->inputDeviceComboBox->currentText() == DEFAULT_DEVICE){
        inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
        client->settings.setValue(AUDIO_DEFAULTINPUT, true);
    } else {
        inputDeviceInfo = ui->inputDeviceComboBox->itemData(idx).value<QAudioDeviceInfo>();
        client->settings.setValue(AUDIO_DEFAULTINPUT, false);
        client->settings.setValue(AUDIO_INPUTDEVICE, inputDeviceInfo.deviceName());
    }
    qInfo() << "Input Device:" << inputDeviceInfo.deviceName() << (client->settings.value(AUDIO_DEFAULTINPUT).toBool() ? "(default)" : "");

    // Format
    QAudioFormat format = inputDeviceInfo.preferredFormat();
    format.setCodec("audio/pcm");
    format.setChannelCount(DEFAULT_CHANNELS);
    format.setSampleRate(DEFAULT_SAMPLE_RATE);
    format.setSampleSize(SAMPLE_SIZE);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    qInfo() << "IFS" << inputDeviceInfo.isFormatSupported(format);

    // Open input device
    delete client->audioInput;
    //client->audioInput = new QAudioInput(inputDeviceInfo, inputDeviceInfo.preferredFormat());
    client->audioInput = new QAudioInput(inputDeviceInfo, format);

    qDebug() << "Input Format:"
            << client->audioInput->format().codec()
            << client->audioInput->format().channelCount() << "channels"
            << client->audioInput->format().sampleRate() << "Hz"
            << client->audioInput->format().sampleSize() << "bit"
            << client->audioInput->format().sampleType()
            << client->audioInput->format().byteOrder();

    client->audiosystem->initInput();
}

void SettingsDialog::outputDeviceChanged(int idx){
    // Check if the default device is selected
    if(ui->outputDeviceComboBox->currentText() == DEFAULT_DEVICE){
        outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
        client->settings.setValue(AUDIO_DEFAULTOUTPUT, true);
    } else {
        outputDeviceInfo = ui->outputDeviceComboBox->itemData(idx).value<QAudioDeviceInfo>();
        client->settings.setValue(AUDIO_DEFAULTOUTPUT, false);
        client->settings.setValue(AUDIO_OUTPUTDEVICE, outputDeviceInfo.deviceName());
    }
    qDebug() << "Output Device:" << outputDeviceInfo.deviceName() << (client->settings.value(AUDIO_DEFAULTOUTPUT).toBool() ? "(default)" : "");

    // Format
    QAudioFormat format = outputDeviceInfo.preferredFormat();
    format.setCodec("audio/pcm");
    format.setChannelCount(DEFAULT_CHANNELS);
    format.setSampleRate(DEFAULT_SAMPLE_RATE);
    format.setSampleSize(SAMPLE_SIZE);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    qInfo() << "OFS" << outputDeviceInfo.isFormatSupported(format);

    // Open output device
    delete client->audioOutput;
    //client->audioOutput = new QAudioOutput(outputDeviceInfo, outputDeviceInfo.preferredFormat());
    client->audioOutput = new QAudioOutput(outputDeviceInfo, format);

    qDebug() << "Output Format:"
            << client->audioOutput->format().codec()
            << client->audioOutput->format().channelCount() << "channels"
            << client->audioOutput->format().sampleRate() << "Hz"
            << client->audioOutput->format().sampleSize() << "bit"
            << client->audioOutput->format().sampleType()
            << client->audioOutput->format().byteOrder();

    client->audiosystem->initOutput();
}

void SettingsDialog::captureCodecChanged(QString text){
    qDebug() << "Capture Codec:" << text;
    client->settings.setValue(CAPTURE_CODEC, text);

    QMap<QString, Codec::CodecType> cmap = {
        { "Opus", Codec::CODEC_OPUS },
    };

    client->voip->initCodec(cmap[text]);
}

void SettingsDialog::netPortChanged(QString text){
    qDebug() << "Net Port:" << text;
    bool ok = false;
    int port = text.toUShort(&ok);
    if(ok){
        client->settings.setValue(NETWORK_PORT, port);
        client->voip->initNetwork(port);
    } else {
        ui->netPortLineEdit->setText(client->settings.value(NETWORK_PORT, NETWORK_PORT_DEFAULT).toString());
    }
}
