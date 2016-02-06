#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QLineEdit;
class QBoxLayout;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Client *client);

private slots:
    void newLetter();
    void save();
    void undo();
    void about();
    void insertCustomer(const QString &customer);
    void addParagraph(const QString &paragraph);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

    void resizeEvent(QResizeEvent * event);

private:
    Client *client;

    QTextEdit *chatbox;
    QLineEdit *chatline;
    QWidget *chatframe;
    QBoxLayout *boxlayout;
    QLineEdit *chattitle;

    QDockWidget *serverinfo;
    QDockWidget *clientinfo;
    QDockWidget *chatbar;


    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newLetterAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
};
//! [0]

#endif
