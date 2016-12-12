#include "messagedialoghelper.h"

MessageDialogHelper::MessageDialogHelper() : QPlatformMessageDialogHelper()
{
    dialogWindow = new MessageDialog();
    connect(dialogWindow, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)), this, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)));
}

void MessageDialogHelper::exec() {
    updateWindowOptions();
    dialogWindow->exec();
}

bool MessageDialogHelper::show(Qt::WindowFlags windowFlags, Qt::WindowModality windowModality, QWindow *parent) {
    updateWindowOptions();
    dialogWindow->setWindowFlags(windowFlags);
    dialogWindow->setWindowModality(windowModality);
    dialogWindow->show();
    return true;
}

void MessageDialogHelper::hide() {
    dialogWindow->hide();
}

void MessageDialogHelper::updateWindowOptions() {
    dialogWindow->setTitle(options().data()->windowTitle());
    dialogWindow->setText(options().data()->text());
    dialogWindow->setButtons(options().data()->standardButtons());
}
