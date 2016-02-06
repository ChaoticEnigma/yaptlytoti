#include "main.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTimer>
#include <QThread>

int main(int argc, char **argv){
    QApplication app(argc, argv);
    app.setOrganizationName("Zennix Studios");
    app.setApplicationName("YAPTLYTOTI");

    Client *client = new Client;
    MainWindow *window = new MainWindow(client);
    QThread *thread = new QThread;

    thread->start();
    window->show();
    int ret = app.exec();
    thread->quit();

    delete thread;
    delete window;
    delete client;
    return ret;
}
