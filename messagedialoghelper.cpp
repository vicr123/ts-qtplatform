#include "messagedialoghelper.h"

MessageDialogHelper::MessageDialogHelper() : QPlatformMessageDialogHelper()
{
    dialogWindow = new MessageDialog();
    connect(dialogWindow, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)), this, SIGNAL(clicked(QPlatformDialogHelper::StandardButton,QPlatformDialogHelper::ButtonRole)));
}

void MessageDialogHelper::exec() {
    updateWindowOptions();
    switch (options().data()->icon()) {
        case QMessageDialogOptions::Warning: {
            QSoundEffect* sound = new QSoundEffect();
            sound->setSource(QUrl("qrc:/sounds/warn.wav"));
            sound->play();
            connect(sound, SIGNAL(playingChanged()), sound, SLOT(deleteLater()));
            break;
        }

        case QMessageDialogOptions::Critical: {
            QSoundEffect* sound = new QSoundEffect();
            sound->setSource(QUrl("qrc:/sounds/critical.wav"));
            sound->play();
            connect(sound, SIGNAL(playingChanged()), sound, SLOT(deleteLater()));
            break;
        }
    }

    dialogWindow->setParent(NULL);
    dialogWindow->exec();
}

bool MessageDialogHelper::show(Qt::WindowFlags windowFlags, Qt::WindowModality windowModality, QWindow *parent) {
    updateWindowOptions();
    dialogWindow->setWindowFlags(windowFlags);
    dialogWindow->setWindowModality(windowModality);
    switch (options().data()->icon()) {
        case QMessageDialogOptions::Warning: {
            QSoundEffect* sound = new QSoundEffect();
            sound->setSource(QUrl("qrc:/sounds/warn.wav"));
            sound->play();
            connect(sound, SIGNAL(playingChanged()), sound, SLOT(deleteLater()));
            break;
        }

        case QMessageDialogOptions::Critical: {
            QSoundEffect* sound = new QSoundEffect();
            sound->setSource(QUrl("qrc:/sounds/critical.wav"));
            sound->play();
            connect(sound, SIGNAL(playingChanged()), sound, SLOT(deleteLater()));
            break;
        }
    }

    dialogWindow->setParent(parent);
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

    switch (options().data()->icon()) {
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
    }
}
