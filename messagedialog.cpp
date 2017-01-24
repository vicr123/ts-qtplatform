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
    ui->saveButton->setVisible(false);
    ui->saveAllButton->setVisible(false);
    ui->discardButton->setVisible(false);
    ui->closeButton->setVisible(false);

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
    if (buttons & QPlatformDialogHelper::Save) {
        ui->saveButton->setVisible(true);
    }
    if (buttons & QPlatformDialogHelper::SaveAll) {
        ui->saveAllButton->setVisible(true);
    }
    if (buttons & QPlatformDialogHelper::Discard) {
        ui->discardButton->setVisible(true);
    }
    if (buttons & QPlatformDialogHelper::Close) {
        ui->discardButton->setVisible(true);
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

void MessageDialog::on_saveButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Save, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void MessageDialog::on_saveAllButton_clicked()
{
    emit clicked(QPlatformDialogHelper::SaveAll, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void MessageDialog::on_discardButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Discard, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void MessageDialog::on_closeButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Close, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void MessageDialog::show() {
    prepareShow();
    QDialog::show();
}

int MessageDialog::exec() {
    prepareShow();
    return QDialog::exec();
}

void MessageDialog::prepareShow() {
    /*if (this->parent != NULL) {
        QDialog::setWindowFlags(Qt::FramelessWindowHint | flags);
    } else {*/
        QDialog::setWindowFlags(flags);
    //}
}

void MessageDialog::paintEvent(QPaintEvent *event) {
    QRect rect = event->rect();
    if (this->windowFlags() & Qt::FramelessWindowHint) {
        QPainter painter(this);
        painter.setPen(this->palette().color(QPalette::Foreground));
        painter.drawRect(rect);
    }
    QDialog::paintEvent(event);
}

void MessageDialog::setParent(QWindow *parent) {
    this->parent = parent;
}

void MessageDialog::setWindowFlags(Qt::WindowFlags type) {
    this->flags = type;
}
