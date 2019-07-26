#include "messagedialoghelper.h"
#include <tsystemsound.h>

MessageDialogHelper::MessageDialogHelper() : QPlatformMessageDialogHelper()
{
    dialogWindow = new MessageDialog();
    connect(dialogWindow, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)), this, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)));
}

void MessageDialogHelper::exec() {
    //updateWindowOptions();
    //playDialogSound();

    dialogWindow->setParent(nullptr);
    dialogWindow->exec();
}

bool MessageDialogHelper::show(Qt::WindowFlags windowFlags, Qt::WindowModality windowModality, QWindow *parent) {
    updateWindowOptions();
    playDialogSound();

    dialogWindow->setWindowFlags(windowFlags);
    dialogWindow->setWindowModality(windowModality);

    dialogWindow->setParent(parent);
    dialogWindow->show();
    return true;
}

void MessageDialogHelper::playDialogSound() {

    switch (options()->icon()) {
        case QMessageDialogOptions::Warning:
            if (tSystemSound::isSoundEnabled("dialogwarn")) tSystemSound::play("dialog-warning");
            break;
        case QMessageDialogOptions::Critical:
            if (tSystemSound::isSoundEnabled("dialogerr")) tSystemSound::play("dialog-error");
            break;
        case QMessageDialogOptions::Information:
            if (tSystemSound::isSoundEnabled("dialoginfo")) tSystemSound::play("dialog-information");
            break;
        case QMessageDialogOptions::Question:
            if (tSystemSound::isSoundEnabled("dialogquestion")) tSystemSound::play("dialog-question");
            break;
        case QMessageDialogOptions::NoIcon:
            break;
    }
}

void MessageDialogHelper::hide() {
    dialogWindow->hide();
}

void MessageDialogHelper::updateWindowOptions() {
    dialogWindow->setTitle(options()->windowTitle());
    dialogWindow->setText(options()->text());
    dialogWindow->setDetailedText(options()->detailedText());
    dialogWindow->setButtons(options()->standardButtons(), options()->customButtons());

    switch (options()->icon()) {
        case QMessageDialogOptions::Warning:
            dialogWindow->setIcon(QIcon::fromTheme("dialog-warning"));
            dialogWindow->setSliceColor(QColor(150, 100, 0));
            break;
        case QMessageDialogOptions::Critical:
            dialogWindow->setIcon(QIcon::fromTheme("dialog-error"));
            dialogWindow->setSliceColor(QColor(150, 0, 0));
            break;
        case QMessageDialogOptions::Question:
            dialogWindow->setIcon(QIcon::fromTheme("dialog-question"));
            break;
        case QMessageDialogOptions::Information:
            dialogWindow->setIcon(QIcon::fromTheme("dialog-information"));
            dialogWindow->setSliceColor(QColor(0, 100, 200));
            break;
        case QMessageDialogOptions::NoIcon:
            break;
    }
}

MessageDialogHelper::ButtonRole MessageDialogHelper::buttonRole(StandardButton button) {
    switch (button) {
        case Discard:
            return DestructiveRole;
        default:
            return QPlatformMessageDialogHelper::buttonRole(button);
    }
}
