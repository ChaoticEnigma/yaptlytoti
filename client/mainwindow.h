#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "main.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QInputDialog>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(Client *client, QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *e);

private:

public slots:

signals:

public:
    Ui::MainWindow *ui;

private:
    Client *client;
};

#endif // MAINWINDOW_H
