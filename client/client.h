#ifndef MAIN_H
#define MAIN_H

#include <QSettings>
#include <QDialog>
#include <QAudioInput>
#include <QAudioOutput>
#include <QMutex>

// Settings Keys
#define AUDIO_INPUTDEVICE   "audio/inputDevice"
#define AUDIO_DEFAULTINPUT  "audio/defaultInput"
#define AUDIO_OUTPUTDEVICE  "audio/outputDevice"
#define AUDIO_DEFAULTOUTPUT "audio/defaultOutput"

struct Client {
    QSettings settings;
    QDialog *settingsDialog;
    QAudioInput *audioInput = nullptr;
    QAudioOutput *audioOutput = nullptr;
    QMutex audioDeviceMutex;
};

#endif // MAIN_H
