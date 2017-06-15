#include "filedialog.h"
#include "ui_filedialog.h"

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
}

FileDialog::~FileDialog()
{
    delete ui;
}

void FileDialog::setParent(QWindow *parent) {
    this->parent = parent;
}

void FileDialog::setWindowFlags(Qt::WindowFlags type) {
    this->flags = type;
}

void FileDialog::setTitle(QString title) {
    this->setWindowTitle(title);
}

void FileDialog::setCurrentDirectory(QUrl directory) {
    this->directory = directory;
}

QUrl FileDialog::currentDirectory() {
    return this->directory;
}

QList<QUrl> FileDialog::selectedFiles() {
    return QList<QUrl>();
}

void FileDialog::on_okButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Ok, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void FileDialog::on_cancelButton_clicked()
{
    emit clicked(QPlatformDialogHelper::Cancel, QPlatformDialogHelper::AcceptRole);
    this->close();
}

void FileDialog::show() {
    prepareShow();
    QDialog::show();
}

int FileDialog::exec() {
    prepareShow();
    return QDialog::exec();
}

void FileDialog::prepareShow() {
    /*if (this->parent != NULL) {
        QDialog::setWindowFlags(Qt::FramelessWindowHint | flags);
    } else {*/
        QDialog::setWindowFlags(flags);
    //}
}
