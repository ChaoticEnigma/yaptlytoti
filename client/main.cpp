#include "client.h"
#include "mainwindow.h"
#include "settingsdialog.h"
#include "audiosystem.h"
#include <QApplication>
#include <QThread>

int main(int argc, char **argv){
    QCoreApplication::setOrganizationName("Zennix Studios");
    QCoreApplication::setApplicationName("YapTlyToti");

    Client *client = new Client;

    QApplication app(argc, argv);
    Q_INIT_RESOURCE(mainwindow);

    // Create windows
    MainWindow *mainWindow = new MainWindow(client);

    // Create audio I/O controller
    client->audiosystem = new AudioSystem(client);

    // Create codec/network controller
    client->voip = new VoIP();

    // Create settings dialog, triggers audio setup
    client->settingsDialog = new SettingsDialog(client, mainWindow);

    // Show window
    mainWindow->init();
    mainWindow->resize(900, 600);
    mainWindow->show();
    int ret = app.exec();
    mainWindow->hide();

    delete mainWindow;
    delete client;
    return ret;
}
