#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QAudioDeviceInfo>

#define DEFAULT_DEVICE "Default"

SettingsDialog::SettingsDialog(Client *client, QWidget *parent) : QDialog(parent), client(client), ui(new Ui::SettingsDialog){
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
    delete client->audioInput;
    client->audioInput = new QAudioInput(inputDeviceInfo, inputDeviceInfo.preferredFormat());
    qInfo() << "Input Format:"
            << client->audioInput->format().codec()
            << client->audioInput->format().sampleType()
            << client->audioInput->format().sampleSize() << "bit"
            << client->audioInput->format().sampleRate() << "Hz";
}

void SettingsDialog::outputDeviceChanged(int idx){
    // Check if the default device is selected
    if(ui->outputDeviceComboBox->currentText() == DEFAULT_DEVICE){
        outputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
        client->settings.setValue(AUDIO_DEFAULTOUTPUT, true);
    } else {
        outputDeviceInfo = ui->outputDeviceComboBox->itemData(idx).value<QAudioDeviceInfo>();
        client->settings.setValue(AUDIO_DEFAULTOUTPUT, false);
        client->settings.setValue(AUDIO_OUTPUTDEVICE, outputDeviceInfo.deviceName());
    }
    qInfo() << "Output Device:" << outputDeviceInfo.deviceName() << (client->settings.value(AUDIO_DEFAULTOUTPUT).toBool() ? "(default)" : "");
    delete client->audioOutput;
    client->audioOutput = new QAudioOutput(outputDeviceInfo, outputDeviceInfo.preferredFormat());
    qInfo() << "Output Format:"
            << client->audioOutput->format().codec()
            << client->audioOutput->format().sampleType()
            << client->audioOutput->format().sampleSize() << "bit"
            << client->audioOutput->format().sampleRate() << "Hz";
}
