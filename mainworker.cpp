#include "mainworker.h"

MainWorker::MainWorker(Client *client, QObject *parent) : QObject(parent), client(client){}

void MainWorker::run(){
    //LOG("Worker Init");
}
