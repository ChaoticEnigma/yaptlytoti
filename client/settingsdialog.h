#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "client.h"
#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Client *client, QWidget *parent = 0);
    ~SettingsDialog();

private:
    Client *client;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
