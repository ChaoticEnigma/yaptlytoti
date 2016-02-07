#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QAudioDeviceInfo>

SettingsDialog::SettingsDialog(Client *client, QWidget *parent) : QDialog(parent), client(client), ui(new Ui::SettingsDialog){
    ui->setupUi(this);

    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)){
        ui->inputDeviceComboBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }
    ui->inputDeviceComboBox->setCurrentIndex(0);

    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)){
        ui->outputDeviceComboBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }
    ui->outputDeviceComboBox->setCurrentIndex(0);
}

SettingsDialog::~SettingsDialog(){
    delete ui;
}
