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

#define NOTIFY_MS 10

class AudioWorker;

struct Client {
    QSettings settings;
    QDialog *settingsDialog;
    QAudioInput *audioInput = nullptr;
    QAudioOutput *audioOutput = nullptr;
    QMutex audioDeviceMutex;
    AudioWorker *audioworker;
};

#endif // MAIN_H
