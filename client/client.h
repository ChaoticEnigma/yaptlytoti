#ifndef MAIN_H
#define MAIN_H

#include <QSettings>
#include <QDialog>
#include <QAudioInput>
#include <QAudioOutput>
//#include <QMutex>
#include "voip.h"

// Settings Keys
#define AUDIO_INPUTDEVICE       "audio/inputDevice"
#define AUDIO_DEFAULTINPUT      "audio/defaultInput"
#define AUDIO_OUTPUTDEVICE      "audio/outputDevice"
#define AUDIO_DEFAULTOUTPUT     "audio/defaultOutput"
#define CAPTURE_CODEC           "codec/captureCodec"
#define CAPTURE_CODEC_DEFAULT   "Opus"
#define NETWORK_PORT            "network/port"
#define NETWORK_PORT_DEFAULT    7777

#define NOTIFY_MS 10

class AudioSystem;

struct Client {
    QSettings settings;
    QDialog *settingsDialog;
    QAudioInput *audioInput = nullptr;
    QAudioOutput *audioOutput = nullptr;
    //QMutex audioDeviceMutex;
    AudioSystem *audiosystem;
    VoIP *voip;
};

#endif // MAIN_H
