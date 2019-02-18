#include "filedialoghelper.h"

#include <QEventLoop>

FileDialogHelper::FileDialogHelper() : QPlatformFileDialogHelper()
{
    dialogWindow = new FileDialog();
    connect(dialogWindow, SIGNAL(accepted()), this, SIGNAL(accept()));
    connect(dialogWindow, SIGNAL(rejected()), this, SIGNAL(reject()));
}

void FileDialogHelper::exec() {
    //updateWindowOptions();
    //dialogWindow->setParent(nullptr);
    dialogWindow->exec();
}

bool FileDialogHelper::show(Qt::WindowFlags windowFlags, Qt::WindowModality windowModality, QWindow *parent) {
    updateWindowOptions();
    dialogWindow->setWindowFlags(windowFlags);
    dialogWindow->setWindowModality(windowModality);
    dialogWindow->setParent(parent);
    dialogWindow->showAfterDelay();

    return true;
}

void FileDialogHelper::hide() {
    dialogWindow->hide();
}

void FileDialogHelper::updateWindowOptions() {
    dialogWindow->setTitle(options()->windowTitle());
    dialogWindow->setAcceptMode(options()->acceptMode());
    dialogWindow->setNameFilters(options()->nameFilters());
    dialogWindow->setFileMode(options()->fileMode());
    dialogWindow->setFilters(options()->filter());
}

bool FileDialogHelper::defaultNameFilterDisables() const {
    return true;
}

void FileDialogHelper::setDirectory(const QUrl &directory) {
    dialogWindow->setCurrentDirectory(directory);
}

QUrl FileDialogHelper::directory() const {
    return dialogWindow->currentDirectory();
}

void FileDialogHelper::selectFile(const QUrl &filename) {

}

QList<QUrl> FileDialogHelper::selectedFiles() const {
    return dialogWindow->selectedFiles();
}

void FileDialogHelper::setFilter() {

}

void FileDialogHelper::selectNameFilter(const QString &filter) {

}

QString FileDialogHelper::selectedNameFilter() const {
    return "";
}
