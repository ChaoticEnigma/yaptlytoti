#include "mainwindow.h"

#include <Qt>
#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QHeaderView>

MainWindow::MainWindow(Client *client, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), client(client){
    ui->setupUi(this);
    //LOG("Window Init");
}
MainWindow::~MainWindow(){
    delete ui;
    //LOG("Window Destroy");
}

void MainWindow::resizeEvent(QResizeEvent *e){
    //LOG("Resized " << e->size().width() << " x " << e->size().height());
}
