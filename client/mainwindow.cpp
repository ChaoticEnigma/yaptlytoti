#include "mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(Client *client, QWidget *parent) : QMainWindow(parent), client(client){

    chatframe = new QWidget();
    chatboxlayout = new QBoxLayout(QBoxLayout::TopToBottom, chatframe);
    chatboxlayout->setContentsMargins(0,0,0,0);
    chatboxlayout->setSpacing(0);

    chattitle = new QLineEdit(chatframe);
    chattitle->setStyleSheet("border: 1px solid rgb(200,200,200); border-radius: 0px;");
    chattitle->setReadOnly(true);
    chattitle->setAlignment(Qt::AlignCenter);
    chatbox = new QTextEdit(chatframe);
    chatbox->setReadOnly(true);

    chatboxlayout->addWidget(chattitle);
    chatboxlayout->addWidget(chatbox);
    chatframe->setLayout(chatboxlayout);
    setCentralWidget(chatframe);

    setStyleSheet("MainWindow::separator { background: rgb(200, 200, 200); width: 2px; height: 2px; }");

    setWindowTitle(tr("Yet Another Program that Lets You Talk on the Internet (Yap)"));
    //setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow(){

}

void MainWindow::init(){
    createActions();
    createMenus();
    //createToolBars();
    createStatusBar();
    createDockWindows();

    //setUnifiedTitleAndToolBarOnMac(true);

    newLetter();
}


void MainWindow::newLetter(){
    chatbox->clear();


    QTextCursor cursor(chatbox->textCursor());
    cursor.movePosition(QTextCursor::Start);
    QTextFrame *topFrame = cursor.currentFrame();
    QTextFrameFormat topFrameFormat = topFrame->frameFormat();
    topFrameFormat.setPadding(16);
    topFrame->setFrameFormat(topFrameFormat);

    QTextCharFormat textFormat;
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    QTextCharFormat italicFormat;
    italicFormat.setFontItalic(true);

    /*QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setCellPadding(16);
    tableFormat.setAlignment(Qt::AlignRight);
    cursor.insertTable(1, 1, tableFormat);
    cursor.insertText("The Firm", boldFormat);
    cursor.insertBlock();
    cursor.insertText("321 City Street", textFormat);
    cursor.insertBlock();
    cursor.insertText("Industry Park");
    cursor.insertBlock();
    cursor.insertText("Some Country"); */
    cursor.setPosition(topFrame->lastPosition());
    cursor.insertText("Railguy: ", boldFormat);
    cursor.insertText("lol nub", textFormat);
    cursor.insertBlock();
    cursor.insertText("Chaos: ", boldFormat);
    cursor.insertText("nou", textFormat);
    cursor.insertBlock();

    QFont font = chattitle->font();
    font.setBold(true);
    chattitle->setFont(font);
    chattitle->setText("#General");
}

void MainWindow::save(){
    QString fileName = QFileDialog::getSaveFileName(this,
                        tr("Choose a file name"), ".",
                        tr("HTML (*.html *.htm)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Dock Widgets"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << chatbox->toHtml();
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
}

void MainWindow::undo(){
    QTextDocument *document = chatbox->document();
    document->undo();
}

void MainWindow::insertCustomer(const QString &customer){
    if (customer.isEmpty())
        return;
    QStringList customerList = customer.split(", ");
    QTextDocument *document = chatbox->document();
    QTextCursor cursor = document->find("NAME");
    if (!cursor.isNull()) {
        cursor.beginEditBlock();
        cursor.insertText(customerList.at(0));
        QTextCursor oldcursor = cursor;
        cursor = document->find("ADDRESS");
        if (!cursor.isNull()) {
            for (int i = 1; i < customerList.size(); ++i) {
                cursor.insertBlock();
                cursor.insertText(customerList.at(i));
            }
            cursor.endEditBlock();
        }
        else
            oldcursor.endEditBlock();
    }
}

void MainWindow::addParagraph(const QString &paragraph){
    if (paragraph.isEmpty())
        return;
    QTextDocument *document = chatbox->document();
    QTextCursor cursor = document->find(tr("Yours sincerely,"));
    if (cursor.isNull())
        return;
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, 2);
    cursor.insertBlock();
    cursor.insertText(paragraph);
    cursor.insertBlock();
    cursor.endEditBlock();

}

void MainWindow::about(){
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
               "Nam porta imperdiet metus, finibus pharetra justo. "
               "Integer aliquam lorem a tincidunt aliquet. Proin "
               "eleifend maximus tincidunt."));
}

void MainWindow::createActions(){
    newLetterAct = new QAction(QIcon(":/images/new.png"), tr("&New Letter"),
                               this);
    newLetterAct->setShortcuts(QKeySequence::New);
    newLetterAct->setStatusTip(tr("Create a new form letter"));
    connect(newLetterAct, SIGNAL(triggered()), this, SLOT(newLetter()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current form letter"));
//    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    settingsAct = new QAction(tr("&Settings..."), this);
    settingsAct->setStatusTip(tr("Open the settings dialog"));
    connect(settingsAct, SIGNAL(triggered()), client->settingsDialog, SLOT(show()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(newLetterAct);
    //fileMenu->addAction(saveAct);
    //fileMenu->addAction(printAct);
    //fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    //editMenu->addAction(undoAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(settingsAct);

    //menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars(){
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
}

void MainWindow::createStatusBar(){
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows(){

    float h = (float)this->height();

    this->setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    QDockWidget* dock = new QDockWidget(tr("Text Channels"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widg = new QWidget(dock); //Placeholder
    dock->setWidget(widg);

    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());


    dock = new QDockWidget(tr("Voice Channels"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    widg = new QWidget(dock); //Placeholder
    dock->setWidget(widg);

    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());


    clientinfo = new QDockWidget("User Info", this); //Client info frame
    clientinfo->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    clientinfo2 = new QWidget(clientinfo);
    clientinfo2->setFixedHeight((int)(.25*h));   //Set height of clientinfo proportional to mainwindow
    clientinfo2->setMinimumWidth(2*clientinfo2->height());

    clientinfo->setWidget(clientinfo2);

    clientboxlayout = new QBoxLayout(QBoxLayout::TopToBottom, clientinfo2); //Outer client box layout
    clientboxlayout->setContentsMargins(0,0,0,0);
    clientboxlayout->setSpacing(0);


    clientbasicinfo = new QWidget(); //Upper Client Info Container
    clientadvancedinfo = new QWidget(); //Lower Client Info Container
    clientbuttons = new QWidget(); // Client Button container

    clientboxlayout2 = new QBoxLayout(QBoxLayout::LeftToRight, clientbasicinfo); //Inner client box layout
    clientboxlayout2->setContentsMargins(0,0,0,0);
    clientboxlayout2->setSpacing(0);
    clientboxlayout3 = new QBoxLayout(QBoxLayout::LeftToRight, clientadvancedinfo);
    clientboxlayout3->setContentsMargins(0,0,0,0);
    clientboxlayout3->setSpacing(0);
    clientboxlayout4 = new QBoxLayout(QBoxLayout::TopToBottom, clientbuttons);
    clientboxlayout4->setContentsMargins(10,0,0,0);
    clientboxlayout4->setSpacing(0);

    userimage = new QPixmap(tr("../yaptlytoti/client/images/user.jpg"), "JPG");
    userimagelabel = new QLabel(); //Create the user image box
    userimagelabel->setFixedHeight((int)(.5*clientinfo2->height())); //Set height of image proportional to clientinfo
    userimagelabel->setFixedWidth((int)(.5*clientinfo2->height()));
    userimagelabel->setPixmap(userimage->scaled(userimagelabel->width(), userimagelabel->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    userimagelabel->setStyleSheet("background-color: lightgrey;");


    username = new QLineEdit(); //Create Username line
    username->setAlignment(Qt::AlignCenter);
    username->setFixedHeight((int)(.5*clientinfo2->height()));
    username->setStyleSheet("border: 1px solid rgb(200,200,200); border-radius: 0px;");
    QFont font(username->font());
    font.setBold(true);
    font.setPointSize(20);
    username->setTextMargins(6,0,6,0);
    username->setFont(font);
    username->setText("Railguy");
    username->setReadOnly(true);
    //username->setStyleSheet("background-color: red;");

    clientboxlayout2->addWidget(userimagelabel); //Add user image
    clientboxlayout2->addWidget(username); //Add user name

    clientbasicinfo->setLayout(clientboxlayout2); //Add inner box layout to upper client container


    // NOTE: I'm aware this can be done much faster, and I intend to do so, but it is not a high priority
    clientadvinfo = new QTextEdit(); //Lower Client Info Text
    clientadvinfo->setLineWrapMode(QTextEdit::NoWrap);
    clientadvinfo->setReadOnly(true);
    clientadvinfo->clear();
    clientadvinfo->setFontWeight(QFont::Bold);
    clientadvinfo->insertPlainText("IP: ");
    clientadvinfo->setFontWeight(QFont::Normal);
    clientadvinfo->insertPlainText("192.137.42.69" "\n");
    clientadvinfo->setFontWeight(QFont::Bold);
    clientadvinfo->insertPlainText("Time connected: ");
    clientadvinfo->setFontWeight(QFont::Normal);
    clientadvinfo->insertPlainText("21" " days " "37" " minutes " "11" " seconds\n");
    clientadvinfo->setFontWeight(QFont::Bold);
    clientadvinfo->insertPlainText("Channel Roles: ");
    clientadvinfo->setFontWeight(QFont::Normal);
    clientadvinfo->insertPlainText("Normal" "\n");
    clientadvinfo->setFontWeight(QFont::Bold);
    clientadvinfo->insertPlainText("Server Roles: ");
    clientadvinfo->setFontWeight(QFont::Normal);
    clientadvinfo->insertPlainText("Zennix" ", " "Developer" "\n");
    clientadvinfo->moveCursor(QTextCursor::Start);

    settingsimage = new QPixmap(tr("../yaptlytoti/client/images/settings.png"), "PNG");
    settingsbutton = new QLabel();
    //settingsbutton->setStyleSheet("border-bottom-style: solid; border-bottom-width: 2px; border-bottom-color: #C8C8C8;");
    settingsbutton->setFixedHeight((int)(.16*clientinfo2->height()));
    settingsbutton->setFixedWidth((int)(.16*clientinfo2->height()));
    settingsbutton->setPixmap(settingsimage->scaled(settingsbutton->width(), settingsbutton->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    micmuteimage = new QPixmap(tr("../yaptlytoti/client/images/micmute.png"), "PNG");
    micmutebutton = new QLabel();
    micmutebutton->setFixedHeight((int)(.16*clientinfo2->height()));
    micmutebutton->setFixedWidth((int)(.16*clientinfo2->height()));
    micmutebutton->setPixmap(micmuteimage->scaled(micmutebutton->width(), micmutebutton->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    soundmuteimage = new QPixmap(tr("../yaptlytoti/client/images/soundmute.png"), "PNG");
    soundmutebutton = new QLabel();
    soundmutebutton->setFixedHeight((int)(.16*clientinfo2->height()));
    soundmutebutton->setFixedWidth((int)(.16*clientinfo2->height()));
    soundmutebutton->setPixmap(soundmuteimage->scaled(soundmutebutton->width(), soundmutebutton->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));


    clientboxlayout4->addWidget(settingsbutton);
    clientboxlayout4->addWidget(micmutebutton);
    clientboxlayout4->addWidget(soundmutebutton);
    clientbuttons->setLayout(clientboxlayout4);
    // TODO: Add buttons

    clientboxlayout3->addWidget(clientadvinfo);
    clientboxlayout3->addWidget(clientbuttons);
    clientadvancedinfo->setLayout(clientboxlayout3);


    clientboxlayout->addWidget(clientbasicinfo); //Add upper client container
    clientboxlayout->addWidget(clientadvancedinfo); //Add lower client info text
    clientinfo2->setLayout(clientboxlayout); //Add the outer layout to clientinfo frame

    addDockWidget(Qt::LeftDockWidgetArea, clientinfo);
    viewMenu->addAction(clientinfo->toggleViewAction());


    serverinfo = new QDockWidget(this);
    serverinfo->setAllowedAreas(Qt::TopDockWidgetArea);

    QWidget *titleWidget = new QWidget(this);
    serverinfo->setTitleBarWidget( titleWidget ); /* Trick to hide titlebar */

    widg = new QWidget(serverinfo); //Placeholder
    //widg->setStyleSheet("border: 1px solid rgb(200,200,200); border-radius: 0px;");
    serverinfo->setWidget(widg);

    serverinfo->setFixedHeight((int)(.15*h));
    addDockWidget(Qt::TopDockWidgetArea, serverinfo);

    chatbar = new QDockWidget(this);
    chatbar->setAllowedAreas(Qt::BottomDockWidgetArea);

    titleWidget = new QWidget(this);
    chatbar->setTitleBarWidget( titleWidget ); /* Trick to hide titlebar */

    chatline = new QLineEdit(chatbar);
    chatline->setStyleSheet("border: 1px solid rgb(200,200,200); border-radius: 0px;");
    chatbar->setWidget(chatline);

    addDockWidget(Qt::BottomDockWidgetArea, chatbar);

   /* dock = new QDockWidget(tr("Paragraphs"), this);
    paragraphsList = new QListWidget(dock);
    paragraphsList->addItems(QStringList()
            << "Thank you for your payment which we have received today."
            << "Your order has been dispatched and should be with you "
               "within 28 days."
            << "We have dispatched those items that were in stock. The "
               "rest of your order will be dispatched once all the "
               "remaining items have arrived at our warehouse. No "
               "additional shipping charges will be made."
            << "You made a small overpayment (less than $5) which we "
               "will keep on account for you, or return at your request."
            << "You made a small underpayment (less than $1), but we have "
               "sent your order anyway. We'll add this underpayment to "
               "your next bill."
            << "Unfortunately you did not send enough money. Please remit "
               "an additional $. Your order will be dispatched as soon as "
               "the complete amount has been received."
            << "You made an overpayment (more than $5). Do you wish to "
               "buy more items, or should we return the excess to you?");
    dock->setWidget(paragraphsList);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());*/

    /*connect(customerList, SIGNAL(currentTextChanged(QString)),
            this, SLOT(insertCustomer(QString)));
    connect(paragraphsList, SIGNAL(currentTextChanged(QString)),
            this, SLOT(addParagraph(QString)));*/
}

void MainWindow::resizeEvent(QResizeEvent * event) {


    float h = (float)event->size().height();
    serverinfo->setFixedHeight((int)(.15*h));
    clientinfo2->setFixedHeight((int)(.25*h));
    clientinfo2->setMinimumWidth(2*clientinfo2->height());
    userimagelabel->setFixedHeight((int)(.5*clientinfo2->height())); //Set height of image proportional to clientinfo
    userimagelabel->setFixedWidth((int)(.5*clientinfo2->height()));
    userimagelabel->setPixmap(userimage->scaled(userimagelabel->width(), userimagelabel->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    username->setFixedHeight((int)(.5*clientinfo2->height()));

    settingsbutton->setFixedHeight((int)(.16*clientinfo2->height()));
    settingsbutton->setFixedWidth((int)(.16*clientinfo2->height()));
    settingsbutton->setPixmap(settingsimage->scaled(settingsbutton->width(), settingsbutton->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    micmutebutton->setFixedHeight((int)(.16*clientinfo2->height()));
    micmutebutton->setFixedWidth((int)(.16*clientinfo2->height()));
    micmutebutton->setPixmap(micmuteimage->scaled(micmutebutton->width(), micmutebutton->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    soundmutebutton->setFixedHeight((int)(.16*clientinfo2->height()));
    soundmutebutton->setFixedWidth((int)(.16*clientinfo2->height()));
    soundmutebutton->setPixmap(soundmuteimage->scaled(soundmutebutton->width(), soundmutebutton->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    QMainWindow::resizeEvent(event);
}
