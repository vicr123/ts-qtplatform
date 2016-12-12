#include "messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::setTitle(QString title) {
    ui->titleLabel->setText(title);
    this->setWindowTitle(title);
}

void MessageDialog::setText(QString text) {
    ui->messageLabel->setText(text);
}

void MessageDialog::setIcon(QIcon icon) {
    this->setWindowIcon(icon);

    ui->iconLabel->setPixmap(icon.pixmap(32, 32));
}

void MessageDialog::setButtons(QPlatformDialogHelper::StandardButtons buttons) {
    ui->okButton->setVisible(false);
    ui->yesButton->setVisible(false);
    ui->noButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    if (buttons & QPlatformDialogHelper::Ok) {
        ui->okButton->setVisible(true);
    }
    if (buttons & QPlatformDialogHelper::Yes) {
        ui->yesButton->setVisible(true);
    }
    if (buttons & QPlatformDialogHelper::No) {
        ui->noButton->setVisible(true);
    }
    if (buttons & QPlatformDialogHelper::Cancel) {
        ui->cancelButton->setVisible(true);
    }
}

void MessageDialog::on_okButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Ok, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void MessageDialog::on_yesButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Yes, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void MessageDialog::on_noButton_clicked()
{
    emit clicked(QPlatformDialogHelper::No, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void MessageDialog::on_cancelButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Cancel, QPlatformDialogHelper::AcceptRole);
    this->close();
}
