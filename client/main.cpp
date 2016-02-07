#include "client.h"
#include "mainwindow.h"
#include "settingsdialog.h"
#include "mainworker.h"
#include <QApplication>
#include <QThread>

int main(int argc, char **argv){
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(mainwindow);
    app.setOrganizationName("Zennix Studios");
    app.setApplicationName("YapTlyToti");

    Client *client = new Client;
    MainWindow *mainWindow = new MainWindow(client);
    client->settingsDialog = new SettingsDialog(client, mainWindow);
    mainWindow->init();

    MainWorker *worker = new MainWorker(client);
    QThread *thread = new QThread;
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(run()));

    thread->start();
    mainWindow->resize(900, 600);
    mainWindow->show();
    int ret = app.exec();
    mainWindow->hide();
    thread->quit();

    delete thread;
    delete worker;

    delete mainWindow;
    delete client;
    return ret;
}
