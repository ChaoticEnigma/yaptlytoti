#include "main.h"
#include "mainwindow.h"
#include "mainworker.h"
#include <QApplication>
#include <QThread>

int main(int argc, char **argv){
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(mainwindow);
    app.setOrganizationName("Zennix Studios");
    app.setApplicationName("YapTlyToti");

    Client *client = new Client;
    MainWindow *window = new MainWindow(client);
    MainWorker *worker = new MainWorker(client);
    QThread *thread = new QThread;
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(run()));

    thread->start();
    window->resize(900, 600);
    window->show();
    int ret = app.exec();
    window->hide();
    thread->quit();

    delete thread;
    delete worker;
    delete window;
    delete client;
    return ret;
}
