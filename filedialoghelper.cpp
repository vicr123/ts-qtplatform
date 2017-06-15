#include "filedialoghelper.h"

FileDialogHelper::FileDialogHelper() : QPlatformFileDialogHelper()
{
    dialogWindow = new FileDialog();
    connect(dialogWindow, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)), this, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)));
}

void FileDialogHelper::exec() {
    //updateWindowOptions();
    dialogWindow->setParent(NULL);
    dialogWindow->exec();
}

bool FileDialogHelper::show(Qt::WindowFlags windowFlags, Qt::WindowModality windowModality, QWindow *parent) {
    updateWindowOptions();
    dialogWindow->setWindowFlags(windowFlags);
    dialogWindow->setWindowModality(windowModality);
    dialogWindow->setParent(parent);

    if (parent != NULL) {
        dialogWindow->windowHandle()->setTransientParent(parent);
        dialogWindow->show();
    }

    return true;
}

void FileDialogHelper::hide() {
    dialogWindow->hide();
}

void FileDialogHelper::updateWindowOptions() {
    dialogWindow->setTitle(options().data()->windowTitle());
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
